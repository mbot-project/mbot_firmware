# mbot-firmware

## Table of Content

- [Description](#description)
- [Installation](#installation)
- [MBot Classic Usage and Features](#mbot-classic-usage-and-features)
- [MBot OMNI Usage and Features](#mbot-omni-usage-and-features)
- [Maintainers](#maintainers)


## Description
Firmware designed for deployment on the Raspberry Pi Pico, tailored specifically for controlling and managing the operations of two robotic platforms: MBot Omni and MBot Classic. 

## Installation

1. After cloning the repository, run the setup script:

    ```bash
    $ cd ~/mbot_firmware
    $ ./setup.sh
    ```
    - which will install dependencies (requires sudo password) and initialize the submodules.

    If setup.sh is not executable by default,run the following to enable it:

    ```bash
    $ sudo chmod +x setup.sh
    ```
2. Build as follows:
    ```bash
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    ```
## MBot Classic Usage and Features
> For ROB 550 mbot classic use, you can also check detailed system setup guide [here](https://rob550-docs.github.io/docs/botlab/setup-guide/mbot-system-setup.html#set-up-mbot-firmware).

### Calibrate the MBot and flash the firmware
Run the following command, the Pico will reboot automatically, and will then run its calibration routine right away. Allow the Pico to finish its calibration routine without interference.
```bash
$ sudo ./upload.sh flash build/tests/mbot_calibrate_classic.uf2
```
During the calibration routine, robot should turning in counter clockwise circle first then turning clockwise. If it is not executing in this order, you might have wrong motor polarity. Modify it in the `tests/mbot_calibrate_classic.c` to be either 1 or -1. And then calibrate the mbot again.

The calibration script will have saved parameters onto the Pico’s memory. We can now flash the firmware that will run on the Pico during operation.
```bash
$ sudo ./upload.sh flash build/src/mbot.uf2
```

## MBot OMNI Usage and Features
> The following content is legacy notes that may or may not apply to the current omni.
### Using GDB on the Pico

As long as the SWD wires are connected to the Raspberry Pi (see above section on main branch), you can use GDB on the Pico to help debug your current firmware or test program.

Note: make sure that the elf file was built using the `-DCMAKE_BUILD_TYPE=Debug` flag. If the file was built for Release, the optimization means most things won't be visible when you use the print command in gdb later.

Build as normal, then upload using:
```bash
upload_swd.sh path/to/elf/file.elf
```

In one window, run:
```bash
debug.sh path/to/elf/file.elf
```

Which starts up the GDB server and blocks this terminal until you Ctrl+C to quit the server. In another terminal, run:

```bash
debug_attach.sh path/to/elf/file.elf
```

Which attaches to the server and stops the program at the start of the main function, from where you can start debugging with GDB commands.

[TODO: get this working in VSCode]

### Installing picotool
NOTE: this is run in the setup.sh script
```bash
export PICO_SDK_PATH=$PWD/lib/pico-sdk
wget https://github.com/raspberrypi/picotool/archive/refs/tags/1.1.1.zip
unzip 1.1.1.zip
cd picotool-1.1.1
mkdir build && cd build
cmake ..
make
sudo make install
cd ../..
rm 1.1.1.zip
rm -r picotool-1.1.1
```

### Installing openocd and gdb-multiarch
Note: We should add this to the setub.sh script if it is run on a RPi
```bash
sudo apt-get install libftdi-dev gdb-multiarch
git clone https://github.com/raspberrypi/openocd.git --recursive --branch rp2040 --depth=1
cd openocd
./bootstrap
./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio
make -j4
sudo make install
```

### Uploading firmware to the pico
There are a few ways to upload firmware, using the bootload partition on the pico, using picotool or using the upload.sh script and openocd.

#### Bootloader
Plug in the pico while holding the BOOTLOAD button.  The pico will be mounted as a drive.  Copy the mbot.uf2 file over to the drive.

#### picotool
Plug in the pico while holding the BOOTLOAD button.  Run:
```bash
picotool load build/src/mbot.uf2
picotool reboot
```

#### upload_swd.sh and openocd
[TODO: Check if we can run on pin7 and pin11 instead]
Run the upload script which uses openocd.  This does not require puting the Pico into bootloader mode.  You must have the SWD wires (SWDIO and SWCLK) connected to GPIO 24 (Pin 18), GND (Pin 20) and GPIO 25 (Pin 22) on the Raspberry Pi.  Note, when using the upload script and openocd, you upload the .elf firmware file, not the .uf2 firmware file, they are just a different format, but the same firmware.
 ```bash
 upload.sh build/src/mbot.elf
 ```

## Maintainers
- The current maintainer of this project is TBD.
    - Please direct all questions regarding support, contributions, and issues to the maintainer. The maintainer is responsible for overseeing the project's development, reviewing and merging contributions, and ensuring the project's ongoing stability and relevance.