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

#include "matrixext.h"

namespace kaldi {

  template<>
  bool ArmaHolder<arma::mat,32>::Read(std::istream &is) {
    if (obj) delete obj;
    obj = new T;

    if (is.peek() == '\0') { // Kaldi's binary mode
      is.get();
      if (is.peek() != 'B') return false;
      is.get();
    } else { // Kaldi's text mode: not supported
      KALDI_WARN << "Sorry, text mode not supported";
      return false;
    }
    if (is.peek() == 'C') {
      is.get();
      KALDI_WARN << "Sorry, compressed matrix not supported";
      return false;
    }
    if (is.peek() == 'D') {
      KALDI_WARN << "ArmaHolder <32> only support float matrix data";
      return false;
    }

    std::string datatype;
    ReadToken(is, true, &datatype);

    try {
      int32 nRow, nCol;
      ReadBasicType(is, true, &nRow);
      ReadBasicType(is, true, &nCol);
      obj->resize(nRow, nCol);

      float buffer;
      for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
          is.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
          obj->at(i,j) = buffer;
        }
      }
      return is.good();
    } catch (std::exception &e) {
      KALDI_WARN << "Error on reading Table object " << e.what();
      delete obj;
      obj = NULL;
      return false;
    }
  }

  template<>
  bool ArmaHolder<arma::mat,32>::Write(std::ostream &os, bool binary, const arma::mat &obj) {
    os.put('\0'); os.put('B'); // Force binary mode
    os.put('F'); os.put('M'); os.put(' '); // Matrix header

    int32 nRow = obj.n_rows;
    int32 nCol = obj.n_cols;
    WriteBasicType(os, true, nRow);
    WriteBasicType(os, true, nCol);

    try {
      float buffer;
      for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
          buffer = obj.at(i,j);
          os.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
        }
      }

      return os.good();
    } catch (const std::exception &e) {
      KALDI_WARN << "Error on writing Table object " << e.what();
      return false;
    }
  }

  template<>
  bool ArmaHolder<arma::mat,64>::Read(std::istream &is) {
    if (obj) delete obj;
    obj = new T;

    if (is.peek() == '\0') { // Kaldi's binary mode
      is.get();
      if (is.peek() != 'B') return false;
      is.get();
    } else { // Kaldi's text mode: not supported
      KALDI_WARN << "Sorry, text mode not supported";
      return false;
    }
    if (is.peek() == 'C') {
      is.get();
      KALDI_WARN << "Sorry, compressed matrix not supported";
      return false;
    }
    if (is.peek() == 'F') {
      KALDI_WARN << "ArmaHolder <64> only support double matrix data";
      return false;
    }

    std::string datatype;
    ReadToken(is, true, &datatype);

    try {
      int32 nRow, nCol;
      ReadBasicType(is, true, &nRow);
      ReadBasicType(is, true, &nCol);
      obj->resize(nRow, nCol);

      double buffer;
      for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
          is.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
          obj->at(i,j) = buffer;
        }
      }
      return is.good();
    } catch (std::exception &e) {
      KALDI_WARN << "Error on reading Table object " << e.what();
      delete obj;
      obj = NULL;
      return false;
    }
  }

  template<>
  bool ArmaHolder<arma::mat,64>::Write(std::ostream &os, bool binary, const arma::mat &obj) {
    os.put('\0'); os.put('B'); // Force binary mode
    os.put('D'); os.put('M'); os.put(' '); // Matrix header

    int32 nRow = obj.n_rows;
    int32 nCol = obj.n_cols;
    WriteBasicType(os, true, nRow);
    WriteBasicType(os, true, nCol);

    try {
      double buffer;
      for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
          buffer = obj.at(i,j);
          os.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
        }
      }

      return os.good();
    } catch (const std::exception &e) {
      KALDI_WARN << "Error on writing Table object " << e.what();
      return false;
    }
  }

  template<class TableReaderType>
  void GetMatrixStats(const char* rspecifier,
      size_t* ndim = nullptr, size_t* nframe = nullptr,
      size_t* maxframe = nullptr, size_t* minframe = nullptr) {
    TableReaderType readerFeat(rspecifier);

    auto& mFirst = readerFeat.Value();
    size_t _ndim = mFirst.n_cols;
    size_t _maxframe = mFirst.n_rows;
    size_t _minframe = mFirst.n_rows;
    size_t _nframe = 0;
    for (; !readerFeat.Done(); readerFeat.Next()) {
      auto& mFeat = readerFeat.Value();
      size_t nframeThis = mFeat.n_rows;
      _nframe += nframeThis;
      if (_maxframe < nframeThis) _maxframe = nframeThis;
      if (_minframe > nframeThis) _minframe = nframeThis;
    }

    if (ndim != nullptr) *ndim = _ndim;
    if (nframe != nullptr) *nframe = _nframe;
    if (maxframe != nullptr) *maxframe = _maxframe;
    if (minframe != nullptr) *minframe = _minframe;
  }

  template void GetMatrixStats<SequentialArmaFloatReader>(const char*,
      size_t*, size_t*, size_t*, size_t*);
  template void GetMatrixStats<SequentialArmaDoubleReader>(const char*,
      size_t*, size_t*, size_t*, size_t*);

}
