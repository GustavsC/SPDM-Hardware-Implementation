#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>
#include <irq.h>
#include <math.h>

#include <generated/mem.h>
#include <generated/csr.h>
#include <generated/soc.h>

#include "sfl.h"
#include "boot.h"

#include <libspdm/include/library/spdm_common_lib.h>
#include <libspdm/include/library/spdm_requester_lib.h>
#include <libspdm/include/library/spdm_responder_lib.h>
#include <libspdm/include/library/spdm_lib_config.h>
#include <libspdm/include/library/spdm_device_secret_lib.h>
#include <libspdm/include/library/spdm_return_status.h>
#include <libspdm/include/library/spdm_secured_message_lib.h>
#include <libspdm/include/library/spdm_transport_mctp_lib.h>
#include <libspdm/include/library/spdm_transport_pcidoe_lib.h>
#include <libspdm/include/library/spdm_crypt_lib.h>

#include <libspdm/include/industry_standard/mctp.h>
#include <libspdm/include/industry_standard/spdm.h>
#include <libspdm/include/industry_standard/spdm_secured_message.h>
#include <libspdm/include/industry_standard/pcidoe.h>
#include <libspdm/include/industry_standard/pci_idekm.h>
#include <libspdm/include/industry_standard/pci_tdisp.h>
#include <libspdm/include/industry_standard/pldm.h>
#include <libspdm/include/industry_standard/cxl_idekm.h>

#include <libspdm/include/internal/libspdm_common_lib.h>
#include <libspdm/include/internal/libspdm_crypt_lib.h>
#include <libspdm/include/internal/libspdm_lib_config.h>
#include <libspdm/include/internal/libspdm_macro_check.h>
#include <libspdm/include/internal/libspdm_requester_lib.h>
#include <libspdm/include/internal/libspdm_responder_lib.h>
#include <libspdm/include/internal/libspdm_common_lib.h>
#include <libspdm/include/internal/libspdm_secured_message_lib.h>

#include <libspdm/include/hal/library/memlib.h>
#include <libspdm/include/hal/base.h>
#include <libspdm/include/hal/library/debuglib.h>
#include <libspdm/include/hal/library/platform_lib.h>
#include <libspdm/include/hal/library/cryptlib.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <bios/spdm_responder.h>

bool m_send_receive_buffer_responder_acquired = false;
uint8_t m_send_receive_buffer_responder[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
size_t m_send_receive_buffer_responder_size;

uint32_t m_use_responder_capability_flags =
    (0 | SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CACHE_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP |
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG |    conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG   */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_FRESH_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP |
     //SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP |
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER |    conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER_WITH_CONTEXT   */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER_WITH_CONTEXT | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HBEAT_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP |    conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP   */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHUNK_CAP |
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ALIAS_CERT_CAP | conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_SET_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CSR_CAP | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_INSTALL_RESET_CAP | conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     0);

uint8_t m_use_mut_auth_responder =
    SPDM_KEY_EXCHANGE_RESPONSE_MUT_AUTH_REQUESTED_WITH_ENCAP_REQUEST;
    
uint8_t m_use_measurement_summary_hash_type_responder =
    SPDM_CHALLENGE_REQUEST_ALL_MEASUREMENTS_HASH;

uint8_t m_support_measurement_spec_responder = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
    
uint32_t m_support_measurement_hash_algo_responder =
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_512 |
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_384 |
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_256;
    
uint32_t m_support_hash_algo_responder = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_384 |
                               SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256 |
                               SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA3_512;

uint32_t m_support_asym_algo_responder =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256;

uint16_t m_support_req_asym_algo_responder =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint16_t m_support_dhe_algo_responder = SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_384_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_256_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_3072 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048;

uint16_t m_support_aead_algo_responder =
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_256_GCM |
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_CHACHA20_POLY1305;

uint16_t m_support_key_schedule_algo_responder = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;

uint8_t m_support_other_params_support_responder =
    SPDM_ALGORITHMS_OPAQUE_DATA_FORMAT_1;

uint8_t m_support_mel_spec_responder;

uint8_t m_session_policy_responder =
    SPDM_KEY_EXCHANGE_REQUEST_SESSION_POLICY_TERMINATION_POLICY_RUNTIME_UPDATE;

uint8_t m_end_session_attributes_responder =
    SPDM_END_SESSION_REQUEST_ATTRIBUTES_PRESERVE_NEGOTIATED_STATE_CLEAR;
    
libspdm_return_t spdm_get_response_vendor_defined_request(
    void *spdm_context, const uint32_t *session_id, bool is_app_message,
    size_t request_size, const void *request, size_t *response_size,
    void *response)
{
    LIBSPDM_ASSERT(is_app_message);
    return LIBSPDM_STATUS_SUCCESS;
}

libspdm_return_t spdm_device_acquire_sender_buffer_responder (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    *max_msg_size = sizeof(m_send_receive_buffer_responder);
    *msg_buf_ptr = m_send_receive_buffer_responder;
    m_send_receive_buffer_responder_acquired = true;
    return LIBSPDM_STATUS_SUCCESS;
}

void spdm_device_release_sender_buffer_responder (
    void *context, const void *msg_buf_ptr)
{
    m_send_receive_buffer_responder_acquired = false;
    return;
}

libspdm_return_t spdm_device_acquire_receiver_buffer_responder (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    *max_msg_size = sizeof(m_send_receive_buffer_responder);
    *msg_buf_ptr = m_send_receive_buffer_responder;
    m_send_receive_buffer_responder_acquired = true;
    return LIBSPDM_STATUS_SUCCESS;
}

void spdm_device_release_receiver_buffer_responder (
    void *context, const void *msg_buf_ptr)
{
    m_send_receive_buffer_responder_acquired = false;
    return;
}

#ifdef CSR_ETHPHY_BASE
libspdm_return_t spdm_responder_send_message(void *spdm_context,
                                             size_t message_size, const void *message,
                                             uint64_t timeout)
{


   printf("Message size spdm_responder_send_message %zu \n", message_size);
   printf("SPDM Message from responder: %llx\n", message);
   //ethphy__register_spdm_SPDM_register_2_write(message);
   
   return LIBSPDM_STATUS_SUCCESS;        
}
#endif

#ifdef CSR_ETHPHY_BASE //Ok
libspdm_return_t spdm_responder_receive_message(void *spdm_context,
                                                size_t *message_size,
                                                void **message,
                                                uint64_t timeout)
{

    printf("Message size spdm_responder_receive_message %zu \n", *message_size);
    message = ethphy__register_spdm_SPDM_register_read();
    printf("Message spdm_responder_receive_message %zu \n", message);   
    return LIBSPDM_STATUS_SUCCESS;
}
#endif


void *spdm_responder(void)
{
    void *spdm_context;
    void *m_spdm_context;
    void *m_scratch_buffer;
    libspdm_data_parameter_t parameter;
   
    uint8_t data8;
    uint16_t data16;
    uint32_t data32;
    
    uint8_t m_use_secured_message_version = 0x1;
    
    spdm_version_number_t spdm_version;
    libspdm_return_t status = 0x1;
    size_t scratch_buffer_size;

    size_t spdm_context_size = libspdm_get_context_size();
	
    printf("Memory used for responder context (in bytes): %zu \n", spdm_context_size);

    m_spdm_context = (void *)malloc(libspdm_get_context_size());
    if (m_spdm_context == NULL) {
        return NULL;
    }
    spdm_context = m_spdm_context;
    printf("0x%x \n", spdm_context);
    printf("0x%x \n", m_spdm_context);
    libspdm_init_context(spdm_context);
    printf("Context Initialzed \n");
    

    libspdm_register_device_io_func(spdm_context, spdm_responder_send_message,
                                    spdm_responder_receive_message);

    libspdm_register_transport_layer_func(spdm_context, libspdm_transport_mctp_encode_message,
            libspdm_transport_mctp_decode_message,libspdm_transport_mctp_get_header_size);

    libspdm_register_device_buffer_func(spdm_context,
                                        spdm_device_acquire_sender_buffer_responder,
                                        spdm_device_release_sender_buffer_responder,
                                        spdm_device_acquire_receiver_buffer_responder,
                                        spdm_device_release_receiver_buffer_responder);

    scratch_buffer_size = libspdm_get_sizeof_required_scratch_buffer(m_spdm_context);
    m_scratch_buffer = (void *)malloc(scratch_buffer_size);
    printf("Address scrath_buffer is %p.\n", m_scratch_buffer);
    if (m_scratch_buffer == NULL) {
        free(m_spdm_context);
        m_spdm_context = NULL;
        return NULL;
    }
    
    libspdm_set_scratch_buffer (spdm_context, m_scratch_buffer, scratch_buffer_size);
    printf("Memory used for responder buffer (in bytes): %zu \n\n", scratch_buffer_size);
    
    /* set version */
    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    spdm_version = SPDM_MESSAGE_VERSION_12 << SPDM_VERSION_NUMBER_SHIFT_BIT;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SPDM_VERSION, &parameter,
                         &spdm_version, sizeof(spdm_version));
    printf("LIBSPDM_DATA_SPDM_VERSION - 0x%x\n", (uint32_t)status);
    
     libspdm_zero_mem(&parameter, sizeof(parameter));
     parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
     spdm_version = m_use_secured_message_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
     status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SECURED_MESSAGE_VERSION,
                         &parameter, &spdm_version,
                         sizeof(spdm_version));
     printf("LIBSPDM_DATA_SECURED_MESSAGE_VERSION - 0x%x\n", (uint32_t)status);
    
    data8 = 0;
    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT,
                     &parameter, &data8, sizeof(data8));
    printf("LIBSPDM_DATA_CAPABILITY_CT_EXPONENT - 0x%x\n", (uint32_t)status);
   
    data32 = m_use_responder_capability_flags;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_CAPABILITY_FLAGS - 0x%x\n", (uint32_t)status);

    /* algorithm */
    data8 = m_support_measurement_spec_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_MEASUREMENT_SPEC, &parameter,
                     &data8, sizeof(data8));
    printf("LIBSPDM_DATA_MEASUREMENT_SPEC - 0x%x\n", (uint32_t)status); 
    
    data32 = m_support_measurement_hash_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_MEASUREMENT_HASH_ALGO, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_MEASUREMENT_HASH_ALGO - 0x%x\n", (uint32_t)status); 
    
    data32 = m_support_asym_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_ASYM_ALGO, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_BASE_ASYM_ALGO - 0x%x\n", (uint32_t)status); 
                     
    data32 = m_support_hash_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_HASH_ALGO, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_BASE_HASH_ALGO - 0x%x\n", (uint32_t)status); 
                     
    data16 = m_support_dhe_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_DHE_NAME_GROUP, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_DHE_NAME_GROUP - 0x%x\n", (uint32_t)status); 
                    
    data16 = m_support_aead_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_AEAD_CIPHER_SUITE, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_AEAD_CIPHER_SUITE - 0x%x\n", (uint32_t)status);    
                 
    data16 = m_support_req_asym_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_REQ_BASE_ASYM_ALG, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_REQ_BASE_ASYM_ALG - 0x%x\n", (uint32_t)status); 
                     
    data16 = m_support_key_schedule_algo_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_KEY_SCHEDULE, &parameter, &data16,
                     sizeof(data16));
    printf("LIBSPDM_DATA_KEY_SCHEDULE - 0x%x\n", (uint32_t)status); 
         
                 
    data8 = m_support_other_params_support_responder;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_OTHER_PARAMS_SUPPORT, &parameter,
                     &data8, sizeof(data8));
    printf("LIBSPDM_DATA_OTHER_PARAMS_SUPPORT - 0x%x\n", (uint32_t)status); 
                 
    data8 = 0xF0;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_HEARTBEAT_PERIOD, &parameter,
                     &data8, sizeof(data8));
    printf("LIBSPDM_DATA_HEARTBEAT_PERIOD - 0x%x\n \n", (uint32_t)status); 
    
    while (1) {
     		status = libspdm_responder_dispatch_message (spdm_context);
     		printf("libspdm_responder_dispatch_message - 0x%x\n", (uint32_t)status);
     		if (status == LIBSPDM_STATUS_SUCCESS) {
       		continue;
     	} else
     	   //printf("Non SPDM message");
     	   break;
  	} 
    
    libspdm_register_get_response_func(
        spdm_context, spdm_get_response_vendor_defined_request);
    
    return m_spdm_context;	
}


