echo "[Setup] Set up permission..."
sudo chown -R $USER:$USER .git
sudo chmod -R 775 .git
echo "[Setup] Updating submodules..."
cd lib && git submodule update --init
cd pico-sdk && git submodule update --init
cd ../../

# set PICO_SDK_PATH
export PICO_SDK_PATH=$PWD/lib/pico-sdk
echo "PICO_SDK_PATH set to $PICO_SDK_PATH"