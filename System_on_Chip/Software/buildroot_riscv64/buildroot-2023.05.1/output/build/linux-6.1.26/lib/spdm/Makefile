SPDM_DIR = PATH/TO/libspdm
SPDM_BUILD_DIR = $(SPDM_DIR)/build_buildroot
SPDM_CRYPTO = mbedtls

SPDM_HEADERS = $(SPDM_DIR)/include

SPDM_INCLUDE := -Iinclude/spdm -Iinclude/spdm/hal -Iinclude/spdm/internal -Iinclude/spdm/include

CFLAGS_spdm_glue.o = $(SPDM_INCLUDE)


SPDM_INCLUDE_DIR =  \
        $(SPDM_HEADERS) \
        $(SPDM_HEADERS)/hal \
        $(SPDM_HEADERS)/internal \
        $(SPDM_HEADERS)/library \
        $(SPDM_HEADERS)/industry_standard \
        $(SPDM_DIR)/library/spdm_common_lib \
        $(SPDM_DIR)/library/spdm_requester_lib \
        $(SPDM_DIR)/library/spdm_secured_message_lib 

SPDM_HEADER_DIR_KERNEL := $(foreach incdir, $(SPDM_INCLUDE_DIR), $(addprefix include/spdm/,$(notdir $(incdir))))


$(SPDM_HEADER_DIR_KERNEL):
	mkdir -p include/spdm
	ln -s $(filter %$(notdir $@), $(SPDM_INCLUDE_DIR)) $@
	if ls $@/*.h >/dev/null 2>&1; then for i in $@/*.h; do ln -s ../../$$i include/spdm/; done; fi
        
SPDM_LIBS = \
        $(SPDM_BUILD_DIR)/lib/libcryptlib_mbedtls.a \
        $(SPDM_BUILD_DIR)/lib/libdebuglib_null.a \
        $(SPDM_BUILD_DIR)/lib/libmalloclib_simple.a \
        $(SPDM_BUILD_DIR)/lib/libmbedcrypto.a \
        $(SPDM_BUILD_DIR)/lib/libmbedtls.a \
        $(SPDM_BUILD_DIR)/lib/libmbedx509.a \
        $(SPDM_BUILD_DIR)/lib/libmemlib.a \
        $(SPDM_BUILD_DIR)/lib/libplatform_lib_null.a \
        $(SPDM_BUILD_DIR)/lib/librnglib_null.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_common_lib.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_crypt_lib.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_crypt_ext_lib.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_device_secret_lib_null.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_requester_lib.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_secured_message_lib.a \
        $(SPDM_BUILD_DIR)/lib/libspdm_transport_mctp_lib.a\
        $(SPDM_BUILD_DIR)/lib/libspdm_transport_pcidoe_lib.a

SPDM_DIR_SUFIX = _spdmlib
SPDM_OBJ  := $(foreach libfile, $(SPDM_LIBS), $(shell ar t $(libfile) | sed s-^-$$(basename $(libfile))$(SPDM_DIR_SUFIX)/-))
SPDM_TARGETS := $(addprefix $(obj)/, $(SPDM_OBJ))

spdm_glue-objs := spdm_glue.o $(SPDM_OBJ)
obj-y += spdm_glue.o

$(SPDM_TARGETS): $(SPDM_LIBS) $(SPDM_HEADER_DIR_KERNEL)
	mkdir -p $$(dirname $@)
	cd $$(dirname $@); ar x $(SPDM_BUILD_DIR)/lib/$$(basename $$(dirname $@ | sed s/$(SPDM_DIR_SUFIX)//)) $$(basename $@)
