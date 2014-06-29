KaldiFace
=====

Some interface between Kaldi's table reader/writer and other libraries' data structures.
A stable header-only mini C++ library.

Dependencies
-----

 * Kaldi : http://kaldi.sourceforge.net/
 * Armadillo : http://arma.sourceforge.net/

These are optional and for unit tests only:

 * Tinyformat : https://github.com/c42f/tinyformat
 * Google Test : http://code.google.com/p/googletest/

An compile script to compile Kaldi with OpenBLAS under cygwin is attached in 

Install
-----
 1. Install all dependencies. You can find an example script of installing Kaldi with OpenBLAS under cygwin at `doc/compile_cygwin.sh`. This will not work out of box and is provided only for reference.
 2. Copy local.mk.template to local.mk, and point the variable inside to the actual location of kaldi source tree.
 3. Apply patch `doc/kaldi-blas.h.patch` to kaldi's `src/matrix/kaldi-blas.h` if needed.
 4. Run `make test` to make sure it actually works.

Usage
-----
```c++
#include "KaldiFace/common.h"

int main(int argc, char *argv[]) {
  try {
    using namespace kaldi;
    
    // ... Some initialization codes
    
    std::string rspecifier = "filename1.ark";
    std::string wspecifier = "filename2.ark";
    
    // Replace the original SequentialBaseFloatMatrixReader
    SequentialArmaFloatReader readerFeat(rspecifier);
    // Replace the original BaseFloatMatrixWriter
    ArmaFloatWriter writerFeat(wspecifier);
    
    // The remainings are standard Kaldi conventions
    for (; !feat_reader.Done(); feat_reader.Next()) {
      std::string keyThis = readerFeat.Key();
      auto& mFeat = readerFeat.Value();
      
      // ... Processing mFeat
      
      writerFeat.Write(keyThis, mFeat);
    } // end for each utterance
  } catch(const std::exception &e) {
    std::cerr << e.what();
    return 16;
  }
  return 0;
}
```

See the unit testings (`test/arma/*.cc`) for more usage examples.

Caveats and Known Bugs
-----
We added -std=gnu++11 flag for c++11 features; with some underlying matrix library of Kaldi this might cause compilation problems. If you are using OpenBLAS or for some reason some template errors appear, apply the patch in doc/ on (Kaldi source tree)/matrix/kaldi-blas.h.

Author and Acknowledgements
-----
Yu-chen Kao (cybeliak) : cybeliak [the mysterious symbol] mail asteraceae org

License
-----
This project is licensed under the Academic Free License, Version 3.0.
See LICENSE for details.
