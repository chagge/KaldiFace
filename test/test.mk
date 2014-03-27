# Master test program makefile
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

RUNTESTBINS := $(patsubst %, Run%, ${TESTBINS})

all: ${TESTBINS}
	@true

test: ${RUNTESTBINS}

RunTest%: Test%
	@./$< --gtest_color=yes

include $(dir $(realpath $(lastword $(MAKEFILE_LIST))))/../common.mk

Test%: Test%.cc ${ADDITIONALDEP}
	@printf "\e[1;32m CXX+LD $< -> $@ \e[m\n"
	@${CXX} ${CXXFLAGS} ${LDFLAGS} -O0 -g1 -I${KALDIFACE_ROOT} -o $@ $< ${ADDLIBS} ${COMMONLIBS} ${LDLIBS} -lgtest

clean:
	@rm -f *.stackdump *.o *.tmp ${TESTBINS}
