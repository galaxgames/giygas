FROM ubuntu:bionic
RUN apt-get update
RUN apt-get install -y git cmake python3.6 build-essential xorg-dev
RUN apt-get install -y python3-pip
RUN apt-get install -y libvulkan-dev vulkan-utils
WORKDIR /root

COPY . .

RUN mkdir build
WORKDIR /root/build
CMD cmake -G "Unix Makefiles" .. && cmake --build . && ./core/giygas_test

