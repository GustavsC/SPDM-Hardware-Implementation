#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

#include "generated/mem.h"
#include "generated/csr.h"
#include "generated/soc.h"

//#include "cert/spdm_cert_chain.h"
#include "cert/root_ca.h"
//#include "cert/cert_chain.h"

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

XGpio gpio_spdm_fifo;

XGpio gpio_spdm_clk;

XGpio gpio_spdm_mux;
static const unsigned char spdm_cert_chain_bin[] = {
		0x68, 0x04, 0x00, 0x00, 0x77, 0x63, 0x39, 0xFF, 0x63, 0x7A, 0xA8, 0x0F, 0xC5, 0x95, 0xAB, 0xE7,
			0xD1, 0x3B, 0x4F, 0xB6, 0x6A, 0x0D, 0x8A, 0x75, 0xB7, 0x3A, 0x9A, 0x7F, 0xD5, 0x9C, 0x12, 0x22,
			0x47, 0xF2, 0x31, 0x38, 0x07, 0xD6, 0x59, 0x56, 0x1D, 0xA4, 0x88, 0xD4, 0x98, 0xAE, 0x38, 0x65,
			0x6C, 0xC2, 0x90, 0xD6, 0x30, 0x82, 0x04, 0x30, 0x30, 0x82, 0x02, 0x98, 0xA0, 0x03, 0x02, 0x01,
			0x02, 0x02, 0x01, 0x03, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01,
			0x0B, 0x05, 0x00, 0x30, 0x2D, 0x31, 0x2B, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x22,
			0x44, 0x4D, 0x54, 0x46, 0x20, 0x6C, 0x69, 0x62, 0x73, 0x70, 0x64, 0x6D, 0x20, 0x52, 0x53, 0x41,
			0x20, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6D, 0x65, 0x64, 0x69, 0x61, 0x74, 0x65, 0x20, 0x63, 0x65,
			0x72, 0x74, 0x30, 0x1E, 0x17, 0x0D, 0x32, 0x34, 0x31, 0x30, 0x30, 0x39, 0x31, 0x38, 0x34, 0x38,
			0x32, 0x34, 0x5A, 0x17, 0x0D, 0x33, 0x34, 0x31, 0x30, 0x30, 0x37, 0x31, 0x38, 0x34, 0x38, 0x32,
			0x34, 0x5A, 0x30, 0x2A, 0x31, 0x28, 0x30, 0x26, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x1F, 0x44,
			0x4D, 0x54, 0x46, 0x20, 0x6C, 0x69, 0x62, 0x73, 0x70, 0x64, 0x6D, 0x20, 0x52, 0x53, 0x41, 0x20,
			0x72, 0x65, 0x73, 0x70, 0x6F, 0x6E, 0x64, 0x65, 0x72, 0x20, 0x63, 0x65, 0x72, 0x74, 0x30, 0x82,
			0x01, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05,
			0x00, 0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xAF,
			0x17, 0xF8, 0x54, 0xCF, 0xF4, 0x5B, 0x22, 0x84, 0xAD, 0xB7, 0xA6, 0x18, 0xC6, 0x50, 0xD8, 0xCD,
			0x4B, 0x38, 0xEA, 0xDB, 0x41, 0x87, 0x48, 0x08, 0x1B, 0xFF, 0xB1, 0x11, 0xFE, 0xAF, 0x3B, 0x9E,
			0x6F, 0x3C, 0x2E, 0x50, 0x7C, 0xA8, 0x8E, 0xB5, 0xC2, 0x7D, 0xA2, 0x8A, 0xD7, 0xF8, 0x27, 0x86,
			0x88, 0xFF, 0xC8, 0xB5, 0xDF, 0x41, 0xDB, 0x40, 0x9A, 0x59, 0x58, 0x96, 0x61, 0xB3, 0x8C, 0xEA,
			0x12, 0xB2, 0x3D, 0xF7, 0x9D, 0x4F, 0x37, 0x05, 0xB2, 0x4D, 0x75, 0x17, 0xD7, 0x30, 0x45, 0xF2,
			0x7A, 0x59, 0x37, 0xA0, 0xFE, 0xA3, 0x81, 0x2C, 0x08, 0x85, 0xB9, 0x27, 0xF5, 0xA5, 0x7D, 0xD1,
			0xA5, 0x5A, 0x4C, 0xAA, 0x1E, 0xF0, 0xF3, 0x2F, 0x8A, 0x5B, 0xED, 0x8F, 0x5C, 0xF6, 0x55, 0xDA,
			0xC5, 0xFB, 0x7C, 0xD8, 0x56, 0xF1, 0x5F, 0x89, 0x03, 0xAF, 0x09, 0x23, 0x34, 0x6E, 0xD4, 0xBF,
			0xDB, 0x7D, 0x3A, 0xD8, 0x17, 0x35, 0x34, 0x67, 0x7E, 0xAC, 0x5B, 0xA9, 0x10, 0x9C, 0x32, 0x88,
			0x07, 0xAE, 0xB6, 0x64, 0xC6, 0xC1, 0xFA, 0xF7, 0x12, 0x70, 0x0E, 0xC8, 0x36, 0x85, 0xF9, 0x55,
			0x53, 0x8A, 0x01, 0xCA, 0x65, 0x8C, 0x53, 0xEC, 0x29, 0x19, 0x45, 0x7B, 0x56, 0xE4, 0x23, 0x65,
			0x68, 0x76, 0x7C, 0xB2, 0x79, 0x3A, 0x06, 0x23, 0xAE, 0xB4, 0x39, 0x01, 0x0A, 0x7A, 0x06, 0xED,
			0x27, 0xCE, 0x76, 0x8A, 0x4E, 0x4F, 0xA4, 0xE5, 0xD5, 0x99, 0x8E, 0x5B, 0xE7, 0x57, 0x4C, 0x2A,
			0x37, 0x4E, 0x4F, 0x37, 0x7B, 0xF1, 0x28, 0xF6, 0xEE, 0xB0, 0x9F, 0x9E, 0xE7, 0x0D, 0x00, 0xA2,
			0x09, 0xE3, 0x77, 0x50, 0x37, 0x42, 0xA3, 0x28, 0x68, 0x0C, 0x2F, 0x64, 0xE9, 0xFF, 0x9B, 0x34,
			0x13, 0x7B, 0x92, 0x31, 0x95, 0xE0, 0xDE, 0x9A, 0x8B, 0x6A, 0xB8, 0x21, 0xB2, 0x7D, 0x89, 0x02,
			0x03, 0x01, 0x00, 0x01, 0xA3, 0x81, 0xDD, 0x30, 0x81, 0xDA, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x1D,
			0x13, 0x01, 0x01, 0xFF, 0x04, 0x02, 0x30, 0x00, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x04,
			0x04, 0x03, 0x02, 0x05, 0xE0, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14,
			0x21, 0x48, 0x36, 0xDA, 0x64, 0xC9, 0x30, 0x34, 0x7A, 0x72, 0xFB, 0xA9, 0xEA, 0x9F, 0xE0, 0xEE,
			0xC1, 0xE9, 0x94, 0xFF, 0x30, 0x31, 0x06, 0x03, 0x55, 0x1D, 0x11, 0x04, 0x2A, 0x30, 0x28, 0xA0,
			0x26, 0x06, 0x0A, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x83, 0x1C, 0x82, 0x12, 0x01, 0xA0, 0x18, 0x0C,
			0x16, 0x41, 0x43, 0x4D, 0x45, 0x3A, 0x57, 0x49, 0x44, 0x47, 0x45, 0x54, 0x3A, 0x31, 0x32, 0x33,
			0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x30, 0x2A, 0x06, 0x03, 0x55, 0x1D, 0x25, 0x01, 0x01,
			0xFF, 0x04, 0x20, 0x30, 0x1E, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x06,
			0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x02, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05,
			0x07, 0x03, 0x09, 0x30, 0x1E, 0x06, 0x0A, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x83, 0x1C, 0x82, 0x12,
			0x06, 0x04, 0x10, 0x30, 0x0E, 0x30, 0x0C, 0x06, 0x0A, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x83, 0x1C,
			0x82, 0x12, 0x02, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14,
			0x0A, 0xE2, 0x09, 0xAB, 0x49, 0x21, 0xD3, 0xA3, 0xBF, 0xEF, 0x33, 0x9A, 0x3B, 0x3E, 0x8B, 0x56,
			0x11, 0x74, 0xAB, 0x7C, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01,
			0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x81, 0x00, 0x4B, 0x68, 0x2C, 0x61, 0xDA, 0xC7, 0x2E, 0xBD,
			0x12, 0x22, 0x3B, 0x0B, 0xA3, 0xB4, 0x05, 0x60, 0xFA, 0x76, 0xBA, 0xBA, 0xE9, 0x10, 0x07, 0xE8,
			0xFC, 0x38, 0xF3, 0xF7, 0x87, 0x97, 0xB8, 0xD9, 0x6E, 0xD1, 0x79, 0xCE, 0x66, 0x71, 0x61, 0x4F,
			0x6D, 0x05, 0x44, 0xA4, 0x79, 0xBB, 0xF0, 0x09, 0x42, 0x7D, 0x26, 0x49, 0x47, 0xF9, 0x87, 0x9E,
			0x83, 0x36, 0xEA, 0xBA, 0x5C, 0x41, 0xE0, 0x72, 0xB3, 0xF1, 0x85, 0xA6, 0xC1, 0x09, 0x49, 0xF2,
			0x69, 0xF2, 0x4C, 0x42, 0xCC, 0xFA, 0x26, 0x8C, 0xFA, 0x1E, 0xEA, 0x34, 0x8B, 0x9A, 0x7C, 0x6D,
			0x9F, 0x2C, 0x2A, 0xA1, 0x9F, 0x4F, 0x6D, 0x04, 0xE5, 0x9F, 0xCA, 0xBD, 0xD4, 0x9A, 0x85, 0xA3,
			0x43, 0x14, 0x2C, 0x00, 0x91, 0x07, 0x5F, 0x08, 0xA6, 0x19, 0xEE, 0xBB, 0x23, 0x9B, 0x3E, 0x2C,
			0xF8, 0x74, 0x61, 0xA3, 0x10, 0x6A, 0xB1, 0x1A, 0x53, 0xF2, 0xC4, 0x38, 0x38, 0xB3, 0x40, 0xFB,
			0xE9, 0x63, 0x0E, 0x39, 0xE7, 0x6C, 0xA9, 0xA4, 0x2A, 0x20, 0x32, 0xDA, 0x2A, 0x4D, 0x02, 0xCF,
			0xE8, 0x39, 0x0B, 0xF3, 0x9B, 0xE2, 0x9B, 0x7F, 0xA0, 0xBE, 0x90, 0x1C, 0xAC, 0x18, 0x14, 0xBE,
			0x8A, 0xB5, 0xDA, 0xB4, 0x0F, 0xC9, 0x6E, 0x48, 0xE9, 0xCF, 0xC7, 0xA1, 0x1C, 0x64, 0x57, 0x93,
			0xFA, 0x52, 0x70, 0x27, 0xBF, 0x7A, 0x4B, 0xE1, 0x7E, 0xAF, 0x3E, 0xFF, 0x6A, 0xBA, 0x5E, 0x93,
			0xA5, 0xC4, 0x09, 0xDC, 0x2C, 0x42, 0x88, 0xEC, 0x5F, 0x7D, 0x55, 0x69, 0x33, 0x11, 0xD0, 0x4B,
			0xF2, 0xFE, 0x38, 0x47, 0xD8, 0x9E, 0xF0, 0x92, 0xD6, 0x0C, 0x79, 0xD1, 0x41, 0xE3, 0xDA, 0x8B,
			0x4C, 0xCC, 0x7B, 0xD9, 0x0A, 0xE1, 0xE6, 0x4A, 0x01, 0xA8, 0xDE, 0x5F, 0x02, 0x01, 0xE4, 0xA5,
			0x97, 0xB2, 0x0E, 0x47, 0xF5, 0x24, 0xD5, 0xB0, 0xB1, 0xEA, 0x8C, 0x7A, 0x8D, 0x77, 0xA8, 0xAC,
			0xEC, 0xEA, 0xCF, 0x59, 0xE7, 0xBC, 0xBD, 0xAF, 0x65, 0x10, 0x88, 0x0D, 0xE1, 0x6B, 0x19, 0x81,
			0x68, 0x94, 0x78, 0x13, 0x75, 0x31, 0xCC, 0xA1, 0x29, 0x9F, 0xFC, 0xC0, 0xEA, 0x43, 0x24, 0xBA,
			0x30, 0x05, 0x99, 0x29, 0xBD, 0xE5, 0xCE, 0xCE, 0x85, 0x3F, 0x7A, 0x3A, 0xAA, 0xD2, 0x54, 0xCC,
			0xF6, 0xE9, 0xD1, 0xBD, 0x1D, 0xE8, 0x69, 0x35, 0x8E, 0x6A, 0x1F, 0xBF, 0x7E, 0xCD, 0x64, 0x46,
			0xB0, 0xB5, 0x78, 0xC1, 0x2D, 0x34, 0xD3, 0x97, 0x59, 0xC1, 0x5A, 0x74, 0x07, 0x28, 0x3E, 0x3D,
			0x34, 0x4E, 0xDD, 0x0D, 0x44, 0x4E, 0xF4, 0x2C, 0x33, 0x30, 0x07, 0x09, 0x26, 0xB9, 0x4F, 0xCD,
			0xBB, 0xA4, 0x8C, 0x90, 0xD8, 0xDF, 0x10, 0x47, 0x1E, 0xEE, 0x33, 0xE0, 0x58, 0x83, 0x4E, 0xA1,
			0x14, 0x15, 0xD0, 0x3C, 0xF6, 0x35, 0xC9, 0xA9

};
static const unsigned int spdm_cert_chain_bin_size = 1491;

#define GPIO_OUTPUT 0
#define GPIO_INPUT 1

void *m_spdm_context;

bool m_send_receive_buffer_acquired = false;
 uint8_t m_send_receive_buffer[LIBSPDM_SENDER_RECEIVE_BUFFER_SIZE];
 size_t m_send_receive_buffer_size;

uint8_t m_support_measurement_spec =
    SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;

uint32_t m_support_asym_algo =
    //SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P384 |
   // SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_ECDSA_ECC_NIST_P256 |
	   // SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
	    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
	    //SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
	    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint32_t m_support_hash_algo = //SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_384;// |
                               SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
//uint32_t m_support_hash_algo;
uint16_t m_support_dhe_algo = SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_384_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_SECP_256_R1 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_3072 |
                              SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048;

uint16_t m_support_aead_algo =
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_AES_256_GCM |
    SPDM_ALGORITHMS_AEAD_CIPHER_SUITE_CHACHA20_POLY1305;

uint16_t m_support_req_asym_algo =
    //SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSAPSS_2048 |
   // SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_3072 |
    SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

uint16_t m_support_key_schedule_algo = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;

uint8_t m_support_other_params_support =
    SPDM_ALGORITHMS_OPAQUE_DATA_FORMAT_1;


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

uint32_t m_use_responder_capability_flags =
    (0 |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP | /* conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP */
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP |
     //SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HBEAT_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
     SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHUNK_CAP |
     /* SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP |    conflict with SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP   */
     0);

#include <stdio.h>
#include <string.h>

#define SERIAL_NUMBER 1

bool gettimeofday(tvp, tz)
{
}

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
		XGpio_DiscreteWrite(&gpio_spdm_clk,1, clk_rising);
		XGpio_DiscreteWrite(&gpio_spdm_clk,1, clk_falling);
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
    	XGpio_DiscreteWrite(&gpio_spdm_clk,1, clk_rising);
    	XGpio_DiscreteWrite(&gpio_spdm_clk,1, clk_falling);
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

    XGpio_Initialize(&gpio_spdm_fifo, XPAR_AXI_GPIO_3_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_clk, XPAR_AXI_GPIO_4_DEVICE_ID);

    XGpio_Initialize(&gpio_spdm_mux, XPAR_AXI_GPIO_5_DEVICE_ID);

    XGpio_SetDataDirection(&gpio_spdm_packet, 1, GPIO_OUTPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_packet, 2, GPIO_OUTPUT); //Responder Channel set to Microblaze output

    XGpio_SetDataDirection(&gpio_spdm_counter, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_counter, 2, GPIO_OUTPUT); //Responder Channel set to Microblaze output

    XGpio_SetDataDirection(&gpio_spdm_fifo, 1, GPIO_INPUT); //Requester Channel set to Microblaze input
    XGpio_SetDataDirection(&gpio_spdm_fifo, 2, GPIO_OUTPUT); //Requester Channel set to Microblaze input

    XGpio_SetDataDirection(&gpio_spdm_clk, 1, GPIO_OUTPUT); //Requester Channel set to Microblaze input

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

    void *data;
    size_t data_size;

    uint8_t slot_id = 0x0;
bool res;
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

    data32 = m_use_responder_capability_flags;
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



    res = libspdm_read_public_certificate_chain(SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256,
    		SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048,
            &data, &data_size,
            NULL, NULL);
    while(res!=1){
    	xil_printf("teste - 0x%x\n", (uint32_t)status);
    }

    libspdm_zero_mem(&parameter, sizeof(parameter));
    parameter.location = LIBSPDM_DATA_LOCATION_LOCAL;
    parameter.additional_data[0] = 0;
    libspdm_set_data(spdm_context,
                      LIBSPDM_DATA_LOCAL_PUBLIC_CERT_CHAIN,
                      &parameter, spdm_cert_chain_bin, sizeof(spdm_cert_chain_bin));

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
