#!/bin/bash

factory="$1"
prefix="$2"
version="$3"
ptversion="$4"

mkdir -p boost-factory
cd boost-factory

extract_log=extract.log

mkdir -p sources
cd sources
wget https://dl.bintray.com/boostorg/release/$ptversion/source/boost_$version.tar.gz
tar -xvf boost_$version.tar.gz > extract.log
cd boost_$version
%
./bootstrap.sh --with-toolset=clang
./b2 --with-test --with-filesystem link=shared install --prefix="$prefix"
./b2 --with-program_options link=static install --prefix="$prefix"
./b2 --with-program_options link=shared install --prefix="$prefix"

echo "[INFO]: Boost include dir: $(ls -d "$prefix/include")"
echo "[INFO]: Boost library dir: $(ls -d "$prefix/lib")"
