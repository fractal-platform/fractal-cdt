#! /bin/bash

printf "\t=========== Building fractal.cdt ===========\n\n"

RED='\033[0;31m'
NC='\033[0m'
txtbld=$(tput bold)
bldred=${txtbld}$(tput setaf 1)
txtrst=$(tput sgr0)

export DISK_MIN=10
CORES=`getconf _NPROCESSORS_ONLN`

# Use current directory's tmp directory
export TEMP_DIR="${SOURCE_DIR}/tmp"
mkdir -p $TEMP_DIR
rm -rf $TEMP_DIR/*

unamestr=`uname`
if [[ "${unamestr}" == 'Darwin' ]]; then
   BOOST=/usr/local
   CXX_COMPILER=g++
   export ARCH="Darwin"
   bash ./build-scripts/pre_build_darwin.sh
else
   OS_NAME=$( cat /etc/os-release | grep ^NAME | cut -d'=' -f2 | sed 's/\"//gI' )

   case "$OS_NAME" in
      "Amazon Linux AMI")
         export ARCH="Amazon Linux AMI"
         bash ./build-scripts/pre_build_amazon.sh
         ;;
      "CentOS Linux")
         export ARCH="Centos"
         export CMAKE=${HOME}/opt/cmake/bin/cmake
         bash ./build-scripts/pre_build_centos.sh
         ;;
      "Linux Mint")
         export ARCH="Linux Mint"
         bash ./build-scripts/pre_build_ubuntu.sh
         ;;
      "Ubuntu")
         export ARCH="Ubuntu"
         bash ./build-scripts/pre_build_ubuntu.sh
         ;;
      "Debian GNU/Linux")
         export ARCH="Debian"
         bash ./build-scripts/pre_build_ubuntu.sh
         ;;
      *)
         printf "\\n\\tUnsupported Linux Distribution. Exiting now.\\n\\n"
         exit 1
   esac
fi


mkdir -p build/tools
pushd build/tools &> /dev/null

if [ -z "$CMAKE" ]; then
  CMAKE=$( command -v cmake )
fi

"$CMAKE" ../../tools
if [ $? -ne 0 ]; then
   exit -1;
fi
make -j${CORES} && make install
if [ $? -ne 0 ]; then
   exit -1;
fi
popd &> /dev/null


mkdir -p build/libraries
pushd build/libraries &> /dev/null

if [ -z "$CMAKE" ]; then
  CMAKE=$( command -v cmake )
fi

"$CMAKE" ../../libraries -DCMAKE_OSX_SYSROOT="/"
if [ $? -ne 0 ]; then
   exit -1;
fi
make -j${CORES} && make install
if [ $? -ne 0 ]; then
   exit -1;
fi
popd &> /dev/null
