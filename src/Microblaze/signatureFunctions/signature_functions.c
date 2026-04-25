#include "signatureFunctions/signature_functions.h"
#include "privateKey/private_key.h"

bool libspdm_get_responder_private_key_from_raw_data(uint32_t base_asym_algo, void **context)
{
    bool result;

    void *rsa_context;
    uint8_t *rsa_n;
    uint8_t *rsa_e;
    uint8_t *rsa_d;
    uint8_t *rsa_p;
    uint8_t *rsa_q;

    size_t rsa_n_size;
    size_t rsa_e_size;
    size_t rsa_d_size;

    size_t rsa_p_size;
    size_t rsa_q_size;

    rsa_n = m_libspdm_rsa2048_res_n;
    rsa_e = m_libspdm_rsa2048_res_e;
    rsa_d = m_libspdm_rsa2048_res_d;

    rsa_q = q;
    rsa_p = p;

    rsa_n_size = sizeof(m_libspdm_rsa2048_res_n);
    rsa_e_size = sizeof(m_libspdm_rsa2048_res_e);
    rsa_d_size = sizeof(m_libspdm_rsa2048_res_d);
    rsa_p_size = sizeof(p);
	rsa_q_size = sizeof(q);

    rsa_context = libspdm_rsa_new();
    if (rsa_context == NULL) {
         return false;
    }
    result = libspdm_rsa_set_key(rsa_context, LIBSPDM_RSA_KEY_N, rsa_n, rsa_n_size);
    if (!result) {
         libspdm_rsa_free(rsa_context);
         return false;
    }
    result = libspdm_rsa_set_key(rsa_context, LIBSPDM_RSA_KEY_E, rsa_e, rsa_e_size);
    if (!result) {
         libspdm_rsa_free(rsa_context);
         return false;
    }
    result = libspdm_rsa_set_key(rsa_context, LIBSPDM_RSA_KEY_D, rsa_d, rsa_d_size);
    if (!result) {
         libspdm_rsa_free(rsa_context);
         return false;
    }

    result = libspdm_rsa_set_key(rsa_context, LIBSPDM_RSA_KEY_P, rsa_p, rsa_p_size);
    result = libspdm_rsa_set_key(rsa_context, LIBSPDM_RSA_KEY_Q, rsa_q, rsa_q_size);

    *context = rsa_context;
    return true;
}

bool libspdm_responder_data_sign(
    spdm_version_number_t spdm_version, uint8_t op_code,
    uint32_t base_asym_algo,
    uint32_t base_hash_algo, bool is_data_hash,
    const uint8_t *message, size_t message_size,
    uint8_t *signature, size_t *sig_size)
{
    void *context;
    bool result;

    result = libspdm_get_responder_private_key_from_raw_data(base_asym_algo, &context);
    if (!result) {
        return false;
    }

    if (is_data_hash) {
        result = libspdm_asym_sign_hash(spdm_version, op_code, base_asym_algo, base_hash_algo,
                                        context,
                                        message, message_size, signature, sig_size);

    } else {
    	result = libspdm_asym_sign(spdm_version, op_code, base_asym_algo,
                                   base_hash_algo, context,
                                   message, message_size,
                                   signature, sig_size);
    }
    libspdm_asym_free(base_asym_algo, context);
    return result;
}
