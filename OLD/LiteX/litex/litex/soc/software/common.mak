SPDM_DIR?= /PATH/TO/libspdm
SPDM_BUILD_DIR?= $(SPDM_DIR)/build
SPDM_HEADERS = $(SPDM_DIR)/include
SPDM_FINAL?= $(SPDM_DIR)/build/lib
SPDM_CRYPTO?= mbedtls
LiteX_DIR?= /PATH/TO/LiteX/litex/litex/soc/software

#LibSPDM Libraries
LIBRARIES = cmockalib rnglib memlib malloclib debuglib cryptlib_$(SPDM_CRYPTO) $(SPDM_CRYPTO) spdm_crypt_lib spdm_secured_message_lib spdm_requester_lib spdm_device_secret_lib_null spdm_common_lib spdm_transport_mctp_lib spdm_transport_pcidoe_lib spdm_device_secret_lib_sample spdm_responder_lib cryptlib_null cryptstublib_dummy debuglib_null intrinsiclib malloclib_null malloclib_simple mbedcrypto mbedx509 platform_lib platform_lib_null rnglib_null spdm_crypt_ext_lib spdm_transport_test_lib

ifeq ($(TRIPLE),--native--)
TARGET_PREFIX=
else
TARGET_PREFIX=$(TRIPLE)-
endif

RM ?= rm -f
PYTHON ?= python3
CCACHE ?=

ifeq ($(CLANG),1)
CC_normal      := $(CCACHE) clang -target $(TRIPLE) -integrated-as -static #-static flag added
CX_normal      := $(CCACHE) clang++ -target $(TRIPLE) -integrated-as -static #-static flag added
else
CC_normal      := $(CCACHE) $(TARGET_PREFIX)gcc -std=gnu99 -static
CX_normal      := $(CCACHE) $(TARGET_PREFIX)g++ -static #-static flag added
endif
AR_normal      := $(TARGET_PREFIX)gcc-ar
LD_normal      := $(TARGET_PREFIX)ld -static
OBJCOPY_normal := $(TARGET_PREFIX)objcopy

CC_quiet      = @echo " CC      " $@ && $(CC_normal)
CX_quiet      = @echo " CX      " $@ && $(CX_normal)
AR_quiet      = @echo " AR      " $@ && $(AR_normal)
LD_quiet      = @echo " LD      " $@ && $(LD_normal)
OBJCOPY_quiet = @echo " OBJCOPY " $@ && $(OBJCOPY_normal)

ifeq ($(V),1)
	CC = $(CC_normal)
	CX = $(CX_normal)
	AR = $(AR_normal)
	LD = $(LD_normal)
	OBJCOPY = $(OBJCOPY_normal)
else
	CC = $(CC_quiet)
	CX = $(CX_quiet)
	AR = $(AR_quiet)
	LD = $(LD_quiet)
	OBJCOPY = $(OBJCOPY_quiet)
endif

# http://scottmcpeak.com/autodepend/autodepend.html
# Generate *.d Makefile dependencies fragments, include using;
# -include $(OBJECTS:.o=.d)
DEPFLAGS += -MD -MP

# Toolchain options
#
INCLUDES = -I$(PICOLIBC_DIRECTORY)/newlib/libc/tinystdio \
           -I$(PICOLIBC_DIRECTORY)/newlib/libc/include \
           -I$(LIBBASE_DIRECTORY) \
           -I$(SOC_DIRECTORY)/software/include \
           -I$(SOC_DIRECTORY)/software \
           -I$(BUILDINC_DIRECTORY) \
           -I$(BUILDINC_DIRECTORY)/../libc \
           -I$(CPU_DIRECTORY) \
           -I$(SPDM_HEADERS) \
           -I$(SPDM_HEADERS)/hal \
           -I$(SPDM_HEADERS)/library \
           -I$(SPDM_HEADERS)/industry_standard \
           -I$(SPDM_HEADERS)/internal \
           -I$(SPDM_DIR)/library/spdm_common_lib \
           -I$(SPDM_DIR)/library/spdm_crypt_lib \
           -I$(SPDM_DIR)/library/spdm_requester_lib \
           -I$(SPDM_DIR)/library/spdm_responder_lib \
           -I$(SPDM_DIR)/library/spdm_transport_mctp_lib \
           -I$(SPDM_DIR)/library/spdm_transport_pcidoe_lib \
           -I$(SPDM_DIR)/library/spdm_secured_message_lib 
           
COMMONFLAGS = $(DEPFLAGS) -Os $(CPUFLAGS) -g3 -no-pie -fomit-frame-pointer -static -Wall -fno-builtin -fno-stack-protector $(INCLUDES) 
ifeq ($(LTO), 1)
COMMONFLAGS += -flto
endif
ifneq ($(CPUFAMILY), arm)
COMMONFLAGS += -fexceptions
endif
CFLAGS = $(COMMONFLAGS) -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -static #-static flag added
CXXFLAGS = $(COMMONFLAGS) -std=c++11 -I$(SOC_DIRECTORY)/software/include/basec++ -I$(SPDM_BUILD_DIR)/lib -fno-rtti -ffreestanding -static #-static flag added
LDFLAGS = -nostdlib -C -nodefaultlibs -Wl,--no-dynamic-linker -Wl,--build-id=none $(CFLAGS) -L$(BUILDINC_DIRECTORY) -static -L$(SPDM_FINAL) $(addprefix -l, $(LIBRARIES)) #-static flag added with -L$

define compilexx
$(CX) -c $(CXXFLAGS) $(1) $< -o $@
endef

define compile
$(CC) -c $(CFLAGS) $(1) $< -o $@
endef

define assemble
$(CC) -c $(CFLAGS) -o $@ $<
endef
