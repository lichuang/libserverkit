#!/bin/sh

pwd=`pwd`
third_party=${pwd}/third_party
pb=protobuf-3.9.0

echo "$pwd"
echo "${third_party}"

cd $pwd/deps

echo "compile protobuf..."
rm -fr ${pb}
tar xvf ${pb}.tar.gz
cd ${pb}
./autogen.sh
./configure --prefix=${third_party}
make -j6
make install
cd ..
rm -fr ${pb}
echo "compile protobuf done"
