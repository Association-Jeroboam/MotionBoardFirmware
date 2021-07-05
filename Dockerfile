FROM frolvlad/alpine-glibc:latest

WORKDIR /home/dev

RUN apk add --no-cache git make && \
    apk --update --no-cache add --virtual build-dependencies \
        bzip2-dev \
        ca-certificates \
        openssl \
        tar \
        w3m && \     
    GCCARM_LINK="https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2\?revision\=2cc92fb5-3e0e-402d-9197-bdfc8224d8a5\?product\=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,6-2017-q2-update" && \
    wget -O /tmp/gcc-arm-none-eabi.tar.bz2 ${GCCARM_LINK} && \
    tar xvf /tmp/gcc-arm-none-eabi.tar.bz2 --strip-components=1 -C /usr/local && \
    rm -rf /tmp/gcc-arm-none-eabi.tar.bz2 && \
    rm -rf /usr/local/share/doc && \
    apk del build-dependencies*
