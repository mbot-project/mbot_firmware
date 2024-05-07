# mbot-firmware

## Table of Content

- [Description](#description)
- [Installation](#installation)
- [Code Structure](#code-structure)
- [MBot Classic Usage and Features](#mbot-classic-usage-and-features)
- [MBot OMNI Usage and Features](#mbot-omni-usage-and-features)
- [Maintainers](#maintainers)


## Description
Firmware designed for deployment on the Raspberry Pi Pico, tailored specifically for MBot Omni and MBot Classic. 

## Installation

1. After cloning the repository, run the setup script:

    ```bash
    $ cd ~/mbot_firmware
    $ ./setup.sh
    ```
    - which will install dependencies (requires sudo password) and initialize the submodules.

    If setup.sh is not executable by default, run the following to enable it:

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

## Code Structure
The students are required to make changes only in the `/src` directory and test the robot using scripts located in the `/python` directory or by adding their own scripts there. Everything else within the mbot_firmware is set to default and should not be directly modified by the students.

More details can be found under [mbot_documentation](https://github.com/mbot-project/mbot_documentation) repository.

## MBot Classic Usage and Features
> For MBot Classic use, you can also check detailed system setup guide [here](https://rob550-docs.github.io/docs/botlab/setup-guide/mbot-system-setup.html).

### Set MBot Specifications
1. Locate the `mbot_classic.h` file and find the versioning comment block. If you're using an official version and it matches your needs, leave everything unchanged. Otherwise, add a new version block using the provided syntax.
    ```c
    #if CLASSIC_VERSION == 10
    /****************** CLASSIC Version 1.0*******************/
    // Existing values for version 1.0
    #elif CLASSIC_VERSION == 20
    /****************** CLASSIC Version 2.0*******************/
    // Existing values for version 2.0
    ```
    To add a new version block, let's say version 3.0, copy and paste the existing section and adjust your own set of values.

2. Open `src/CMakeLists.txt` and look for the `CLASSIC_VERSION` variable at the top of the file. Set the `CLASSIC_VERSION` variable to the appropriate value (e.g., "2.0" if you are using a version 2.0).
3. Open `tests/mbot_calibrate_classic.c`. Locate the variables at the top of the file. Adjust them accordingly to match the version you have set.

### Calibrate the MBot and flash the firmware
Run the following command, the Pico will reboot automatically, and will then run its calibration routine right away. Allow the Pico to finish its calibration routine without interference.
```bash
$ sudo ./upload.sh flash build/tests/mbot_calibrate_classic.uf2
```
- During the calibration routine, robot should turning in counter clockwise circle first then turning clockwise. If it is not executing in this order, you might have wrong motor polarity. Modify it in the `tests/mbot_calibrate_classic.c` to be either 1 or -1. And then calibrate the mbot again.

The calibration script will have saved parameters onto the Pico’s memory. We can now flash the firmware that will run on the Pico during operation.
```bash
$ sudo ./upload.sh flash build/src/mbot_classic.uf2
```
### Test 
Use `python/mbot_move_simple.py` to send lcm velocity command to test.

## MBot OMNI Usage and Features
> For MBot OMNI use, you can also check detailed setup guide [here](https://hellorob.org/mbot/).

### Set MBot Specifications
1. Locate the `mbot_omni.h` file and find the versioning comment block. If you're using an official version and it matches your needs, leave everything unchanged. Otherwise, add a new version block using the provided syntax.
    ```c
    #if OMNI_VERSION == 10
    /****************** OMNI Version 1.0*******************/
    // Existing values for version 1.0
    #elif OMNI_VERSION == 20
    /****************** OMNI Version 2.0*******************/
    // Existing values for version 2.0
    ```
    To add a new version block, let's say version 3.0, copy and paste the existing section and adjust your own set of values.

2. Open `src/CMakeLists.txt` and look for the `OMNI_VERSION` variable at the top of the file. Set the `OMNI_VERSION` variable to the appropriate value (e.g., "2.0" if you are using a version 2.0).
3. Open `tests/mbot_calibrate_omni.c`. Locate the variables at the top of the file. Adjust them accordingly to match the version you have set.

### Calibrate the MBot and flash the firmware
Run the following command, the Pico will reboot automatically, and will then run its calibration routine right away. Allow the Pico to finish its calibration routine without interference.
```bash
$ sudo ./upload.sh flash build/tests/mbot_calibrate_omni.uf2
```
The calibration script will have saved parameters onto the Pico’s memory. We can now flash the firmware that will run on the Pico during operation.
```bash
$ sudo ./upload.sh flash build/src/mbot_omni.uf2
```
### Test 
Use `python/mbot_move_simple.py` to send lcm velocity command to test.

## Maintainers
- The current maintainers of this project are: Dr. Peter Gaskell, Shaw Sun. Please direct all questions regarding support, contributions, and issues to the maintainer. 