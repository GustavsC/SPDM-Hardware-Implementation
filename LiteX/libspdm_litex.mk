SPDM_DIR?= /PATH/TO/libspdm_docs/libspdm
LiteX_DIR?= /PATH/TO/LiteX/litex/litex/soc/software
SPDM_HEADERS = $(SPDM_DIR)/include

SPDM_INCLUDE_DIR =  \
        $(SPDM_HEADERS) \
        $(SPDM_HEADERS)/hal \
        $(SPDM_HEADERS)/library \
        $(SPDM_HEADERS)/industry_standard \
        $(SPDM_DIR)/library/spdm_common_lib \
        $(SPDM_DIR)/library/spdm_crypt_lib \
        $(SPDM_DIR)/library/spdm_requester_lib \
        $(SPDM_DIR)/library/spdm_responder_lib \
        $(SPDM_DIR)/library/spdm_transport_mctp_lib \
        $(SPDM_DIR)/library/spdm_transport_pcidoe_lib \
        $(SPDM_DIR)/library/spdm_secured_message_lib 

SPDM_HEADER_DIR_KERNEL := $(foreach incdir, $(SPDM_INCLUDE_DIR), $(addprefix libspdm/,$(notdir $(incdir))))

$(SPDM_HEADER_DIR_KERNEL):
	cd $(LiteX_DIR) && mkdir -p libspdm/
	cd $(LiteX_DIR) && ln -s $(filter %$(notdir $@), $(SPDM_INCLUDE_DIR)) $@
	cd $(LiteX_DIR) && cd libspdm/ && if ls $@/*.h >/dev/null 2>&1; then for i in $@/*.h; do ln -s ../../$$i libspdm/; done; fi
