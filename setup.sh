echo "[Setup] Apt Installing required packages..."
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential -y
echo "[Setup] Set up permission..."
sudo chown -R $USER:$USER .git
sudo chmod -R 775 .git
echo "[Setup] Updating submodules..."
cd lib && git submodule update --init
cd pico-sdk && git submodule update --init
# git checkout master
git pull
cd ../../
export PICO_SDK_PATH=$PWD/lib/pico-sdk
echo "[Setup] Done!"
