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

An compile script to compile Kaldi with OpenBLAS under cygwin is attached in doc/compile\_cygwin.sh. This will not work out of box and is provided only for reference.

Install
-----
TBD.

Usage
-----
See the unit testings (`test/arma/*.cc`) for usage examples.

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
