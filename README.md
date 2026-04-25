
SPDM Hardware Implementation 
================================================
Researchers: LARC-SEMBEI-Escola Politécnica da USP

This repository contains an implementation project of the Security Protocol and Data Model (SPDM) in hardware. The hardware can execute a BIOS as a Requester and an Ethernet card as a Responder, performed in 9 phases: GET_VERSION, GET_CAPABILITIES, NEGOTIATE_ALGORITHMS, GET_DIGESTS, GET_CERTIFICATE, CHALLENGE, GET_MEASUREMENTS, KEY_EXCHANGE and FINISH. [1].

<img width="475" height="733" alt="Screenshot 2026-04-12 at 19-16-49 Security Protocol and Data Model (SPDM) Specification - DSP0274_1 2 1 pdf" src="https://github.com/user-attachments/assets/ae5e8476-9b07-472e-b466-60d390ec5450" />

The SPDM-Hardware architecture uses SHA-256 as Hash Algorithm. RSA-2048 for Signatures and ChaCha20-Poly1305 as authenticated encryption with associated data (AEAD);

The project is executed in a NetFPGA SUME.

Vivado 2023 or above with Virtex-7 license is required.
The host machine need at least 16GB RAM.

# TCL Method
In the TCL Script method, a script recreates the hardware inside Vivado using Verilog source code. The software is already pre-compiled and incorporated into the binary during compilation.
To replicate the experiment follow:

1. Open Vivado Tcl Shell command prompt.
2. cd into the directory of SoC/SoC_with_spdm.
3. Run ***source digilent_netfpga_sume.tcl -notrace*** to generate a binary.

Building the project may take a very long time (~1.5 hours using 4 cores).

## Note:
A SoC version without SPDM is avaliable for comparative. If required:

1. Open Vivado Tcl Shell command prompt.
2. cd into the directory of SoC/SoC_no_spdm.
3. Run ***source digilent_netfpga_sume.tcl -notrace*** to generate a binary.

# Compile Method
All libraries and detailed toolchain. In the Compile Method, all source code is in "src".

## 1. RISC-V GNU Toolchain

All compilation will be done with this toolchain: 

```
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev
$ mkdir riscv
$ cd riscv
$ git clone https://github.com/riscv/riscv-gnu-toolchain
$ git checkout tags/2023.06.09
```

We'll need two libraries, Newlib and Linux, so the toolchain will be built twice:

For Newlib library:
```
$ ./configure --prefix=/opt/riscv --enable-multilib
$ make
```

For Linux library:
```
$ ./configure --prefix=/opt/riscv --enable-multilib
$ make linux
```

## 2. System on Chip - Hardware 

The SoC will be build with LiteX library. Install:

```
$ mkdir LiteX
$ cd LiteX
$ wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
$ chmod +x litex_setup.py
$ ./litex_setup.py --init --tag=2024.04 --install --user --config=full
```

### 2.1 LibSPDM 

Get the LibSPDM library from the official repository.

```
$ git clone https://github.com/DMTF/libspdm.git
$ cd libspdm
$ git switch release-2.3
$ git submodule update --init --recursive
```

Before compiling LibSPDM, the CMakeLists.txt file needs to be modified. Modify the corresponding file in this repository, which can be found in the libspdm/libspdm_LiteX directory. After changing CMakeLists.txt, apply the following patch when compiling for Requester. 

```
$ git apply libspdm_requester.patch
```

Once done, follow the instructions below in the LibSPDM directory:

```
$ mkdir build
$ cd build
$ export PATH=$PATH:/opt/riscv/bin
$ cmake .. -DARCH=riscv64 -DTOOLCHAIN=RISCV_GNU -DTARGET=Release -DCRYPTO=mbedtls
$ make copy_sample_key
$ make
```

### 2.2 LibSPDM in BIOS
Before compiling LibSPDM within Litex, you must run the Makefile litex_libspdm.mk. This Makefile will add LibSPDM files in the LiteX software directory. 
Pay attention to the correct directories when compiling.

```
$ make -f libspdm_litex.mk
```

Replace/Add the corresponding files in the LiteX BIOS directory: bswapsi2.c, int_endianness.h, int_lib.h, int_types.h, int_util.h, linker.ld, spdm_requester.c, spdm_requester.h, spdm_responder.c, spdm_responder.h, boot.c, Makefile, cmd_bios.c, bundle_responder.certchain.h and ca_root_responder.h. These files are in this repository under the path of  "LiteX/litex/litex/soc/software/bios".

Replace common.mak in "LiteX/litex/litex/soc/software".

The directory structure with the added and replaced files will look similar:

```
/LiteX/litex/litex/soc/software/common.mak
/LiteX/litex/litex/soc/software/bios/bswapsi2.c
/LiteX/litex/litex/soc/software/bios/int_endianness.h
/LiteX/litex/litex/soc/software/bios/int_lib.h
/LiteX/litex/litex/soc/software/bios/int_types.h
/LiteX/litex/litex/soc/software/bios/int_util.h
/LiteX/litex/litex/soc/software/bios/linker.ld
/LiteX/litex/litex/soc/software/bios/spdm_requester.c
/LiteX/litex/litex/soc/software/bios/spdm_requester.h
/LiteX/litex/litex/soc/software/bios/spdm_responder.c
/LiteX/litex/litex/soc/software/bios/spdm_responder.h
/LiteX/litex/litex/soc/software/bios/boot.c   
/LiteX/litex/litex/soc/software/bios/Makefile
/LiteX/litex/litex/soc/software/bios/cmds/cmd_bios.c
/LiteX/litex/litex/soc/software/bios/certificates/bundle_responder.certchain.h
/LiteX/litex/litex/soc/software/bios/certificates/ca_root_responder.h
```

***Pay attention to editing the correct directories when compiling, especially in the Makefiles (libspdm_litex.mk, Makefile, common.mak).***

### 2.3 LibSPDM in Ethernet Card

These files are for create an Ethernet card with SPDM registers in our FPGA.
Replace/add the following files in their respective directories:

```
/LiteX/liteeth/liteeth/phy/v7_1000basex.py
/LiteX/liteiclink/liteiclink/serdes/gth_7series.py
/LiteX/liteiclink/liteiclink/serdes/gth_7series_init.py
/LiteX/litex-boards/litex-boards/platforms/digilent_netfpga_sume.py
/LiteX/litex-boards/litex-boards/targets/digilent_netfpga_sume.py
/LiteX/litex/litex/soc/integration/soc_core.py
```
After all these changes, go back to the main directory and follow:

```
$ export PATH=$PATH:/opt/riscv/bin
$ source /tools/Xilinx/Vivado/2023.1/settings64.sh
$ litex-boards/litex_boards/targets/digilent_netfpga_sume.py --build --cpu-type rocket --cpu-variant linux --cpu-mem-width 8 --with-ethernet --bus-standard axi --no-compile-gateware
```

### 2.4 LibSPDM as Responder

LibSPDM in Responder requires Vitis 2023.1 and his tools. Import src/Microblaze/digilent_netfpga_sume.xsa as Hardware Specification in Vitis and the following files:

```
/Microblaze/certificates/
/Microblaze/measurementFunctions
/Microblaze/privateKey
/Microblaze/signatureFunctions
/Microblaze/requester.c
/Microblaze/lscript.ld
```

Apply libspdm\_responder.patch and add as part of the project. Set requester.c as main file and lscript.ld as linker. Compile spdm_requester.elf

Microblaze needs to be created using digilent_netfpga_sume.tcl.

# System on Chip - Software [3]

## Kernel Image and rootfs.cpio

To compile kernel, bootloader and file system.

First of all, configure the busybox and compile for riscv:

```
curl https://busybox.net/downloads/busybox-1.33.2.tar.bz2 | tar xfj -
cp conf/busybox-1.33.2-rv64gc.config busybox-1.33.2/.config
(cd busybox-1.33.2; make CROSS_COMPILE=PATH/TO/riscv64-unknown-linux-gnu-)
mkdir linux
cp busybox-1.33.2/linux/.config linux/.config
```

With the busybox compiled, build the initramfs.cpio. In the busybox-1.33.2 directory: 

```
mkdir initramfs
pushd initramfs
mkdir -p bin sbin lib etc dev home proc sys tmp mnt nfs root \
          usr/bin usr/sbin usr/lib
cp ../busybox bin/
ln -s bin/busybox ./init
cat > etc/inittab <<- "EOT"
::sysinit:/bin/busybox mount -t proc proc /proc
::sysinit:/bin/busybox mount -t devtmpfs devtmpfs /dev
::sysinit:/bin/busybox mount -t tmpfs tmpfs /tmp
::sysinit:/bin/busybox mount -t sysfs sysfs /sys
::sysinit:/bin/busybox --install -s
/dev/console::sysinit:-/bin/ash
EOT
fakeroot <<- "EOT"
find . | cpio -H newc -o > ../initramfs.cpio
EOT
popd
```


For Kernel Image and rootfs.cpio we will use buildroot:

```
$ mkdir buildroot_riscv64
$ wget https://git.busybox.net/buildroot/snapshot/buildroot-2023.05.1.tar.bz2
$ tar -xjf buildroot-2023.05.1.tar.bz2 
$ cd buildroot-2023.05.1 
```

Use the .configs of this repository in the respectives folders.

In buildroot-2023.05.01/.config, use the correct PATH in following configs:

```
BR2_DEFCONFIG="PATH/TO/buildroot_riscv64/buildroot-2023.05.1/configs/qemu_riscv64_virt_defconfig"
BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE="PATH/TO/busybox-1.33.2/linux/.config"
BR2_PACKAGE_BUSYBOX_CONFIG="PATH/TO/busybox-1.33.2/.config"
BR2_LINUX_KERNEL_CUSTOM_DTS_PATH="PATH/TO/nexys4ddr.dts"
```

In busybox-1.33.2/linux/.config, configure the PATH to initramfs.cpio 

```
CONFIG_INITRAMFS_SOURCE="PATH/TO/busybox-1.33.2/initramfs.cpio"
```

```
$ make
```

The binary of Kernel Image are in the /output/images/Image
The binary of rootfs.cpio are in the /output/images/rootfs.cpio

## Bootloader - OpenSBI [4]

The bootloader requires a Device Tree Blob (DTB) to determine the hardware organization in memory space that the operating system Kernel will manage. DTB is described by the data structure Device Tree Source (DTS). This repository contains the SoC Device Tree in Kernel/DeviceTree.

```
$ cd Kernel/DeviceTree
$ dtc -O dtb -o netfpgasume.dtb netfpgasume.dts
```

Using OpenSBI 0.8

```
$ git clone https://github.com/litex-hub/opensbi
$ cd opensbi
$ git checkout 84c6dc17f7d41c5c02760a5533d7268b57369837
$ export PATH=$PATH:/opt/riscv/bin
$ make CROSS_COMPILE=riscv64-unknown-linux-gnu- PLATFORM=generic \
    FW_FDT_PATH=PATH/TO/netfpgasume.dtb FW_JUMP_FDT_ADDR=0x82400000
```
The binary of OpenSBI are in /opensbi/build/platform/generic/firmware/fw_jump.bin

## Executing

Create a boot.json with the memory configuration:

```
{
	"rootfs.cpio":   "0x82000000",
	"Image":       "0x80200000",
	"fw_jump.bin": "0x80000000"
}
```

With the binaries (Image, rootfs.cpio, fw_jump.bin and boot.json) in the same folder, run:

```
litex_term /dev/ttyUSB1 --images=PATH/TO/boot.json
```

Execute the digilent_netfpga_sume.bit in the FPGA.

# References
[1] https://www.dmtf.org/sites/default/files/standards/documents/DSP0274_1.2.1.pdf

[2] https://digilent.com/reference/programmable-logic/netfpga-sume/start?srsltid=AfmBOorSQYE7kGKgUYJlLcDCyw6vfYHvrAbTzTf5HQJcgC4-E-jnjQHG

[3] https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/archive.html

[4] https://github.com/enjoy-digital/litex

[5]https://docs.amd.com/r/2023.1-English/ug893-vivado-ide/Using-the-Tcl-Console

[6] https://docs.amd.com/r/2023.1-English/ug1400-vitis-embedded/Installing-the-Vitis-Software-Platform

