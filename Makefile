# Project main makefile
#***************************************************************************
# Copyright 2011-2014, Yu-chen Kao
#
# Licensed under the Academic Free License, Version 3.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at the root directory of this
# software project or at:
#
#     http://opensource.org/licenses/AFL-3.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#***************************************************************************

MODULES = arma
ALLSOURCES = $(shell find KaldiFace/ -type f -name '*.cc') $(shell find KaldiFace/ -type f -name '*.h')
ALLOBJSREC = $(patsubst %.cc, %.o, $(shell find KaldiFace/ -type f -name '*.cc'))

.PHONY: all test clean

all: libKaldiFace.a

libKaldiFace.a: ${ALLSOURCES}
	@+$(foreach m,${MODULES}, ${MAKE} -C KaldiFace/${m} &&) true
	ar rcs libKaldiFace.a ${ALLOBJSREC}

clean:
	@+$(foreach m,${MODULES}, ${MAKE} -C KaldiFace/${m} clean &&) true
	@+$(foreach m,${MODULES}, ${MAKE} -C test/${m} clean &&) true
	rm -f *.stackdump libKaldiFace.a

test: libKaldiFace.a
	@+$(foreach m,${MODULES}, ${MAKE} -C test/${m} test &&) true
