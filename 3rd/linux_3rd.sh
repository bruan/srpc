#!/bin/sh

git clone -b 2.7.0 https://github.com/google/protobuf.git ./protobuf_src

cd protobuf_src/cmake
mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=../../install ../
make
make install

cd ../../../
mkdir -p protobuf
mkdir -p protobuf/include
mkdir -p protobuf/lib
mkdir -p protobuf/lib/linux

cp -rf protobuf_src/install/include protobuf
cp -rf protobuf_src/install/lib64/libprotobuf.a protobuf/lib/linux