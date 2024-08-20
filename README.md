
SPDM-Hardware-Implementation 
================================================
Researchers: LARC-SEMBEI-Escola Politécnica da USP

# Risc-V GNU Toolchain [1]

All compilation will be done with this toolchain: 

```
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

# System on Chip - Hardware [2]

```
$ mkdir LiteX
$ cd LiteX
$ wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
$ chmod +x litex_setup.py
$ ./litex_setup.py --init --install --user --config=full
$ ./litex_setup.py --tag=2022.04
$ export PATH=$PATH:/opt/riscv/bin
$ source /tools/Xilinx/Vivado/2022.2/settings64.sh
$ litex-boards/litex_boards/targets/digilent_nexys4ddr.py --build --cpu-type rocket --cpu-variant linux --sys-clk-freq 50e6 --with-ethernet --with-sdcard
```

The Binary will be located at build/digilent_nexys4ddr/gateware/digilent_nexys4ddr.bit

The Pre Compiled Binary is at this repository: System_on_Chip/Binaries/digilent_nexys4ddr.bit

#System on Chip - Software [3]

## Kernel Image and rootfs.cpio

First of all, configure the busybox and compile for riscv:

```
curl https://busybox.net/downloads/busybox-1.33.2.tar.bz2 | tar xfj -
cp conf/busybox-1.33.2-rv64gc.config busybox-1.33.2/.config
(cd busybox-1.33.2; make CROSS_COMPILE=PATH/TO/riscv64-unknown-linux-gnu-)
```

With the busybox compiled, build the initramfs.cpio. In the busybox-1.33.2 directory: 

```
mkdir initramfs
pushd initramfs
mkdir -p bin sbin lib etc dev home proc sys tmp mnt nfs root \
          usr/bin usr/sbin usr/lib
cp ../busybox-1.33.2/busybox bin/
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

The Pre Compiled Binaries are in the System_on_Chip/Binaries/Image and System_on_Chip/Binaries/rootfs.cpio

## Bootloader - OpenSBI [4]

Using OpenSBI 0.8

```
$ git clone https://github.com/litex-hub/opensbi
$ cd opensbi
$ git checkout 84c6dc17f7d41c5c02760a5533d7268b57369837
$ export PATH=$PATH:/opt/riscv/bin
$ make CROSS_COMPILE=riscv64-unknown-linux-gnu- PLATFORM=generic \
    FW_FDT_PATH=PATH/TO/nexys4ddr.dtb FW_JUMP_FDT_ADDR=0x82400000
```
The binary of OpenSBI are in /opensbi/build/platform/generic/firmware/fw_jump.bin

The Pre Compiled Binaries are in the System_on_Chip/Binaries/fw_jump.bin

## Running

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

Execute the digilent_nexys4ddr.bit in the FPGA.

#LibSPDM [5]

Get LibSPDM library from the official repository.

```
$ git clone https://github.com/DMTF/libspdm.git
$ cd libspdm
$ git switch release-2.3
$ git submodule update --init --recursive
```

##LibSPDM - LiteX

Before compiling LibSPDM for LiteX, certain files must be modified. Replace the corresponding files with those in this repository, the directories in this repository have the same names, just replace them.
In summary they are: config.h, crt_wrapper_host.c and CMakeLists.txt. All of them in this repository libspdm/libspdm_LiteX
Once this is done, follow the instructions below in libspdm directory:

```
$ mkdir build
$ cd build
$ export PATH=$PATH:/opt/riscv/bin
$ cmake -DARCH=riscv64 -DTOOLCHAIN=RISCV_GNU -DTARGET=Release -DCRYPTO=mbedtls
$ make copy_sample_key
$ make
```

##LibSPDM - Kernel Linux
Before compiling LibSPDM for Kernel, certain files must be modified. Replace the corresponding files with those in this repository, the directories in this repository have the same names, just replace them.
In summary they are: config.h, crt_wrapper_host.c, bignum.c and CMakeLists.txt. All of them in this repository libspdm/libspdm_Kernel.
Once this is done, follow the instructions below in libspdm directory:

```
$ mkdir build_buildroot
$ cd build_buildroot
$ export PATH=$PATH:/opt/riscv/bin
$ export PATH="/PATH/TO/buildroot-2023.05.1/output/host/bin:$PATH"
$ cmake -DARCH=riscv64 -DTOOLCHAIN=RISCV_GNU -DTARGET=Release -DCRYPTO=mbedtls
$ make copy_sample_key
$ make
```

#Libspdm in LiteX BIOS
Before compiling LibSPDM within Litex, you must run the Makefile litex_libspdm.mk, paying attention to the correct directories when compiling.

```
$ make -f libspdm_litex.mk
```
Replace the corresponding files in this directory in Litex and add the libraries needed for compilation. All the files are in this repository under the path "litex".
After that, execute again:

```
$ litex-boards/litex_boards/targets/digilent_nexys4ddr.py --build --cpu-type rocket --cpu-variant linux --sys-clk-freq 50e6 --with-ethernet --with-sdcard
```
#Libspdm in Linux
Replace the corresponding files in this directory in buildroot-2023.05.1/output/build/linux-6.1.26 and add the libraries needed for compilation. All the files are in this repository under the path "System_on_Chip/Software".
After that:

```
$ make linux-rebuild
```



# References
[1] https://github.com/riscv/riscv-gnu-toolchain
[2] https://github.com/enjoy-digital/litex
[3] https://github.com/litex-hub/linux-on-litex-rocket
[4] https://github.com/riscv-software-src/opensbi
[5] https://github.com/DMTF/libspdm.git
