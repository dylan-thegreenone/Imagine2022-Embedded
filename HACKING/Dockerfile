FROM docker.io/debian

# You're going to have to add your user to the group that the device's port uses. That's usually dialout or uucp
RUN apt-get -y update && apt-get -y install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
WORKDIR /root/
RUN mkdir -p esp && cd esp && git clone --recursive https://github.com/espressif/esp-idf.git -b v4.3.2
RUN cd esp-idf && ./install.sh esp32
RUN cat '. $HOME/esp/esp-idf/export.sh' >> .bashrc
