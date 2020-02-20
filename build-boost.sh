#!/bin/bash

mkdir -p boost-factory
cd boost-factory

boost_factory_dir="$(pwd)"
boost_prefix="$boost_factory_dir/usrlib/boost_1_72_0"
extract_log=extract.log
export BOOST_LIBDRARYDIR="$boost_prefix/lib"
export LD_LIBRARY_PATH="$BOOST_LIBDRARYDIR"

mkdir -p sources
cd sources
wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
tar -xvf boost_1_72_0.tar.gz > extract.log
cd boost_1_72_0

./bootstrap.sh --with-toolset=clang
./b2 --with-test --with-filesystem link=shared install --prefix="$boost_prefix"
./b2 --with-program_options link=static install --prefix="$boost_prefix"
./b2 --with-program_options link=shared install --prefix="$boost_prefix"
echo "[INFO]: Boost include dir: $(ls -d "$boost_prefix/include")"
echo "[INFO]: Boost library dir: $(ls -d "$BOOST_LIBDRARYDIR")"
