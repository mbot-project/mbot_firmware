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
Firmware designed for deployment on the Raspberry Pi Pico & [Robotics Control Board](https://mbot-dev.engin.umich.edu/mbot-robotics-control-board/), tailored specifically for MBot Omni and MBot Classic.

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
    cmake [-DMBOT_TYPE=<TYPE> -DENC=<ENC_RES> -DWHEEL_DIAMETER=<DIA_MM> -DGEAR_RATIO=<RATIO>] ..
    make
    ```
    Optional CMake flags:
    * `MBOT_TYPE`: `OMNI`, `CLASSIC`, or `BALBOT`
    * `ENC`: Encoder resolution
    * `WHEEL_DIAMETER`: Wheel diameter (in mm)
    * `GEAR_RATIO`: Motor gear ratio
    
**If you don’t** provide any values, all executables will be built using the default settings in `mbot_firmware/include/config`. All you need to do is:
```bash
cmake ..
make
```

**If you do** want to use custom values, you can override the defaults by passing them to cmake. For example, if you have MBot OMNI, with encoder resolution 50, wheel diameter 100 mm, gear ratio 78:
```bash
cmake -DMBOT_TYPE=OMNI -DENC=50 -DWHEEL_DIAMETER=100 -DGEAR_RATIO=78 ..
make
```

**⚠️ Important: Always Check CMake Output!!!**

CMake might keep old settings in its cache. Double-check the output to make sure your new values were applied correctly.
```bash
# Previously run:
cmake -DMBOT_TYPE=OMNI -DENC=50 -DWHEEL_DIAMETER=100 -DGEAR_RATIO=78 ..

# Then now run this:
cmake -DMBOT_TYPE=CLASSIC -DENC=50 ..

# Output:
Building code for MBot type CLASSIC
-- Using encoder resolution: 50
-- Using wheel diameter: 100      <--- ⚠️ Still using old value!
-- Using gear ratio: 78           <--- ⚠️ Still using old value!
```
If this happens, clear the cache by deleting the build folder:
```bash
# remove the entire build folder
rm -rf build
```
Then repeat the build steps from above.

## Code Structure
The students are required to make changes only in the `/src` directory and test the robot using scripts located in the `/python` directory or by adding their own scripts there. Everything else within the mbot_firmware is set to default and should not be directly modified by the students.

## MBot Classic Usage and Features
> For MBot Classic use, you can also check detailed system setup guide [here](https://rob550-docs.github.io/docs/botlab/setup-guide/mbot-system-setup.html).

### Set Custom MBot Specifications

If you're using motors and/or wheels that differ from those included in the standard MBot kit, you can update the relevant parameters in `include/config/mbot_classic_config.h`:
- `#define GEAR_RATIO` – Set the gear ratio of your motor
- `#define ENCODER_RES` – Set the encoder resolution (counts per revolution)
- `#define DIFF_WHEEL_DIAMETER` – Set the diameter of the wheel (in meters)
- `#define DIFF_WHEEL_RADIUS` – Set the radius of the wheel (in meters)


If you're using an the standard MBot Classic kit then leave everything unchanged. 


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

### Set Custom MBot Specifications

If you're using motors and/or wheels that differ from those included in the standard MBot kit, you can update the relevant parameters in `include/config/mbot_omni_config.h`:
- `#define GEAR_RATIO` – Set the gear ratio of your motor
- `#define ENCODER_RES` – Set the encoder resolution (counts per revolution)
- `#define OMNI_WHEEL_RADIUS` – Set the radius of the omni wheel (in meters)

If you're using an the standard MBot Omni kit then leave everything unchanged. 

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
