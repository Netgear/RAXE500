# Open Source release for Netgear RAX-E500


## Prequisites required on build machine

* correct cross-toolchain must be installed in /opt

    mkdir -p /opt/toolchains \
    && cd /opt/toolchains \
    && git clone --branch aarch64-gcc-9.2-linux-4.19-glibc-2.30-binutils-2.32 git@github.com:Netgear/cross-toolchain.git 


* package dependencies

    for package in build-essential gawk autoconf automake autotools-dev libtool liblzo2-dev
    do
       sudo apt install ${package}
    done


