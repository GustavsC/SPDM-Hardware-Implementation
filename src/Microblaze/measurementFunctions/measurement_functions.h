#ifndef MEASUREMENTS_FUNCTIONS_H
#define MEASUREMENTS_FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

#define LIBSPDM_MEASUREMENT_BLOCK_HASH_NUMBER 4
#define LIBSPDM_MEASUREMENT_BLOCK_NUMBER (LIBSPDM_MEASUREMENT_BLOCK_HASH_NUMBER /*Index - 1~4*/ + \
                                          1 /*SVN - 0x10*/ + \
                                          1 /*Manifest - 0xFD*/ + 1 /*DEVICE_MODE - 0xFE*/)
#define LIBSPDM_MEASUREMENT_RAW_DATA_SIZE 72
#define LIBSPDM_MEASUREMENT_MANIFEST_SIZE 128
#define LIBSPDM_MEASUREMENT_INDEX_SVN 0x10

size_t libspdm_fill_measurement_image_hash_block (
    bool use_bit_stream,
    uint32_t measurement_hash_algo,
    uint8_t measurements_index,
    spdm_measurement_block_dmtf_t *measurement_block
);

size_t libspdm_fill_measurement_svn_block (
    spdm_measurement_block_dmtf_t *measurement_block
);

size_t libspdm_fill_measurement_manifest_block (
    spdm_measurement_block_dmtf_t *measurement_block
);

size_t libspdm_fill_measurement_device_mode_block (
    spdm_measurement_block_dmtf_t *measurement_block
);

libspdm_return_t libspdm_measurement_collection(
    spdm_version_number_t spdm_version,
    uint8_t measurement_specification,
    uint32_t measurement_hash_algo,
    uint8_t measurements_index,
    uint8_t request_attribute,
    uint8_t *content_changed,
    uint8_t *measurements_count,
    void *measurements,
    size_t *measurements_size
);

bool libspdm_generate_measurement_summary_hash(
    spdm_version_number_t spdm_version, uint32_t base_hash_algo,
    uint8_t measurement_specification, uint32_t measurement_hash_algo,
    uint8_t measurement_summary_hash_type,
    uint8_t *measurement_summary_hash,
    size_t *measurement_summary_hash_size
);

#endif
