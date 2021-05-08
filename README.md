# Lumia929Pkg
WIP Custom ARM UEFI firmware for Lumia Icon.  
Based on [Lumia930Pkg](https://github.com/rickliu2000/Lumia930Pkg) by @rickliu2000  
Thanks to @konradybcio for tutorial and answer me questions.
## What's not work
- Press ESC (Camera Button) to go to Setup Menu
- Linux boots to green screen and slowly turns to white
- Lot more
## How to Compile
#### Download EDK2 & GCC-5
```
git clone --recurse-submodules https://github.com/tianocore/edk2 /Path/To/EDK2/  
cd /Path/To/EDK2  
git clone https://github.com/RedGreenBlue09/Lumia929Pkg Lumia929Pkg/
```
Download GCC-5 from here: [Linaro GCC-5 Toolchain](https://releases.linaro.org/components/toolchain/binaries/latest-5/arm-linux-gnueabihf/)  
And extract to /Path/To/GCC-5/
#### Install dependencies
```
sudo apt-get install build-essential uuid-dev iasl git gcc-5 nasm python3-distutils
make -C BaseTools
```
#### Export needed variables
```
export WORKSPACE=/Path/To/EDK2  
export EDK_TOOLS_PATH=/Path/To/EDK2/BaseTools  
export GCC5_ARM_PREFIX=/Path/To/GCC-5/bin/arm-linux-gnueabi-
. edksetup.sh
```
#### Build EDK2
```
build -a ARM -p Lumia929Pkg/Lumia929.dsc -t GCC5 -j<NumberOfThreads> -s -n 0
```
#### Export to ELF & emmc_appsboot.mbn
```
$GCC5_ARM_PREFIX\objcopy -I binary -O elf32-littlearm --binary-architecture arm Build/Lumia929-ARM/DEBUG_GCC5/FV/MSM8974_EFI.fd MSM8974_EFI.fd.elf  
$GCC5_ARM_PREFIX\ld MSM8974_EFI.fd.elf -T Lumia929Pkg/FvWrapper.ld -o emmc_appsboot.mbn
```
## How to Install
1. Compile Lumia929Pkg & Copy emmc_appsboot.mbn to EFIESP
2. Download compiled BootShim from here: [BootShim](https://drive.google.com/open?id=1kURXQ55zKRdksJDg8XCeBpt89x2tCczX)
3. Extract, copy BootShim.efi and ShimStage2.efi to EFIESP & Rename ShimStage2 to Stage2
4. Go to MainOS:\EFIESP\Windows\System32\Boot
5. Rename resetphone.efi to something
6. Copy BootShim.efi to this folder and rename to resetphone.efi
7. Add "NoIntegrityChecks" to "Reset Phone Application" entry in BCD of your phone
8. Enable DisplayBootMenu in BCD
9. Reboot Phone
10. At the BootMenu press vol down to boot EDK2
##### Credit: @imbushuo for creating [BootShim](https://github.com/imbushuo/boot-shim).
## How to use
1. Create startup.nsh inside the root of EFIESP and write "exit" inside
2. Boot EDK2 and let it boot to Shell without press anything, it will boot to Setup.
* To use Setup:
```
  * Volume Up     = Arrow UP
  * Volume Down   = Arrow Down
  * Power Button  = Enter
  * Camera Button = ESC
```
* You can boot EFI files by:  
  Boot Maintenance Manager ---> Boot From File ---> Partition ---> EFI File
