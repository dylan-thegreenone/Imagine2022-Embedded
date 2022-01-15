FROM docker.io/debian

# You're going to have to add your user to the group that the device's port uses. That's usually dialout or uucp
RUN apt-get -y update && \
    apt-get -y install flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 unzip
RUN apt-get -y install vim git # This is extra stuff I want to put in a separate layer
WORKDIR /root/esp
RUN git clone --recursive https://github.com/espressif/esp-idf.git
WORKDIR esp-idf
RUN ./install.sh esp32
RUN echo '. /root/esp/esp-idf/export.sh' >> /root/.bashrc
