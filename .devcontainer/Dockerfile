FROM docker.io/ubuntu:24.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends git wget unzip flex bison gperf && \
    apt-get install -y --no-install-recommends python3 python3-pip python3-venv && \
    apt-get install -y --no-install-recommends cmake ccache ninja-build libffi-dev libssl-dev libusb-1.0 dfu-util && \
    apt-get clean && rm -rf /var/lib/apt/lists/*


ARG IDF_VER="v5.3.1"
ARG IDF_SRC="https://github.com/espressif/esp-idf/releases/download/${IDF_VER}/esp-idf-${IDF_VER}.zip"
ARG IDF_DST="/usr/src/esp-idf"

WORKDIR ${IDF_DST}

ADD ${IDF_SRC} .

RUN unzip esp-idf-${IDF_VER}.zip && rm esp-idf-${IDF_VER}.zip && \
    mv esp-idf-${IDF_VER}/* . && rm -rf esp-idf-${IDF_VER}

RUN /usr/bin/python3 ./tools/idf_tools.py install required esp-clang --targets esp32 && \
    /usr/bin/python3 ./tools/idf_tools.py install-python-env --features core

RUN /usr/bin/python3 ./tools/idf_tools.py export >> /etc/profile.d/esp-idf-export.sh

ENV IDF_PATH=${IDF_DST}
ENV IDF_TARGET="esp32"
ENV IDF_TOOLCHAIN="clang"
