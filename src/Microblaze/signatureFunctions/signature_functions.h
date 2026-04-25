#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

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

bool libspdm_get_responder_private_key_from_raw_data(
		uint32_t base_asym_algo,
		void **context
);

bool libspdm_responder_data_sign(
    spdm_version_number_t spdm_version, uint8_t op_code,
    uint32_t base_asym_algo,
    uint32_t base_hash_algo, bool is_data_hash,
    const uint8_t *message, size_t message_size,
    uint8_t *signature, size_t *sig_size
);

#endif
