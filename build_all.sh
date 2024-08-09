#!/bin/bash
set -e  # Quit on error.

if [ ! -d "build" ]; then
    mkdir build
fi

rm -rf build/*  # Clear contents from old builds.
cd build

# CLASSIC with each encoder resolution (20, 40, 48).
echo "*******************************"
echo "*  BUILDING CLASSIC VERSIONS  *"
echo "*******************************"
echo
cmake -DMBOT_TYPE=CLASSIC -DENC=20 .. && make
cmake -DMBOT_TYPE=CLASSIC -DENC=40 .. && make
cmake -DMBOT_TYPE=CLASSIC -DENC=48 .. && make

# OMNI, 101mm wheels (old) with each encoder resolution (20, 48).
echo
echo "*******************************************"
echo "*  BUILDING OMNI VERSIONS (101mm wheels)  *"
echo "*******************************************"
echo
cmake -DMBOT_TYPE=OMNI -DOMNI_WHEEL_DIAMETER=101 -DENC=20 .. && make
cmake -DMBOT_TYPE=OMNI -DOMNI_WHEEL_DIAMETER=101 -DENC=48 .. && make

# OMNI, 96mm wheels (new) with each encoder resolution (20, 48).
echo
echo "*******************************************"
echo "*  BUILDING OMNI VERSIONS (96mm wheels)   *"
echo "*******************************************"
echo
cmake -DMBOT_TYPE=OMNI -DOMNI_WHEEL_DIAMETER=96 -DENC=20 .. && make
cmake -DMBOT_TYPE=OMNI -DOMNI_WHEEL_DIAMETER=96 -DENC=48 .. && make

echo
echo "Building all versions complete!"
