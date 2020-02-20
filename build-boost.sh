#!/bin/bash

boost_factory="$1"
boost_prefix="$2"
boost_version="$3"
boost_ptversion="$4"

mkdir -p boost-factory
cd boost-factory

extract_log=extract.log

mkdir -p sources
cd sources
wget https://dl.bintray.com/boostorg/release/$ptversion/source/boost_$boost_version.tar.gz
tar -xvf boost_$boost_version.tar.gz > extract.log
cd boost_$boost_version

./bootstrap.sh --with-toolset=clang
./b2 --with-test --with-filesystem link=shared install --prefix="$boost_prefix"
./b2 --with-program_options link=static install --prefix="$boost_prefix"
./b2 --with-program_options link=shared install --prefix="$boost_prefix"

echo "[INFO]: Boost include dir: $(ls -d "$boost_prefix/include")"
echo "[INFO]: Boost library dir: $(ls -d "$boost_prefix/lib")"
