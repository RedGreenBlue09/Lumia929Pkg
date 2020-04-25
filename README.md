# Lumia929Pkg
WIP Custom ARM UEFI firmware for Lumia Icon.
Based on [Lumia930Pkg](https://github.com/rickliu2000/Lumia930Pkg) by @rickliu2000
## Build
#### Export needed variables
export WORKSPACE=/Path/To/EDK2  
export EDK_TOOLS_PATH=/Path/To/EDK2/BaseTools  
export GCC5_ARM_PREFIX=/Path/To/GCC-5/bin/arm-linux-gnueabi-
#### Setup Environment
. edksetup.sh
#### Build EDK2
build -a ARM -p Lumia929Pkg/Lumia929.dsc -t GCC5 -j6 -s -n 0
#### Export to ELF & emmc_appsboot.mbn
$GCC5_ARM_PREFIX\objcopy -I binary -O elf32-littlearm --binary-architecture arm Build/Lumia929-ARM/DEBUG_GCC5/FV/MSM8974_EFI.fd MSM8974_EFI.fd.elf  
$GCC5_ARM_PREFIX\ld MSM8974_EFI.fd.elf -T Lumia929Pkg/FvWrapper.ld -o emmc_appsboot.mbn
