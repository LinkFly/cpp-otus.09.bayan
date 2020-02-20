#!/bin/bash

mkdir boost-factory
cd boost-factory
boost_factory_dir="$(pwd)"
boost_prefix="$boost_factory_dir/boost"
export BOOST_LIBDRARYDIR="$boost_prefix/lib"
export LD_LIBRARY_PATH="$BOOST_LIBDRARYDIR"
wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
tar -xvf boost_1_72_0.tar.gz
cd boost_1_72_0
./bootstrap
./b2 --with-test --with-filesystem link=shared install --prefix="$boost_prefix"
./b2 --with-program_options link=static install --prefix="$boost_prefix"
./b2 --with-program_options link=shared install --prefix="$boost_prefix"
echo "[INFO]: Boost include dir: $(ls -d "$boost_prefix/include")
echo "[INFO]: Boost library dir: $(ls -d "$BOOST_LIBDRARYDIR")
