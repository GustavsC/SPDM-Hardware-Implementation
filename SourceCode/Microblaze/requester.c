#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

#include "generated/mem.h"
#include "generated/csr.h"
#include "generated/soc.h"

#include "libspdm/include/library/spdm_common_lib.h"
#include "libspdm/include/library/spdm_requester_lib.h"
#include "libspdm/include/library/spdm_responder_lib.h"
#include "libspdm/include/library/spdm_lib_config.h"
#include "libspdm/include/library/spdm_device_secret_lib.h"
#include "libspdm/include/library/spdm_return_status.h"
#include "libspdm/include/library/spdm_secured_message_lib.h"
#include "libspdm/include/library/spdm_transport_mctp_lib.h"
#include "libspdm/include/library/spdm_transport_pcidoe_lib.h"
#include "libspdm/include/library/spdm_crypt_lib.h"

#include "libspdm/include/industry_standard/mctp.h"
#include "libspdm/include/industry_standard/spdm.h"
#include "libspdm/include/industry_standard/spdm_secured_message.h"
#include "libspdm/include/industry_standard/pcidoe.h"
#include "libspdm/include/industry_standard/pci_idekm.h"
#include "libspdm/include/industry_standard/pci_tdisp.h"
#include "libspdm/include/industry_standard/pldm.h"
#include "libspdm/include/industry_standard/cxl_idekm.h"

#include "libspdm/include/internal/libspdm_common_lib.h"
#include "libspdm/include/internal/libspdm_crypt_lib.h"
#include "libspdm/include/internal/libspdm_lib_config.h"
#include "libspdm/include/internal/libspdm_macro_check.h"
#include "libspdm/include/internal/libspdm_requester_lib.h"
#include "libspdm/include/internal/libspdm_responder_lib.h"
#include "libspdm/include/internal/libspdm_common_lib.h"
#include "libspdm/include/internal/libspdm_secured_message_lib.h"

#include "libspdm/include/hal/library/memlib.h"
#include "libspdm/include/hal/base.h"
#include "libspdm/include/hal/library/debuglib.h"
#include "libspdm/include/hal/library/platform_lib.h"
#include "libspdm/include/hal/library/cryptlib.h"

XGpio gpio_spdm_packet;

XGpio gpio_spdm_counter;

XGpio gpio_spdm_context_packet;

XGpio gpio_spdm_fifo;

XGpio gpio_spdm_empty;

XGpio gpio_spdm_mux;


#define GPIO_OUTPUT 0
#define GPIO_INPUT 1

void *m_spdm_context;

bool m_send_receive_buffer_acquired = false;
 uint8_t m_send_receive_buffer[LIBSPDM_SENDER_RECEIVE_BUFFER_SIZE];
 size_t m_send_receive_buffer_size;

uint8_t m_support_measurement_spec =
    SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;

uint32_t m_support_asym_algo =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256;

uint32_t m_support_hash_algo = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_384 |
                               SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;

uint16_t m_support_dhe_algo = SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_384_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_256_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_3072 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048;

uint16_t m_support_aead_algo =
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_256_GCM |
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_CHACHA20_POLY1305;

uint16_t m_support_req_asym_algo =
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint16_t m_support_key_schedule_algo = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;

uint8_t m_support_other_params_support =
    SPDM_ALGORITHMS_OPAQUE_DATA_FORMAT_1;

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

uint32_t m_use_requester_capability_flags =
    (0 |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
     //SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
     SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHUNK_CAP |
     /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP |    conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP   */
     0);

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

void spdm_server_connection_state_callback(
    void *spdm_context, libspdm_connection_state_t connection_state)
{

}

void spdm_server_session_state_callback(void *spdm_context,
                                        uint32_t session_id,
                                        libspdm_session_state_t session_state)
{
}


libspdm_return_t spdm_responder_send_message(void *spdm_context,
                                             size_t message_size, const void *message,
                                             uint64_t timeout)
{

	uint8_t *data = (uint8_t *)malloc(message_size);

	uint8_t enable = 0x1;
    uint8_t clk_rising = 0x1;
    uint8_t clk_falling = 0x0;

    // Check if memory allocation was successful
	if (data == NULL) {
	     printf("Memory allocation failed\n");
	        return 1;
	 }

	memcpy(data, message, message_size);
	XGpio_DiscreteWrite(&gpio_spdm_counter,2, message_size);
	XGpio_DiscreteWrite(&gpio_spdm_fifo,1, enable);

	for (size_t i = 0; i < message_size; i++) {
		XGpio_DiscreteWrite(&gpio_spdm_packet,2, data[i]);
		XGpio_DiscreteWrite(&gpio_spdm_empty,2, clk_rising);
		XGpio_DiscreteWrite(&gpio_spdm_empty,2, clk_falling);
	}

	free(data);
	XGpio_DiscreteWrite(&gpio_spdm_fifo,1, 0x0);
	XGpio_DiscreteWrite(&gpio_spdm_mux,2, enable);
	return LIBSPDM_STATUS_SUCCESS;

}

libspdm_return_t spdm_responder_receive_message(void *spdm_context,
                                                size_t *message_size,
                                                void **message,
                                                uint64_t timeout)
{

	uint8_t requester_writing = XGpio_DiscreteRead(&gpio_spdm_mux,1);
    while(requester_writing == 0x0){
    	requester_writing = XGpio_DiscreteRead(&gpio_spdm_mux,1);
    }

	XGpio_DiscreteWrite(&gpio_spdm_mux,2, 0x0);

	message_size = XGpio_DiscreteRead(&gpio_spdm_counter,1);

    uint8_t enable = 0x1;
    uint8_t clk_rising = 0x1;
    uint8_t clk_falling = 0x0;

    XGpio_DiscreteWrite(&gpio_spdm_fifo,2, enable);

    for (size_t i = 0; i < message_size; i++){
    	m_send_receive_buffer[i] = XGpio_DiscreteRead(&gpio_spdm_packet,1);
    	XGpio_DiscreteWrite(&gpio_spdm_empty,2, clk_rising);
    	XGpio_DiscreteWrite(&gpio_spdm_empty,2, clk_falling);
    }
    XGpio_DiscreteWrite(&gpio_spdm_fifo,2, 0x0);

    *message = m_send_receive_buffer;

    memcpy(*message, m_send_receive_buffer, message_size);
    //free(message);
    return LIBSPDM_STATUS_SUCCESS;

}

libspdm_return_t libspdm_get_response_vendor_defined_request (
     void           *spdm_context,
     const uint32_t *session_id,
     bool            is_app_message,
     size_t          request_size,
     const void     *request,
     size_t         *response_size,
     void           *response
   )
{
	return LIBSPDM_STATUS_SUCCESS;
}

int main()
{
    init_platform();

    XGpio_Initialize(&gpio_spdm_packet, XPAR_AXI_GPIO_0_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_counter, XPAR_AXI_GPIO_1_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_context_packet, XPAR_AXI_GPIO_2_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_fifo, XPAR_AXI_GPIO_3_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_empty, XPAR_AXI_GPIO_4_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_mux, XPAR_AXI_GPIO_5_DEVICE_ID);

    XGpio_SetDataDirection(&gpio_spdm_packet, 1, GPIO_OUTPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_packet, 2, GPIO_OUTPUT); //Responder Channel set to Microblaze output

    XGpio_SetDataDirection(&gpio_spdm_counter, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_counter, 2, GPIO_OUTPUT); //Responder Channel set to Microblaze output

    XGpio_SetDataDirection(&gpio_spdm_context_packet, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_context_packet, 2, GPIO_OUTPUT); //Responder Channel set to Microblaze output

    XGpio_SetDataDirection(&gpio_spdm_fifo, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_fifo, 2, GPIO_OUTPUT); //Requester Channel set to Microblaze input

    XGpio_SetDataDirection(&gpio_spdm_empty, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_empty, 2, GPIO_OUTPUT); //Requester Channel set to Microblaze input

    XGpio_SetDataDirection(&gpio_spdm_mux, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_mux, 2, GPIO_OUTPUT); //Requester Channel set to Microblaze input

    uint8_t start = 0x1;

    void *spdm_context;
    void *m_scratch_buffer;
    libspdm_data_parameter_t parameter;
    spdm_version_number_t spdm_version;
    uint8_t m_use_secured_message_version = 0x1;

    uint8_t data8;
    uint16_t data16;
    uint32_t data32;

    libspdm_return_t status = 0x1;
    size_t scratch_buffer_size = 0;

    m_spdm_context = (void *)malloc(libspdm_get_context_size());

    if (m_spdm_context == NULL) {
    	print("Context NULL\n");
    	return NULL;
    }

    spdm_context = m_spdm_context;
    libspdm_init_context(spdm_context);

    libspdm_register_device_io_func(spdm_context, spdm_responder_send_message,
                                            spdm_responder_receive_message);

    libspdm_register_transport_layer_func(spdm_context, libspdm_transport_mctp_encode_message,
    		libspdm_transport_mctp_decode_message,libspdm_transport_mctp_get_header_size);

    libspdm_register_device_buffer_func(spdm_context,
                                            spdm_device_acquire_sender_buffer,
                                            spdm_device_release_sender_buffer,
                                            spdm_device_acquire_receiver_buffer,
                                            spdm_device_release_receiver_buffer);

   scratch_buffer_size = libspdm_get_sizeof_required_scratch_buffer(m_spdm_context);
   m_scratch_buffer = (void *)malloc(scratch_buffer_size);

   if (m_scratch_buffer == NULL) {
	      free(m_spdm_context);
          return NULL;
   }

    libspdm_set_scratch_buffer (spdm_context, m_scratch_buffer, scratch_buffer_size);

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    spdm_version = SPDM_MESSAGE_VERSION_12 << SPDM_VERSION_NUMBER_SHIFT_BIT;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SPDM_VERSION, &parameter, &spdm_version, sizeof(spdm_version));
    xil_printf("LIBSPDM_DATA_SPDM_VERSION - 0x%x\n", (uint32_t)status);

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    spdm_version = m_use_secured_message_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_SECURED_MESSAGE_VERSION,
                             &parameter, &spdm_version,
                             sizeof(spdm_version));
    xil_printf("LIBSPDM_DATA_SECURED_MESSAGE_VERSION - 0x%x\n", (uint32_t)status);

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;


    data8 = 0;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT,
                         &parameter, &data8, sizeof(data8));
    xil_printf("LIBSPDM_DATA_CAPABILITY_CT_EXPONENT - 0x%x\n", (uint32_t)status);

    data32 = m_use_requester_capability_flags;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                         &data32, sizeof(data32));
    xil_printf("LIBSPDM_DATA_CAPABILITY_FLAGS - 0x%x\n", (uint32_t)status);

    data8 = m_support_measurement_spec;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_MEASUREMENT_SPEC, &parameter,
                         &data8, sizeof(data8));
    xil_printf("LIBSPDM_DATA_MEASUREMENT_SPEC - 0x%x\n", (uint32_t)status);

    data32 = m_support_asym_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_ASYM_ALGO, &parameter,
                         &data32, sizeof(data32));
    xil_printf("LIBSPDM_DATA_BASE_ASYM_ALGO - 0x%x\n", (uint32_t)status);

    data32 = m_support_hash_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_BASE_HASH_ALGO, &parameter,
                     &data32, sizeof(data32));
    xil_printf("LIBSPDM_DATA_BASE_HASH_ALGO - 0x%x\n", (uint32_t)status);

    data16 = m_support_dhe_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_DHE_NAME_GROUP, &parameter,
                         &data16, sizeof(data16));
    xil_printf("LIBSPDM_DATA_DHE_NAME_GROUP - 0x%x\n", (uint32_t)status);

    data16 = m_support_aead_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_AEAD_CIPHER_SUITE, &parameter,
                         &data16, sizeof(data16));
    xil_printf("LIBSPDM_DATA_AEAD_CIPHER_SUITE - 0x%x\n", (uint32_t)status);

    data16 = m_support_req_asym_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_REQ_BASE_ASYM_ALG, &parameter,
                     &data16, sizeof(data16));

    data16 = m_support_key_schedule_algo;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_KEY_SCHEDULE, &parameter, &data16,
                     sizeof(data16));

    data8 = m_support_other_params_support;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_OTHER_PARAMS_SUPPORT, &parameter,
                     &data8, sizeof(data8));
    xil_printf("LIBSPDM_DATA_OTHER_PARAMS_SUPPORT - 0x%x\n", (uint32_t)status);

    data8 = 0xF0;
    status = libspdm_set_data(spdm_context, LIBSPDM_DATA_HEARTBEAT_PERIOD, &parameter,
                         &data8, sizeof(data8));
    xil_printf("LIBSPDM_DATA_HEARTBEAT_PERIOD - 0x%x\n", (uint32_t)status);

    XGpio_DiscreteWrite(&gpio_spdm_mux,2, start);
	while (1) {
    	status = libspdm_responder_dispatch_message (spdm_context);
        if (status == LIBSPDM_STATUS_UNSUPPORTED_CAP) {
    	    break;
        }
     }

	libspdm_register_get_response_func(spdm_context, libspdm_get_response_vendor_defined_request);

	libspdm_register_session_state_callback_func(
        spdm_context, spdm_server_session_state_callback);

	libspdm_register_connection_state_callback_func(
        spdm_context, spdm_server_connection_state_callback);

    //cleanup_platform();
    libspdm_deinit_context(spdm_context);
    return spdm_context;
}
