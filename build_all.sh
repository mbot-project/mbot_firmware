#!/bin/bash
set -e  # Quit on error.

# Supported configurations.
CLASSIC_ENCODER_RES="20 40 48"
OMNI_ENCODER_RES="20 48"
OMNI_WHEEL_DIAMETER="96 101"

if [ ! -d "build" ]; then
    mkdir build
fi

rm -rf build/*  # Clear contents from old builds.
cd build

# CLASSIC with each encoder resolution.
echo "*******************************"
echo "*  BUILDING CLASSIC VERSIONS  *"
echo "*******************************"
echo

for enc in $CLASSIC_ENCODER_RES
do
    cmake -DMBOT_TYPE=CLASSIC -DENC=$enc .. && make
done

# OMNI, all combinations of wheel diameters and encoder resolutions..
echo
echo "****************************"
echo "*  BUILDING OMNI VERSIONS  *"
echo "****************************"
echo

for wheel_dia in $OMNI_WHEEL_DIAMETER
do
    for enc in $OMNI_ENCODER_RES
    do
        cmake -DMBOT_TYPE=OMNI -DOMNI_WHEEL_DIAMETER=$wheel_dia -DENC=$enc .. && make
    done
done

echo
echo "Building all versions complete! Copying release files..."
echo

# Copy all the files for the release to a separate folder.

# Extract the CMAKE_PROJECT_VERSION from the CMakeCache.txt file
CMAKE_PROJECT_VERSION=$(grep -m 1 "CMAKE_PROJECT_VERSION:" "CMakeCache.txt" | sed 's/.*=//')

# Check if the version was found
if [ -z "$CMAKE_PROJECT_VERSION" ]; then
  echo "CMAKE_PROJECT_VERSION not found in $CMAKECACHE_PATH."
  exit 1
fi

# The files for this release will go here.
RELEASE_DIR=release-v$CMAKE_PROJECT_VERSION/
# If there was an old release directory, delete it.
if [ -d $RELEASE_DIR ]; then
    rm -rf $RELEASE_DIR
fi
mkdir $RELEASE_DIR

# Classic files.
for enc in $CLASSIC_ENCODER_RES
do
    cp mbot_classic_v${CMAKE_PROJECT_VERSION}_enc${enc}.uf2 $RELEASE_DIR
    cp mbot_calibrate_classic_v${CMAKE_PROJECT_VERSION}_enc${enc}.uf2 $RELEASE_DIR
done

# Omni files.
for wheel_dia in $OMNI_WHEEL_DIAMETER
do
    for enc in $OMNI_ENCODER_RES
    do
        cp mbot_omni_v${CMAKE_PROJECT_VERSION}_enc${enc}_w${wheel_dia}mm.uf2 $RELEASE_DIR
        cp mbot_calibrate_omni_v${CMAKE_PROJECT_VERSION}_enc${enc}_w${wheel_dia}mm.uf2 $RELEASE_DIR
    done
done

# Copy the tests.
cp mbot_classic_motor_test.uf2 $RELEASE_DIR
cp mbot_omni_motor_test.uf2 $RELEASE_DIR
cp mbot_encoder_test.uf2 $RELEASE_DIR

echo "Done preparing release! The files for the release are in directory:"
echo
echo "build/$RELEASE_DIR"
for item in "$RELEASE_DIR"/*; do
  echo -e "\t$(basename "$item")"
done
