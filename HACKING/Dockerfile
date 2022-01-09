FROM docker.io/debian

# You're going to have to add your user to the group that the device's port uses. That's usually dialout or uucp
RUN apt-get -y update && apt-get -y install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
RUN mkdir -p ~/esp && cd ~/esp && git clone --recursive https://github.com/espressif/esp-idf.git && \
cd ~/esp/esp-idf && ./install.sh esp32
#cat '. $HOME/esp/esp-idf/export.sh' >> ~/.bashrc

# Run with:
# podman run -dit --device=/dev/ttyUSB0:/dev/ttyUSB0:rwm --group-add keep-groups --annotation io.crun.keep_original_groups=1  14f167d6ad5e

# Once you're inside, use these commands to run the demo:
# . $HOME/esp/esp-idf/export.sh
# cd ~/esp/esp-idf/examples/bluetooth/bluedroid/classic_bt/bt_discovery
# idf.py -p /dev/ttyUSB0 flash monitor
