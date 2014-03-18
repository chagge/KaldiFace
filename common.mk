# The common makefile for others to include
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

OLDCXXFLAGS := ${CXXFLAGS}
OLDLDFLAGS := ${LDFLAGS}

KALDIFACE_ROOT := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

include ${KALDIFACE_ROOT}/local.mk
include ${KALDI_ROOT}/src/kaldi.mk

CXXFLAGS := $(shell echo ${CXXFLAGS} ${OLDCXXFLAGS} | sed "s%-I \?\.\.%-I${KALDI_ROOT}/src%g")
LDFLAGS := $(shell echo ${LDFLAGS} ${OLDLDFLAGS})

CXXFLAGS := ${CXXFLAGS} -std=gnu++11

COMMONLIBS := ${KALDI_ROOT}/src/matrix/kaldi-matrix.a \
         ${KALDI_ROOT}/src/util/kaldi-util.a \
         ${KALDI_ROOT}/src/base/kaldi-base.a -larmadillo

.PRECIOUS: Test%

.PHONY: all test clean
