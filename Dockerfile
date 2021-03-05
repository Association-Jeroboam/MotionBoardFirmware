FROM frolvlad/alpine-glibc:latest

WORKDIR /home/dev

RUN apk add --no-cache git make && \
    apk --update --no-cache add --virtual build-dependencies \
        bzip2-dev \
        ca-certificates \
        openssl \
        tar \
        w3m && \     
    GCCARM_LINK="$(w3m -o display_link_number=1 -dump 'https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads' | \
        grep -m1 '^\[[0-9]\+\].*downloads.*gcc-arm-none-eabi.*linux\.tar\.bz2' | \
        sed -e 's/^\[[0-9]\+\] //')" && \
    wget -O /tmp/gcc-arm-none-eabi.tar.bz2 ${GCCARM_LINK} && \
    tar xvf /tmp/gcc-arm-none-eabi.tar.bz2 --strip-components=1 -C /usr/local && \
    rm -rf /tmp/gcc-arm-none-eabi.tar.bz2 && \
    rm -rf /usr/local/share/doc && \
    apk del build-dependencies*
