#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>
#include <irq.h>

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
#include <bios/spdmfuncs.h>

#define SOCKET_SPDM_COMMAND_NORMAL 0x0001
#define SOCKET_SPDM_COMMAND_SHUTDOWN 0xFFFE
#define SOCKET_TRANSPORT_TYPE_MCTP 0x01
typedef int SOCKET;

bool m_send_receive_buffer_acquired = false;
uint8_t m_send_receive_buffer[LIBSPDM_SENDER_RECEIVE_BUFFER_SIZE];
size_t m_send_receive_buffer_size;
 
static bool m_error_acquire_sender_buffer = false;
static bool m_error_acquire_receiver_buffer = false;
 
libspdm_return_t spdm_device_acquire_sender_buffer (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    LIBSPDM_ASSERT (!m_send_receive_buffer_acquired);
    if (m_error_acquire_sender_buffer) {
        return LIBSPDM_STATUS_ACQUIRE_FAIL;
    } else {
        *max_msg_size = sizeof(m_send_receive_buffer);
        *msg_buf_ptr = m_send_receive_buffer;
        libspdm_zero_mem (m_send_receive_buffer, sizeof(m_send_receive_buffer));
        m_send_receive_buffer_acquired = true;

        return LIBSPDM_STATUS_SUCCESS;
    }
}

void spdm_device_release_sender_buffer (
    void *context, const void *msg_buf_ptr)
{
    LIBSPDM_ASSERT (m_send_receive_buffer_acquired);
    LIBSPDM_ASSERT (msg_buf_ptr == m_send_receive_buffer);
    
    m_send_receive_buffer_acquired = false;
}

libspdm_return_t spdm_device_acquire_receiver_buffer (
    void *context, size_t *max_msg_size, void **msg_buf_ptr)
{
    LIBSPDM_ASSERT (!m_send_receive_buffer_acquired);

    if (m_error_acquire_receiver_buffer) {
        return LIBSPDM_STATUS_ACQUIRE_FAIL;
    } else {
        *max_msg_size = sizeof(m_send_receive_buffer);
        *msg_buf_ptr = m_send_receive_buffer;
        libspdm_zero_mem (m_send_receive_buffer, sizeof(m_send_receive_buffer));
        m_send_receive_buffer_acquired = true;

        return LIBSPDM_STATUS_SUCCESS;
    }
}

void spdm_device_release_receiver_buffer (
     void *context, const void *msg_buf_ptr)
{
    LIBSPDM_ASSERT (m_send_receive_buffer_acquired);
    LIBSPDM_ASSERT (msg_buf_ptr == m_send_receive_buffer);
    m_send_receive_buffer_acquired = false;
}

SOCKET m_socket;
 
libspdm_return_t spdm_device_send_message(void *spdm_context,
                                       size_t request_size, const void *request,
                                       uint64_t timeout)
{
    bool result;

    result = 1;//send_platform_data(m_socket, SOCKET_SPDM_COMMAND_NORMAL, request, (uint32_t)request_size);
    if (!result) {
        printf("send_platform_data Error - %x\n",
#ifdef _MSC_VER
               WSAGetLastError()
#else
               errno
#endif
               );
        return LIBSPDM_STATUS_SEND_FAIL;
    }
    return LIBSPDM_STATUS_SUCCESS;
}

libspdm_return_t spdm_device_receive_message(void *spdm_context,
                                          size_t *response_size,
                                          void **response,
                                          uint64_t timeout)
{
    bool result;
    uint32_t command;

    result = 1;// receive_platform_data(m_socket, &command, *response, response_size);
    if (!result) {
        printf("receive_platform_data Error - %x\n",
#ifdef _MSC_VER
               WSAGetLastError()
#else
               errno
#endif
               );
        return LIBSPDM_STATUS_RECEIVE_FAIL;
    }
    return LIBSPDM_STATUS_SUCCESS;
}

void spdm_responder(void)
{
	void *spdm_bios_context;
	void *m_spdm_bios_context;
	
	size_t request_size_buffer;
	void *request_scratch_buffer;
	
	size_t spdm_context_size;
		
        libspdm_data_parameter_t parameter;
        
        uint8_t data8;
        uint32_t data32;
        
        uint8_t m_use_secured_message_version = 0;
        
        uint32_t m_use_responder_capability_flags = 
    (0 | SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CACHE_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP |
     /* SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG |    conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG   */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG | /* conflict with SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_FRESH_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP |
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
;
        uint32_t m_use_capability_flags;
        
        spdm_version_number_t spdm_version;
        
        libspdm_return_t status;
        uint8_t m_use_version = 0;
                                  
	spdm_context_size = libspdm_get_context_size();
	printf("Using %zu \n", spdm_context_size);  // prints as bytes
	
	m_spdm_bios_context = (void *)malloc(spdm_context_size); //spdm_context_size
	
	if (m_spdm_bios_context == NULL) {
        	printf("Erro");
        	return NULL;
   	}
   	else
   	   printf("Ok \n");
	
	spdm_bios_context = m_spdm_bios_context;
	
	libspdm_init_context(spdm_bios_context);
	
	printf("Context Initialized \n");
	
	//Buffer initialized
	request_size_buffer = libspdm_get_sizeof_required_scratch_buffer(m_spdm_bios_context);
	
	printf("Using %zu \n", request_size_buffer);  // prints as hexadecimal
	
	request_scratch_buffer = (void *)malloc(request_size_buffer); //request_size_buffer

	if (request_scratch_buffer == NULL) {
          free(m_spdm_bios_context);
          m_spdm_bios_context = NULL;
          return NULL;
   	}
   	else
   	   printf("Ok \n");
   	   
   	libspdm_set_scratch_buffer (spdm_bios_context, request_scratch_buffer, request_size_buffer);
	
	//Device IO
	
	libspdm_register_device_io_func(spdm_bios_context, spdm_device_send_message, spdm_device_receive_message);

        //Transport Layer                        
        libspdm_register_transport_layer_func (spdm_bios_context, libspdm_transport_mctp_encode_message, libspdm_transport_mctp_decode_message, libspdm_transport_mctp_get_header_size);
  		
  	//Buffer Func
	libspdm_register_device_buffer_func (spdm_bios_context, spdm_device_acquire_sender_buffer, spdm_device_release_sender_buffer, spdm_device_acquire_receiver_buffer, spdm_device_release_receiver_buffer);
	
	//Set data version
	libspdm_zero_mem(&parameter, sizeof(parameter));
        parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
	spdm_version = m_use_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
	
	status = libspdm_set_data (spdm_bios_context, LIBSPDM_DATA_SPDM_VERSION, &parameter,&spdm_version,sizeof(spdm_version));
	printf("libspdm_set_data - 0x%x\n", (uint32_t)status);
	
	//Set data 
	libspdm_zero_mem(&parameter, sizeof(parameter));
        parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
        spdm_version = m_use_secured_message_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
        
        status = libspdm_set_data(spdm_bios_context, LIBSPDM_DATA_SECURED_MESSAGE_VERSION, &parameter, &spdm_version,sizeof(spdm_version));                       
	printf("libspdm_set_data - 0x%x\n", (uint32_t)status);
	
	//Set data
    	libspdm_zero_mem(&parameter, sizeof(parameter));
   	parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;

    	data8 = 0;
    	libspdm_set_data(spdm_bios_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT,&parameter, &data8, sizeof(data8));
    	
    	//Set data    	
        //data32 = m_use_capability_flags;
        //m_use_responder_capability_flags = m_use_capability_flags;
        m_use_capability_flags = m_use_responder_capability_flags;
    
    	status = libspdm_set_data(spdm_bios_context, LIBSPDM_DATA_CAPABILITY_FLAGS, &parameter,
                     &data32, sizeof(data32));
        printf("libspdm_set_data - 0x%x\n", (uint32_t)status);
	
	
	while (1) {
     		status = libspdm_responder_dispatch_message (spdm_bios_context);
     		printf("libspdm_set_data - 0x%x\n", (uint32_t)status);
     		if (status == LIBSPDM_STATUS_SUCCESS) {
       		continue;
     	} else
     	   printf("Non SPDM message");
     	   break;
  	} 
  	
  	
  	
       libspdm_register_get_response_func (spdm_bios_context, libspdm_get_response_encapsulated_request);

       libspdm_deinit_context(spdm_bios_context);
       printf("A");
       return m_spdm_bios_context;
	
}


void spdm_requester(void)
{
	void *spdm_bios_context;
	void *m_spdm_bios_context;
	
	size_t request_size_buffer;
	void *request_scratch_buffer;
	
	size_t spdm_context_size;	
		
	libspdm_data_parameter_t parameter;
        
        uint8_t data8 = 0x0;
        
        libspdm_return_t status;
        
        spdm_version_number_t spdm_version = 0x2;
        
        uint8_t m_use_version = 0;
        uint32_t *session_id = 0;
                                  
	spdm_context_size = libspdm_get_context_size();
	printf("Using %zu \n", spdm_context_size);  // prints as bytes
	
	m_spdm_bios_context = (void *)malloc(spdm_context_size); //spdm_context_size
	
	if (m_spdm_bios_context == NULL) {
        	printf("Erro");
        	return NULL;
   	}
   	else
   	   printf("Ok \n");
	
	spdm_bios_context = m_spdm_bios_context;
	
	libspdm_init_context(spdm_bios_context);
	
	printf("Context Initialized \n");
	
	//Buffer initialized
	request_size_buffer = libspdm_get_sizeof_required_scratch_buffer(spdm_bios_context);
	printf("Using %zu \n", request_size_buffer);  // prints as hexadecimal
	
	request_scratch_buffer = (void *)malloc(request_size_buffer); //request_size_buffer

	
	//Device IO
	
	libspdm_register_device_io_func(spdm_bios_context, spdm_device_send_message,
                                    spdm_device_receive_message);

        //Transport Layer                        
        libspdm_register_transport_layer_func (spdm_bios_context, libspdm_transport_mctp_encode_message, libspdm_transport_mctp_decode_message, 			       libspdm_transport_mctp_get_header_size);
  		
  	//Buffer Func
	libspdm_register_device_buffer_func (spdm_bios_context, spdm_device_acquire_sender_buffer, spdm_device_release_sender_buffer, spdm_device_acquire_receiver_buffer, spdm_device_release_receiver_buffer);
	
	libspdm_set_scratch_buffer (spdm_bios_context, request_scratch_buffer, request_size_buffer);
		
	//Set data
	data8 = 0;
	spdm_version = m_use_version << SPDM_VERSION_NUMBER_SHIFT_BIT;
	
	status = libspdm_set_data (spdm_bios_context, LIBSPDM_DATA_SPDM_VERSION, &parameter,&spdm_version,sizeof(spdm_version)); 
	printf("libspdm_set_data - 0x%x\n", (uint32_t)status);     
	
	status = libspdm_set_data (spdm_bios_context, LIBSPDM_DATA_CAPABILITY_CT_EXPONENT, &parameter,&data8,sizeof(data8));
	printf("libspdm_set_data - 0x%x\n", (uint32_t)status);
		
	status = libspdm_init_connection (spdm_bios_context,0); //get version/capabilities/negotiate algorithms
	
	printf("libspdm_init_connection - 0x%x\n", (uint32_t)status);
       
	//libspdm_send_receive_data (spdm_context, &session_id, TRUE, &request, request_size_buffer, &response, &request_size_buffer);
	libspdm_deinit_context(spdm_bios_context);
	return m_spdm_bios_context;
	
}

