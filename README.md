# mbot-firmware

## Table of Content

- [Description](#description)
- [Installation](#installation)
- [Code Structure](#code-structure)
- [MBot Classic Usage and Features](#mbot-classic-usage-and-features)
- [MBot OMNI Usage and Features](#mbot-omni-usage-and-features)
- [Generating New Releases](#generating-new-releases)
- [Maintainers](#maintainers)


## Description
Firmware designed for deployment on the Raspberry Pi Pico, tailored specifically for MBot Omni and MBot Classic.

## Installation
1. After cloning the repository, run the setup script:

    ```bash
    cd ~/mbot_firmware
    ./setup.sh
    ```
    - which will install dependencies (requires sudo password) and initialize the submodules.

    If setup.sh is not executable by default, run the following to enable it:

    ```bash
    sudo chmod +x setup.sh
    ```
2. Build as follows:
    ```bash
    mkdir build
    cd build
    cmake [-DMBOT_TYPE=<MBOT-TYPE> -DENC=<ENC-RES> -DOMNI_WHEEL_DIAMETER=<WHEEL-DIA>] ..
    make
    ```


### Specifying the MBot type

Various parameters can be configured.

* `MBOT_TYPE`: One of `OMNI` or `CLASSIC`. Only the firmware for the given robot type will be generated.
* `ENC`: The encoder resolution. This must be one of the supported resolutions: `20`, `40`, or `48`. For type `OMNI`, encoder resolution `40` is not supported.
* `OMNI_WHEEL_DIAMETER`: The wheel diameter for the `OMNI` robot type, in millimeters. This must be one of the supported values: `96` or `101`. This option is ignored for type `CLASSIC`.

## Code Structure
The students are required to make changes only in the `/src` directory and test the robot using scripts located in the `/python` directory or by adding their own scripts there. Everything else within the mbot_firmware is set to default and should not be directly modified by the students.

More details can be found under [mbot_documentation](https://github.com/mbot-project/mbot_documentation) repository.

## MBot Classic Usage and Features
> For MBot Classic use, you can also check detailed system setup guide [here](https://rob550-docs.github.io/docs/botlab/setup-guide/mbot-system-setup.html).

### Set Custom MBot Specifications

Locate the file `include/config/mbot_classic_config.h`. If you're using an official version and it matches your needs, leave everything unchanged. Otherwise, edit the values to suit your needs.

### Calibrate the MBot and flash the firmware
Run the following command, the Pico will reboot automatically, and will then run its calibration routine right away. Allow the Pico to finish its calibration routine without interference.
```bash
sudo mbot-upload-firmware flash build/mbot_calibrate_classic.uf2
```
- During the calibration routine, robot should turning in counter clockwise circle first then turning clockwise. If it is not executing in this order, you might have wrong motor polarity. Modify it in the `tests/mbot_calibrate_classic.c` to be either 1 or -1. And then calibrate the mbot again.

The calibration script will have saved parameters onto the Pico’s memory. We can now flash the firmware that will run on the Pico during operation.
```bash
sudo mbot-upload-firmware flash build/mbot_classic.uf2
```
### Test
Use `python/mbot_move_simple.py` to send lcm velocity command to test.

## MBot OMNI Usage and Features
> For MBot OMNI use, you can also check detailed setup guide [here](https://hellorob.org/mbot/).

### Set Custom MBot Specifications

Locate the file `include/config/mbot_omni_config.h`. If you're using an official version and it matches your needs, leave everything unchanged. Otherwise, edit the values to suit your needs.

### Calibrate the MBot and flash the firmware
Run the following command, the Pico will reboot automatically, and will then run its calibration routine right away. Allow the Pico to finish its calibration routine without interference.
```bash
sudo mbot-upload-firmware flash build/mbot_calibrate_omni.uf2
```
The calibration script will have saved parameters onto the Pico’s memory. We can now flash the firmware that will run on the Pico during operation.
```bash
sudo mbot-upload-firmware flash build/mbot_omni.uf2
```
### Test
Use `python/mbot_move_simple.py` to send lcm velocity command to test.

## Generating New Releases

To create new releases, follow these steps:
1. Bump the version variable, `CMAKE_PROJECT_VERSION`, in `CMakeLists.txt` and commit it to GitHub.
2. In the root of the repository, run the script to generate all the compiled firmware files (this will take some time):
   ```bash
   ./build_all.sh
   ```
3. When the script finishes, download all the files in the folder `build/release-vX.Y.Z`
4. Create a new release on GitHub, with the title *and* tag "vX.Y.Z". Select "Create new tag on publish".
5. Upload all the files from `build/release-vX.Y.Z` under the "Attach binaries" section of the release.
6. Write a description of the changes for the new release and publish.

## Maintainers
- The current maintainers of this project are: Dr. Peter Gaskell, Shaw Sun. Please direct all questions regarding support, contributions, and issues to the maintainer.
