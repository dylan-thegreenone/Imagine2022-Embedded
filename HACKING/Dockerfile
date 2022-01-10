FROM docker.io/debian

# You're going to have to add your user to the group that the device's port uses. That's usually dialout or uucp
RUN apt-get -y update && apt-get -y install flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 unzip
RUN apt-get -y install vim git wget
WORKDIR /root/
RUN mkdir -p esp && cd esp && wget https://github.com/espressif/esp-idf/archive/refs/tags/v4.3.2.zip -O esp-idf.zip && unzip esp-idf.zip
RUN mv esp/esp-idf-4.3.2 esp/esp-idf && cd esp/esp-idf && ./install.sh esp32
RUN echo '. /root/esp/esp-idf/export.sh' >> .bashrc
