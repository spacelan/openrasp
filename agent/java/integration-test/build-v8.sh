#!/bin/bash

set +e

pushd ../engine/src/main/native

tar zxf libv8.tar.gz -C /tmp

SRC=$(pwd)

pushd /tmp

cmake cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/tmp/libv8/linux ${SRC}

make

popd

popd