# This file was generated using the following command:
# ./configure --mathlib=OPENBLAS --openblas-root=/opt/local --threaded-math=yes

# Rules that enable valgrind debugging ("make valgrind")

valgrind: .valgrind

.valgrind:
	echo -n > valgrind.out
	for x in $(TESTFILES); do echo $$x>>valgrind.out; valgrind ./$$x >/dev/null 2>> valgrind.out; done
	! ( grep 'ERROR SUMMARY' valgrind.out | grep -v '0 errors' )
	! ( grep 'definitely lost' valgrind.out | grep -v -w 0 )
	rm valgrind.out
	touch .valgrind


CONFIGURE_VERSION := 2
FSTROOT = /opt/local/src/kaldi-trunk/tools/openfst
# makefiles/kaldi.mk.cygwin contains Cygwin-specific rules

ifndef FSTROOT
$(error FSTROOT not defined.)
endif

    #-I ../../tools/CLAPACK/ \

CXXFLAGS = -msse -msse2 -w -I.. -DKALDI_DOUBLEPRECISION=0  \
    -DHAVE_POSIX_MEMALIGN -DHAVE_OPENBLAS -I/opt/local/include -I ../../tools/CLAPACK/ \
    -Wno-sign-compare -Winit-self -DUSE_KALDI_SVD \
    -I $(FSTROOT)/include \
    $(EXTRA_CXXFLAGS) \
    -O3 # -O0 -DKALDI_PARANOID 

LDFLAGS = -L/opt/local/lib -s -Wl,--enable-auto-import -Wl,-O1
LDLIBS = $(EXTRA_LDLIBS) $(FSTROOT)/lib/libfst.a -ldl -L/usr/lib/lapack \
         -lopenblas -lgfortran -lm -lpthread -lgomp
CXX = g++
CC = g++
RANLIB = ranlib
AR = ar

