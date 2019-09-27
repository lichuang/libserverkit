#!/bin/sh

pwd=`pwd`
third_party=${pwd}/third_party
pb=protobuf-3.9.0
gtest=googletest-release-1.8.0

echo "$pwd"
echo "${third_party}"

mkdir ${third_party}/include -p
mkdir ${third_party}/lib -p

cd $pwd/deps

echo "compile gtest..."
rm -fr ${gtest}
tar xvf ${gtest}.tar.gz
cd ${gtest}
cmake .
make -j8
cp ./googlemock/libgmock* ${third_party}/lib/
cp ./googlemock/gtest/libgtest* ${third_party}/lib/
cp -r googlemock/include/gmock ${third_party}/include/
cp -r googletest/include/gtest ${third_party}/include/
cd ../
rm -fr ${gtest}
echo "compile gtest done"

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
