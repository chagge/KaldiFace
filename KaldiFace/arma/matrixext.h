// vim: fdm=syntax
// Extension to Kaldi's IO and matrix libraries
/***************************************************************************\
  Copyright 2011-2014, Yu-chen Kao

  Licensed under the Academic Free License, Version 3.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at the root directory of this
  software project or at:
 
      http://opensource.org/licenses/AFL-3.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
\***************************************************************************/

// This file is a derived work of the following file in the Kaldi project:
//   (Kaldi Root)/src/base/io-funcs-inl.h
//   (Kaldi Root)/src/util/kaldi-holder-inl.h
// which is licensed under the Apache License, Version 2.0,
// by Microsoft Corporation et al. The license can be downloaded from
//   http://www.apache.org/licenses/LICENSE-2.0.html.
// The source code for Kaldi is available from 
//   http://kaldi.sourceforge.net/.

#ifndef __INCLUDED_KALDIFACE_COMMON_MATRIXEXT_H_
#define __INCLUDED_KALDIFACE_COMMON_MATRIXEXT_H_

#include "util/common-utils.h"
#include "matrix/kaldi-matrix.h"
#include <armadillo>

namespace kaldi {

  // Holder of Armadillo's matrix onto Kaldi matrix data format
  template<class ContentType, int Size> class ArmaHolder {
    public:
      // I don't actually have an idea why we need this
      typedef ContentType T;

      ArmaHolder(): obj(NULL) { }
      ~ArmaHolder() { if (obj) delete obj; }

      bool Read(std::istream &is);
      static bool Write(std::ostream &os, bool binary, const T &t);

      inline void Clear() { if (obj) { delete obj; obj = NULL; } }
      const T &Value() const { return *(obj); }
      static bool IsReadInBinary() { return true; }

    private:
      KALDI_DISALLOW_COPY_AND_ASSIGN(ArmaHolder);
      T *obj;
  }; // end definition of ArmaHolder

  template<class TableReaderType>
  void GetMatrixStats(const char* rspecifier,
      size_t* ndim = nullptr, size_t* nframe = nullptr,
      size_t* maxframe = nullptr, size_t* minframe = nullptr);

  typedef TableWriter<ArmaHolder<arma::mat, 32> > ArmaFloatWriter;
  typedef SequentialTableReader<ArmaHolder<arma::mat, 32> > SequentialArmaFloatReader;
  typedef RandomAccessTableReader<ArmaHolder<arma::mat, 32> >  RandomAccessArmaFloatReader;
  typedef RandomAccessTableReaderMapped<ArmaHolder<arma::mat, 32> >  RandomAccessArmaFloatReaderMapped;
  typedef TableWriter<ArmaHolder<arma::mat, 64> > ArmaDoubleWriter;
  typedef SequentialTableReader<ArmaHolder<arma::mat, 64> > SequentialArmaDoubleReader;
  typedef RandomAccessTableReader<ArmaHolder<arma::mat, 64> >  RandomAccessArmaDoubleReader;
  typedef RandomAccessTableReaderMapped<ArmaHolder<arma::mat, 64> >  RandomAccessArmaDoubleReaderMapped;

}

#endif /* end of include guard */
