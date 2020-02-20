#!/bin/bash
echo $1
boost_factory="$1"
boost_prefix="$2"

mkdir -p boost-factory
cd boost-factory

extract_log=extract.log

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
echo "[INFO]: Boost library dir: $(ls -d "$boost_prefix/lib")"
