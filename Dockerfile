FROM ubuntu:22.04

# add software repositories for retrieving latest clang-format
# refresh the package index for latest software packages
# install wget and gnupg packages
# retrieve the archive signature for clang-format
# refresh the package index for latest software packages
# install necessary packages
# remove unnecessary packages
RUN DEBIAN_FRONTEND=noninteractive \
    echo "\ndeb http://apt.llvm.org/jammy/ llvm-toolchain-jammy main\
    \ndeb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy main\
    \n# 19\
    \ndeb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main\
    \ndeb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main\
    \n# 20\
    \ndeb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main\
    \ndeb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main" >> /etc/apt/sources.list &&\
    apt-get update &&\
    apt-get install -y wget gnupg &&\
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - &&\
    apt-get update &&\
    apt-get install -y clang-format cppcheck git make unzip &&\
    apt-get remove -y gnupg wget

# set the working directory
WORKDIR /home

# copy everything from project's root directory to docker image's working directory
COPY . .

# unzip STM32CubeCLT v1.18.0, install it, and finally clean up unnecessary files
RUN unzip ./tools/en.st-stm32cubeclt_1.18.0_24403_20250225_1636_amd64.sh.zip -d ./tools && \
    chmod +x ./tools/st-stm32cubeclt_1.18.0_24403_20250225_1636_amd64.sh && \
    echo | LICENSE_ALREADY_ACCEPTED=1 ./tools/st-stm32cubeclt_1.18.0_24403_20250225_1636_amd64.sh && \
    rm -rf tools/

# run a script to add tools' directories to PATH environment variable on startup
RUN echo "source /etc/profile.d/cubeclt-bin-path_1.18.0.sh" >> ~/.bashrc
