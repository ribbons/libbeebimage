libbeebimage
============

libbeebimage is a cross-platform C & C++ library for reading graphics files
originally created on BBC Micro or Master computers.

Build & Install
---------------

You'll need [CMake](https://cmake.org/) in your PATH and a C++ compiler.

First, open a shell in the root of the project and then run the following to
generate the build files:

### Linux

``` sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

### Windows

``` sh
mkdir build
cd build
cmake ..
```

The command to build with under Windows will then depend on your C++ compiler.

Contribute
----------

Please feel free to send pull requests to fix bugs or add enhancements.
