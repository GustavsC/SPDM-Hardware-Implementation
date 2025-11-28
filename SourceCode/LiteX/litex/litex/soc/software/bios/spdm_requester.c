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
#include <time.h>

#include <bios/spdm_requester.h>
#include <bios/certificates/bundle_responder.certchain.h>
#include <bios/certificates/ca_root_responder.h>

bool read_responder_public_certificate_chain(void *spdm_context, libspdm_data_parameter_t parameter,
    uint32_t base_hash_algo, uint32_t base_asym_algo)
{
    bool res;
    size_t file_size = (size_t) sizeof(cert_chain_responder);
    spdm_cert_chain_t *cert_chain;
    size_t cert_chain_size;

    size_t digest_size;
    bool is_device_cert_model;
    
    const uint8_t *root_cert_buffer;
    size_t root_cert_buffer_size;
    const uint8_t *leaf_cert_buffer;
    size_t leaf_cert_buffer_size;

    /*default is true*/
    is_device_cert_model = true;

    if (base_asym_algo == 0) {
        return false;
    }

    digest_size = libspdm_get_hash_size(base_hash_algo);

    cert_chain_size = sizeof(spdm_cert_chain_t) + digest_size + file_size;
    cert_chain = (void *)malloc(cert_chain_size);
    
    if (cert_chain == NULL) {
        return false;
    }
    cert_chain->length = (uint16_t)cert_chain_size;
    cert_chain->reserved = 0;

    if (cert_chain_size >
        0xFFFF - (sizeof(spdm_cert_chain_t) + LIBSPDM_MAX_HASH_SIZE)) {

        printf("!!! VerifyCertificateChainData - FAIL (chain size too large) !!!\n");
    }
        
    if (!libspdm_x509_get_cert_from_cert_chain(
            cert_chain_responder, file_size, 0, &root_cert_buffer,
            &root_cert_buffer_size)) {
        
        printf("VerifyCertificateChainData - FAIL (get root certificate failed) \n");
    }
        
    if (!libspdm_x509_verify_cert_chain(root_cert_buffer, root_cert_buffer_size,
                                        cert_chain_responder, file_size)){    
            
        printf("VerifyCertificateChainData - FAIL (cert chain verify failed)!!!\n");
    }
            
    if (!libspdm_x509_get_cert_from_cert_chain(
            cert_chain_responder, file_size, -1,
            &leaf_cert_buffer, &leaf_cert_buffer_size)){
            
        printf("VerifyCertificateChainData - FAIL (get leaf certificate failed)!!!\n");         
    }    
        
    if (!libspdm_x509_certificate_check(leaf_cert_buffer, leaf_cert_buffer_size,
                                        base_asym_algo, base_hash_algo,
                                        is_device_cert_model)){
                                        
    printf("VerifyCertificateChainData - FAIL (leaf certificate check failed)!!!\n");
                                        
    }
        
    /* Get Root Certificate and calculate hash value*/

    res = libspdm_hash_all(base_hash_algo, ca_cert_der_responder, sizeof(ca_cert_der_responder),
                           (uint8_t *)(cert_chain + 1));

    if (!res) {
        free(cert_chain);
        printf("Hash Error! \n");
        return false;
    }
    
    libspdm_copy_mem((uint8_t *)cert_chain + sizeof(spdm_cert_chain_t) + digest_size,
                     cert_chain_size - (sizeof(spdm_cert_chain_t) + digest_size),
                     cert_chain_responder, file_size);
    
    libspdm_return_t status = 0x1;

    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    parameter.additional_data[0] = 0;  // slot 0                
    status = libspdm_set_data (spdm_context, LIBSPDM_DATA_PEER_PUBLIC_CERT_CHAIN, &parameter, cert_chain, cert_chain_size);    
    printf("LIBSPDM_DATA_PEER_PUBLIC_CERT_CHAIN - 0x%x\n\n", (uint32_t)status); 
    return true;
}

bool m_send_receive_buffer_acquired = false;
uint8_t m_send_receive_buffer[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
size_t m_send_receive_buffer_size;
uint8_t total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT] = {0xFF,0xFF};

uint32_t m_use_requester_capability_flags =
    (0 |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
     //SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
     //SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHUNK_CAP |
     /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP |    conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP   */
     0);

uint8_t m_use_mut_auth =
    SPDM_KEY_EXCHANGE_RESPONSE_MUT_AUTH_REQUESTED_WITH_ENCAP_REQUEST;
    
uint8_t m_use_measurement_summary_hash_type =
    SPDM_CHALLENGE_REQUEST_ALL_MEASUREMENTS_HASH;

uint8_t m_support_measurement_spec = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
    
uint32_t m_support_measurement_hash_algo =
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_512 |
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_384 |
    SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_256;
    
uint32_t m_support_hash_algo = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_384 |
                               SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256; //|
                               //SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA3_512;

uint32_t m_support_asym_algo =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint16_t m_support_req_asym_algo =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint16_t m_support_dhe_algo = SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_384_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_256_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_3072 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048;

uint16_t m_support_aead_algo =
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_256_GCM |
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_CHACHA20_POLY1305;

uint16_t m_support_key_schedule_algo = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;

uint8_t m_support_other_params_support =
    SPDM_ALGORITHMS_OPAQUE_DATA_FORMAT_1;

uint8_t m_support_mel_spec;

uint8_t m_session_policy =
    SPDM_KEY_EXCHANGE_REQUEST_SESSION_POLICY_TERMINATION_POLICY_RUNTIME_UPDATE;

uint8_t m_end_session_attributes =
    SPDM_END_SESSION_REQUEST_ATTRIBUTES_PRESERVE_NEGOTIATED_STATE_CLEAR;
    

libspdm_return_t spdm_device_acquire_sender_buffer (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    *max_msg_size = sizeof(m_send_receive_buffer);
    *msg_buf_ptr = m_send_receive_buffer;
    m_send_receive_buffer_acquired = true;
    return LIBSPDM_STATUS_SUCCESS;
}

void spdm_device_release_sender_buffer (
    void *context, const void *msg_buf_ptr)
{
    m_send_receive_buffer_acquired = false;
    return;
}

libspdm_return_t spdm_device_acquire_receiver_buffer (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    *max_msg_size = sizeof(m_send_receive_buffer);
    *msg_buf_ptr = m_send_receive_buffer;
    m_send_receive_buffer_acquired = true;
    return LIBSPDM_STATUS_SUCCESS;
}

void spdm_device_release_receiver_buffer (
    void *context, const void *msg_buf_ptr)
{
    m_send_receive_buffer_acquired = false;
    return;
}



#ifdef CSR_ETHPHY_BASE //Ok
libspdm_return_t spdm_requester_send_message(void *spdm_context,
                                             size_t message_size, const void *message,
                                             uint64_t timeout)
{
    
    uint8_t wait = 0x01;
    uint8_t write = 0x05;
    uint8_t responder_signal = 0xF8; 
    
    uint8_t responder_execution = ethphy__register_spdm_SPDM_FIFO_read();
    responder_execution = responder_execution & 0b00100000;
    while(responder_execution != 0x20)
    {
       responder_execution = ethphy__register_spdm_SPDM_FIFO_read();
       responder_execution = responder_execution & 0b00100000;
    }
    
    printf("Message size spdm_requester_send_message %zu \n", message_size);

    // Allocate memory for the uint32_t array
    uint8_t *data = (uint8_t *)malloc(message_size * sizeof(uint32_t));
    
    uint32_t i = 0x0;
    
    ethphy__register_spdm_SPDM_requester_control_write(message_size);
    // Check if memory allocation was successful
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
        
    memcpy(data, message, message_size);
    printf("SPDM Message from requester: ");
       
    ethphy__register_spdm_SPDM_FIFO_write(wait);
        
    while(i < message_size) {
         ethphy__register_spdm_SPDM_register_write(data[i]);
         
         ethphy__register_spdm_SPDM_FIFO_write(write);
         ethphy__register_spdm_SPDM_FIFO_write(wait);
         
         printf("0x%x ", data[i]);              
         i = i + 0x1;                            
    }

    printf("\n\n");    
    free(data);
    
    ethphy__register_spdm_SPDM_FIFO_write(responder_signal); 
    return LIBSPDM_STATUS_SUCCESS;
    
}
#endif
#ifdef CSR_ETHPHY_BASE 
libspdm_return_t spdm_requester_receive_message(void *spdm_context,
                                                size_t *message_size,
                                                void **message,
                                                uint64_t timeout)
{ 

    uint8_t wait = 0x02;
    uint8_t read = 0x06;
    
    uint8_t responder_execution = ethphy__register_spdm_SPDM_FIFO_read();
    responder_execution = responder_execution & 0b00100000;
    
    while(responder_execution != 0x20)
    {
       responder_execution = ethphy__register_spdm_SPDM_FIFO_read();
       responder_execution = responder_execution & 0b00100000;
    }
    ethphy__register_spdm_SPDM_FIFO_write(wait);
         
    message_size = ethphy__register_spdm_SPDM_responder_control_read();
    
    printf("Message size spdm_requester_receive_message %zu \n", message_size);
    printf("SPDM Message from responder: ");
    
    for (size_t i = 0; i < message_size; i++){ //castMessage_size/4
    	
    	m_send_receive_buffer[i] = ethphy__register_spdm_SPDM_register_2_read();
    	ethphy__register_spdm_SPDM_FIFO_write(read);
    	ethphy__register_spdm_SPDM_FIFO_write(wait);
    	
    	printf("0x%x ", m_send_receive_buffer[i]);
    }
    printf("\n\n");   
    *message = m_send_receive_buffer;
    memcpy(*message, m_send_receive_buffer, message_size);
  
    return LIBSPDM_STATUS_SUCCESS;
} 
#endif

void *spdm_requester(void)
{
    uint8_t m_use_secured_message_version = 0x1;
    uint8_t *spdm_context;
    uint8_t *m_spdm_context;
    void *m_scratch_buffer;
    libspdm_return_t status = 0x1;
    
    libspdm_data_parameter_t parameter;

    uint8_t data8;
    uint16_t data16;
    uint32_t data32;
    uint64_t data64;
    
    uint32_t m_use_hash_algo;
    uint32_t m_use_asym_algo;

    spdm_version_number_t spdm_version;
    size_t scratch_buffer_size;
    
    uint8_t slot_mask = 0;
    
    uint8_t slot_id = 0;
    
    uint8_t cert_chain[LIBSPDM_MAX_CERT_CHAIN_SIZE];
    size_t cert_chain_size;
    
    uint8_t measurement_hash_type = 0xFF;
    uint8_t measurement_hash[LIBSPDM_MAX_HASH_SIZE];
     
    printf("Memory used for requester context (in bytes): %zu \n", (uint32_t)libspdm_get_context_size());    

    m_spdm_context = (void *)malloc(libspdm_get_context_size());
    if (m_spdm_context == NULL) {
        return NULL;
    }
    spdm_context = m_spdm_context;
    libspdm_init_context(spdm_context);
    printf("Context Initialzed \n");
    
    printf("Address is %p.\n",spdm_context);

    libspdm_register_device_io_func(spdm_context, spdm_requester_send_message, spdm_requester_receive_message);


     libspdm_register_transport_layer_func(
            spdm_context,
            libspdm_transport_mctp_encode_message,
            libspdm_transport_mctp_decode_message,libspdm_transport_mctp_get_header_size);

    libspdm_register_device_buffer_func(spdm_context,
                                        spdm_device_acquire_sender_buffer,
                                        spdm_device_release_sender_buffer,
                                        spdm_device_acquire_receiver_buffer,
                                        spdm_device_release_receiver_buffer);

    scratch_buffer_size = libspdm_get_sizeof_required_scratch_buffer(m_spdm_context);
    m_scratch_buffer = (void *)malloc(scratch_buffer_size);
    printf("Memory used for requester buffer (in bytes): %zu \n", scratch_buffer_size);
    printf("Address scrath_buffer is %p.\n",m_scratch_buffer);
    
    if (m_scratch_buffer == NULL) {
        free(m_spdm_context);
        m_spdm_context = NULL;
        return NULL;
    }
    libspdm_set_scratch_buffer (spdm_context, m_scratch_buffer, scratch_buffer_size);
      
    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    spdm_version = SPDM_MESSAGE_VERSION_12 << SPDM_VERSION_NUMBER_SHIFT_BIT;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SPDM_VERSION, &parameter,
                     &spdm_version, sizeof(spdm_version));
    printf("LIBSPDM_DATA_SPDM_VERSION - 0x%x\n", (uint32_t)status); 
    /*libspdm_zero_mem(&parameter, sizeof(parameter));
     parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
     spdm_version = m_use_secured_message_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
     status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SECURED_MESSAGE_VERSION,
                         &parameter, &spdm_version,
                         sizeof(spdm_version));
     printf("LIBSPDM_DATA_SECURED_MESSAGE_VERSION - 0x%x\n", (uint32_t)status); */
    
    data8 = 0;
    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT,
                     &parameter, &data8, sizeof(data8));
    printf("LIBSPDM_DATA_CAPABILITY_CT_EXPONENT - 0x%x\n", (uint32_t)status);
       
    data32 = m_use_requester_capability_flags;    
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_CAPABILITY_FLAGS - 0x%x\n", (uint32_t)status);

    data64 = 0x0;
    status = libspdm_set_data (spdm_context, LIBSPDM_DATA_CAPABILITY_RTT_US, &parameter, &data64, sizeof(data64));
    printf("LIBSPDM_DATA_CAPABILITY_RTT_US - 0x%x\n", (uint32_t)status);

    data8 = m_support_measurement_spec;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_MEASUREMENT_SPEC, &parameter,
                     &data8, sizeof(data8));
    printf("LIBSPDM_DATA_MEASUREMENT_SPEC - 0x%x\n", (uint32_t)status);
    
    data32 = m_support_asym_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_ASYM_ALGO, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_BASE_ASYM_ALGO - 0x%x\n", (uint32_t)status);
    
    data32 = m_support_hash_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_HASH_ALGO, &parameter,
                     &data32, sizeof(data32));
    printf("LIBSPDM_DATA_BASE_HASH_ALGO - 0x%x\n", (uint32_t)status);
    
    data16 = m_support_dhe_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_DHE_NAME_GROUP, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_DHE_NAME_GROUP - 0x%x\n", (uint32_t)status);
    
    data16 = m_support_aead_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_AEAD_CIPHER_SUITE, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_AEAD_CIPHER_SUITE - 0x%x\n", (uint32_t)status);
    
    data16 = m_support_req_asym_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_REQ_BASE_ASYM_ALG, &parameter,
                     &data16, sizeof(data16));
    printf("LIBSPDM_DATA_REQ_BASE_ASYM_ALG - 0x%x\n", (uint32_t)status); 
    
    data16 = m_support_key_schedule_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_KEY_SCHEDULE, &parameter, &data16,
                     sizeof(data16));
    printf("LIBSPDM_DATA_KEY_SCHEDULE - 0x%x\n", (uint32_t)status); 
    
    data8 = m_support_other_params_support;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_OTHER_PARAMS_SUPPORT, &parameter,
                     &data8, sizeof(data8));
    printf("LIBSPDM_DATA_OTHER_PARAMS_SUPPORT - 0x%x\n\n", (uint32_t)status); 
    
    m_use_hash_algo = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
    m_use_asym_algo = SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;
    
    read_responder_public_certificate_chain(spdm_context, parameter, m_use_hash_algo,
                                                              m_use_asym_algo);
                   
    
     
    status = libspdm_init_connection(spdm_context, 0);
    printf("libspdm_init_connection - 0x%x\n\n", (uint32_t)status);
    
    if(status!=0x0){
    	printf("Error, connection failed");
    	while(1){
        }
    }
      
    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    status = libspdm_get_digest (spdm_context, slot_mask, total_digest_buffer);
    
    printf("libspdm_get_digest - 0x%x\n\n", (uint32_t)status);
 
    status = libspdm_get_certificate (spdm_context, slot_id, cert_chain_size, cert_chain);
    printf("libspdm_get_certificate - 0x%x\n\n", (uint32_t)status);
    
    status = libspdm_challenge (spdm_context, slot_id, measurement_hash_type, measurement_hash, slot_mask);
    printf("libspdm_challenge - 0x%x\n", (uint32_t)status);
   
    return spdm_context;	
}
