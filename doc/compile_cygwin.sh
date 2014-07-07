#!/bin/bash

export CC=gcc
export CXX=g++
export LD=ld
export MAKE=make
export SCRIPT_PATH="$(dirname $(readlink -f $0))"
export CFLAGS+=" -I/opt/local/include"
export CXXFLAGS+=" -I/opt/local/include"
export CPPFLAGS+=" -I/opt/local/include"
export LDFLAGS+=" -L/opt/local/lib"
mkdir -p /opt/local/src
cd /opt/local/src

##### OpenBLAS #####

rm -rf OpenBLAS
git clone https://github.com/xianyi/OpenBLAS.git
cd OpenBLAS
sed -i Makefile.system -e 's/^EXTRALIB += -lgfortran/& -lgomp/' # Patch for cygwin
make CC=gcc FC=gfortran BINARY=64 USE_THREAD=0 NUM_THREADS=1 NO_AFFINITY=1 TARGET=NEHALEM || exit 1
make PREFIX=/opt/local install || exit 1
cd ..
rm -rf OpenBLAS

##### Kaldi #####

svn co http://svn.code.sf.net/p/kaldi/code/trunk kaldi-trunk
cd kaldi-trunk/tools
CXXFLAGS+=" -O3" LDFLAGS+=" -s" make irstlm_tgt openfst sph2pipe || exit 1
cd ../src
make -j1 clean
install -v $SCRIPT_PATH/kaldi.mk . && \
  make depend && make -j$NUMBER_PARALLEL || exit 1
cd ../..
