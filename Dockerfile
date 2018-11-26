FROM ubuntu:bionic
RUN apt-get update
RUN apt-get install -y git cmake python3.6 build-essential xorg-dev
RUN apt-get install -y python3-pip
RUN apt-get install -y libvulkan-dev vulkan-utils
WORKDIR /root

COPY dew dew
RUN python3.6 -m pip install -e ./dew

COPY dewfile.json dewfile.json
RUN python3.6 -m dew --cmake-generator "Unix Makefiles"

COPY CMakeLists.txt CMakeLists.txt
COPY Config.cmake.in Config.cmake.in
COPY core core
COPY util util
COPY examples examples
COPY giygasfile giygasfile

COPY glslang glslang
WORKDIR glslang
RUN mkdir build
WORKDIR build
RUN cmake -G "Unix Makefiles" ..
RUN cmake --build . --target install

RUN mkdir build
WORKDIR /root/build
CMD cmake -G "Unix Makefiles" .. -DCMAKE_PREFIX_PATH="../.dew/install" -DCMAKE_INSTALL_PREFIX="../.dew/install" && cmake --build . && ./core/giygas_test

