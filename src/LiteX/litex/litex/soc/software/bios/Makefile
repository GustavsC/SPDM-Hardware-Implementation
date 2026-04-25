include ../include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

SPDM_DIR?= /PATH/TO/libspdm
SPDM_FINAL?= $(SPDM_DIR)/build/lib
SPDM_CRYPTO?= mbedtls

LIBRARIES = cryptlib_$(SPDM_CRYPTO) $(SPDM_CRYPTO) cmockalib rnglib_null memlib malloclib debuglib spdm_crypt_lib spdm_secured_message_lib spdm_requester_lib spdm_common_lib spdm_transport_mctp_lib spdm_transport_pcidoe_lib spdm_device_secret_lib_null spdm_responder_lib mbedcrypto platform_lib spdm_crypt_ext_lib spdm_transport_test_lib mbedx509 debuglib_null 

# Permit TFTP_SERVER_PORT override from shell environment / command line
ifdef TFTP_SERVER_PORT
CFLAGS += -DTFTP_SERVER_PORT=$(TFTP_SERVER_PORT)
endif

OBJECTS = boot-helper.o	\
	  bswapsi2.o \
	  boot.o			\
	  helpers.o			\
	  cmd_bios.o		\
	  cmd_mem.o			\
	  cmd_boot.o		\
	  cmd_i2c.o			\
	  cmd_spiflash.o	\
	  cmd_litedram.o	\
	  cmd_liteeth.o		\
	  cmd_litesdcard.o  \
	  cmd_litesata.o    \
	  sim_debug.o		\
	  spdmfuncs.o 		\
	  main.o

ifneq "$(or $(BIOS_CONSOLE_NO_AUTOCOMPLETE),$(BIOS_CONSOLE_LITE))" ""
CFLAGS += -DBIOS_CONSOLE_NO_AUTOCOMPLETE
else
OBJECTS += complete.o
endif

ifdef BIOS_CONSOLE_NO_HISTORY
CFLAGS += -DBIOS_CONSOLE_NO_HISTORY
endif

ifdef BIOS_CONSOLE_DISABLE
CFLAGS += -DBIOS_CONSOLE_DISABLE
endif

ifdef BIOS_CONSOLE_LITE
CFLAGS += -DBIOS_CONSOLE_LITE
OBJECTS += readline_simple.o
else
OBJECTS += readline.o
endif

ifeq ($(CPU), zynq7000)
LSCRIPT = linker-zynq.ld
else ifeq ($(CPU), zynqmp)
LSCRIPT = linker-zynqmp.ld
else
LSCRIPT = linker.ld
endif

all: bios.bin
	$(PYTHON) -m litex.soc.software.memusage bios.elf $(CURDIR)/../include/generated/regions.ld $(TRIPLE)

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
ifneq ($(OS),Windows_NT)
	chmod -x $@
endif
ifeq ($(CPUENDIANNESS),little)
	$(PYTHON) -m litex.soc.software.crcfbigen $@ --little
else
	$(PYTHON) -m litex.soc.software.crcfbigen $@
endif

bios.elf: $(BIOS_DIRECTORY)/$(LSCRIPT) $(OBJECTS)

vpath %.a $(PACKAGES:%=../%)

%.elf: crt0.o $(LIBS:%=%.a)
	$(CC) $(LDFLAGS) -T $(BIOS_DIRECTORY)/$(LSCRIPT) -N -o $@ \
		crt0.o \
		$(OBJECTS) \
		$(PACKAGES:%=-L../%) \
		-Wl,--whole-archive \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%) -L$(SPDM_FINAL) $(addprefix -l, $(LIBRARIES)) 

ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

VPATH = $(BIOS_DIRECTORY):$(BIOS_DIRECTORY)/cmds:$(CPU_DIRECTORY)

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) bios.elf bios.bin .*~ *~

.PHONY: all clean
