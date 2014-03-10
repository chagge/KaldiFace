// vim: fdm=syntax
// Test Kaldi holder for Armadillo matrices
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

#include <gtest/gtest.h>
#include "KaldiFace/common.h"

#include <tinyformat.h>
using tfm::format;

namespace  { // for Kaldi and Arma interaction

  class TestFixKaldiArma : public ::testing::Test {
    protected:
      // just too lazy to mock, use real filesystem
      const char* fnTest = "ark:TestKaldiArma.tmp";
      const size_t nMtx = 4;
      std::vector<kaldi::Matrix<double>> lAnsKaldi;
      std::vector<kaldi::Matrix<float>> lAnsKaldiFloat;
      std::vector<arma::Mat<double>> lAnsArma;
      std::vector<kaldi::Matrix<double>> lOutKaldi;
      std::vector<kaldi::Matrix<float>> lOutKaldiFloat;
      std::vector<arma::Mat<double>> lOutArma;
  
      virtual void SetUp() {
        lAnsArma.push_back(arma::randn<arma::Mat<double>>(1,1));
        lAnsArma.push_back(arma::randn<arma::Mat<double>>(1,75));
        lAnsArma.push_back(arma::randn<arma::Mat<double>>(75,1));
        lAnsArma.push_back(arma::randn<arma::Mat<double>>(75,75));
        lAnsKaldi.resize(nMtx);
        lAnsKaldiFloat.resize(nMtx);
        lOutKaldi.resize(nMtx);
        lOutKaldiFloat.resize(nMtx);
        lOutArma.resize(nMtx);
        for (size_t n = 0; n < nMtx; ++n) {
          size_t nRow = lAnsArma[n].n_rows;
          size_t nCol = lAnsArma[n].n_cols;
          lAnsKaldi[n].Resize(nRow, nCol);
          lAnsKaldiFloat[n].Resize(nRow, nCol);
          lOutKaldi[n].Resize(nRow, nCol);
          lOutKaldiFloat[n].Resize(nRow, nCol);
          lOutArma[n].resize(nRow, nCol);
          for (size_t x = 0; x < nRow; ++x) {
            for (size_t y = 0; y < nCol; ++y) {
              lAnsKaldi[n](x,y) = lAnsArma[n].at(x,y);
              lAnsKaldiFloat[n](x,y) = lAnsArma[n].at(x,y);
            }
          }
        } // end for matrices
      } // end SetUp for TestFixKaldiArma

      void CheckKaldiOut(size_t n) {
        SCOPED_TRACE(format("CheckKaldiOut : Matrix %d", n));
        ASSERT_EQ(lAnsKaldi[n].NumCols(), lOutKaldi[n].NumCols());
        ASSERT_EQ(lAnsKaldi[n].NumRows(), lOutKaldi[n].NumRows());
        size_t nRow = lAnsArma[n].n_rows;
        size_t nCol = lAnsArma[n].n_cols;
        for (size_t x = 0; x < nRow; ++x) {
          for (size_t y = 0; y < nCol; ++y) {
            SCOPED_TRACE(format("At pos %d,%d", x, y));
            ASSERT_FLOAT_EQ(lAnsKaldi[n](x,y), lOutKaldi[n](x,y));
          }
        }
      } // end test routine for Kaldi matrix

      void CheckArmaOut(size_t n) {
        SCOPED_TRACE(format("CheckArmaOut : Matrix %d", n));
        ASSERT_EQ(lAnsArma[n].n_cols, lOutArma[n].n_cols);
        ASSERT_EQ(lAnsArma[n].n_rows, lOutArma[n].n_rows);
        size_t nRow = lAnsArma[n].n_rows;
        size_t nCol = lAnsArma[n].n_cols;
        for (size_t x = 0; x < nRow; ++x) {
          for (size_t y = 0; y < nCol; ++y) {
            SCOPED_TRACE(format("At pos %d,%d", x, y));
            ASSERT_FLOAT_EQ(lAnsArma[n](x,y), lOutArma[n](x,y));
          }
        }
      } // end test routine for Arma matrix
  }; // end Fixture class TestFixKaldiArma

  TEST_F(TestFixKaldiArma, TestRead32) {
    for (size_t n = 0; n < nMtx; ++n) {
      { // Make the destructor flush automatically
        kaldi::BaseFloatMatrixWriter writerFeat(fnTest);
        std::string key = "TestKey";
        writerFeat.Write(key, lAnsKaldiFloat[n]);
      }
      kaldi::SequentialArmaFloatReader readerFeat(fnTest);
      EXPECT_EQ(readerFeat.Key(), "TestKey");
      lOutArma[n] = readerFeat.Value();
      CheckArmaOut(n);
    } // end for each test matrix
  } // end TEST_F TestFixKaldiArma.TestRead32

  TEST_F(TestFixKaldiArma, TestWrite32) {
    for (size_t n = 0; n < nMtx; ++n) {
      { // Make the destructor flush automatically
        kaldi::ArmaFloatWriter writerFeat(fnTest);
        std::string key = "TestKey";
        writerFeat.Write(key, lAnsArma[n]);
      }
      kaldi::SequentialBaseFloatMatrixReader readerFeat(fnTest);
      EXPECT_EQ(readerFeat.Key(), "TestKey");
      lOutKaldiFloat[n] = readerFeat.Value();
      // Simply copy
      for (size_t x = 0; x < lAnsArma[n].n_rows; ++x) {
        for (size_t y = 0; y < lAnsArma[n].n_cols; ++y) {
          lOutKaldi[n](x,y) = lOutKaldiFloat[n](x,y);
        } // end for y
      } // end for x
      CheckKaldiOut(n);
    } // end for each test matrix
  } // end TEST_F TestFixKaldiArma.TestWrite32

  TEST_F(TestFixKaldiArma, TestRead64) {
    for (size_t n = 0; n < nMtx; ++n) {
      { // Make the destructor flush automatically
        kaldi::DoubleMatrixWriter writerFeat(fnTest);
        std::string key = "TestKey";
        writerFeat.Write(key, lAnsKaldi[n]);
      }
      kaldi::SequentialArmaDoubleReader readerFeat(fnTest);
      EXPECT_EQ(readerFeat.Key(), "TestKey");
      lOutArma[n] = readerFeat.Value();
      CheckArmaOut(n);
    } // end for each test matrix
  } // end TEST_F TestFixKaldiArma.TestRead64

  TEST_F(TestFixKaldiArma, TestWrite64) {
    for (size_t n = 0; n < nMtx; ++n) {
      { // Make the destructor flush automatically
        kaldi::ArmaDoubleWriter writerFeat(fnTest);
        std::string key = "TestKey";
        writerFeat.Write(key, lAnsArma[n]);
      }
      kaldi::SequentialDoubleMatrixReader readerFeat(fnTest);
      EXPECT_EQ(readerFeat.Key(), "TestKey");
      lOutKaldi[n] = readerFeat.Value();
      CheckKaldiOut(n);
    } // end for each test matrix
  } // end TEST_F TestFixKaldiArma.TestWrite64

  TEST_F(TestFixKaldiArma, TestMultiRead32) {
    {
      kaldi::BaseFloatMatrixWriter writerFeat(fnTest);
      for (size_t n = 0; n < nMtx; ++n) {
        std::string key = format("TestKey%d",n);
        writerFeat.Write(key, lAnsKaldiFloat[n]);
      } // end for each test matrix
    }
    kaldi::SequentialArmaFloatReader readerFeat(fnTest);
    for (size_t n=0; !readerFeat.Done(); readerFeat.Next(), ++n) {
      ASSERT_LT(n, nMtx);
      EXPECT_EQ(readerFeat.Key(), format("TestKey%d",n));
      lOutArma[n] = readerFeat.Value();
      CheckArmaOut(n);
    } // end for each read matrix
  } // end TEST_F TestFixKaldiArma.TestMultiRead32

  TEST_F(TestFixKaldiArma, TestMultiWrite32) {
    {
      kaldi::ArmaFloatWriter writerFeat(fnTest);
      for (size_t n = 0; n < nMtx; ++n) {
        std::string key = format("TestKey%d",n);
        writerFeat.Write(key, lAnsArma[n]);
      } // end for each test matrix
    }
    kaldi::SequentialBaseFloatMatrixReader readerFeat(fnTest);
    for (size_t n=0; !readerFeat.Done(); readerFeat.Next(), ++n) {
      ASSERT_LT(n, nMtx);
      EXPECT_EQ(readerFeat.Key(), format("TestKey%d",n));
      lOutKaldiFloat[n] = readerFeat.Value();
      // Simply copy
      for (size_t x = 0; x < lAnsArma[n].n_rows; ++x) {
        for (size_t y = 0; y < lAnsArma[n].n_cols; ++y) {
          lOutKaldi[n](x,y) = lOutKaldiFloat[n](x,y);
        } // end for y
      } // end for x
      CheckKaldiOut(n);
    } // end for each read matrix
  } // end TEST_F TestFixKaldiArma.TestMultiWrite32

  TEST_F(TestFixKaldiArma, TestMultiRead64) {
    {
      kaldi::DoubleMatrixWriter writerFeat(fnTest);
      for (size_t n = 0; n < nMtx; ++n) {
        std::string key = format("TestKey%d",n);
        writerFeat.Write(key, lAnsKaldi[n]);
      } // end for each test matrix
    }
    kaldi::SequentialArmaDoubleReader readerFeat(fnTest);
    for (size_t n=0; !readerFeat.Done(); readerFeat.Next(), ++n) {
      ASSERT_LT(n, nMtx);
      EXPECT_EQ(readerFeat.Key(), format("TestKey%d",n));
      lOutArma[n] = readerFeat.Value();
      CheckArmaOut(n);
    } // end for each read matrix
  } // end TEST_F TestFixKaldiArma.TestMultiRead64

  TEST_F(TestFixKaldiArma, TestMultiWrite64) {
    {
      kaldi::ArmaDoubleWriter writerFeat(fnTest);
      for (size_t n = 0; n < nMtx; ++n) {
        std::string key = format("TestKey%d",n);
        writerFeat.Write(key, lAnsArma[n]);
      } // end for each test matrix
    }
    kaldi::SequentialDoubleMatrixReader readerFeat(fnTest);
    for (size_t n=0; !readerFeat.Done(); readerFeat.Next(), ++n) {
      ASSERT_LT(n, nMtx);
      EXPECT_EQ(readerFeat.Key(), format("TestKey%d",n));
      lOutKaldi[n] = readerFeat.Value();
      CheckKaldiOut(n);
    } // end for each read matrix
  } // end TEST_F TestFixKaldiArma.TestMultiWrite64

  TEST_F(TestFixKaldiArma, TestDeathArma32) {
    { // Make the destructor flush automatically
      kaldi::DoubleMatrixWriter writerFeat(fnTest);
      std::string key = "TestKey";
      writerFeat.Write(key, lAnsKaldi[0]);
    }
    ASSERT_DEATH(kaldi::SequentialArmaFloatReader readerFeat(fnTest), "only support float matrix");
  } // end TEST_F TestFixKaldiArma.TestDeathArma32

  TEST_F(TestFixKaldiArma, TestDeathArma64) {
    { // Make the destructor flush automatically
      kaldi::BaseFloatMatrixWriter writerFeat(fnTest);
      std::string key = "TestKey";
      writerFeat.Write(key, lAnsKaldiFloat[0]);
    }
    ASSERT_DEATH(kaldi::SequentialArmaDoubleReader readerFeat(fnTest), "only support double matrix");
  } // end TEST_F TestFixKaldiArma.TestDeathArma64


  TEST(TestGetMatrixStats, TestGetStats32) {
    const char* fnTest = "ark:TestKaldiArma.tmp";
    {
      kaldi::ArmaFloatWriter writerFeat(fnTest);
      writerFeat.Write("t1", arma::randn<arma::Mat<double>>(42,13));
      writerFeat.Write("t2", arma::randn<arma::Mat<double>>(16,13));
      writerFeat.Write("t3", arma::randn<arma::Mat<double>>(171,13));
      writerFeat.Write("t4", arma::randn<arma::Mat<double>>(94,13));
    }

    size_t nfile, ndim, nframe, maxframe, minframe;
    kaldi::GetMatrixStats<kaldi::SequentialArmaFloatReader>
      (fnTest, &nfile, &ndim, &nframe, &maxframe, &minframe);
    ASSERT_EQ(nfile, 4);
    ASSERT_EQ(ndim, 13);
    ASSERT_EQ(nframe, 323);
    ASSERT_EQ(maxframe, 171);
    ASSERT_EQ(minframe, 16);
  } // end TEST_F TestFixKaldiArma.TestGetStats

  TEST(TestGetMatrixStats, TestGetStats64) {
    const char* fnTest = "ark:TestKaldiArma.tmp";
    {
      kaldi::ArmaDoubleWriter writerFeat(fnTest);
      writerFeat.Write("t1", arma::randn<arma::Mat<double>>(42,13));
      writerFeat.Write("t2", arma::randn<arma::Mat<double>>(16,13));
      writerFeat.Write("t3", arma::randn<arma::Mat<double>>(171,13));
      writerFeat.Write("t4", arma::randn<arma::Mat<double>>(94,13));
    }

    size_t nfile, ndim, nframe, maxframe, minframe;
    kaldi::GetMatrixStats<kaldi::SequentialArmaDoubleReader>
      (fnTest, &nfile, &ndim, &nframe, &maxframe, &minframe);
    ASSERT_EQ(nfile, 4);
    ASSERT_EQ(ndim, 13);
    ASSERT_EQ(nframe, 323);
    ASSERT_EQ(maxframe, 171);
    ASSERT_EQ(minframe, 16);
  } // end TEST_F TestFixKaldiArma.TestGetStats

} // namespace 

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
