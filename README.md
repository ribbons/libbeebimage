libbeebimage
============

A cross-platform C & C++ library for reading graphics files originally created
on BBC Micro or Master computers.

The most common ways of storing images on the BBC were; as a dump of the
graphics memory, in a run-length encoded format loaded by a program called
[LdPic](https://nerdoftheherd.com/projects/libbeebimage/ldpic/), or in a
different (optionally) run-length encoded format understood by a program called
[ScrLoad](http://mdfs.net/Mirror/Image/JGH/).

However, due to the unusual way that the BBC's graphics memory was mapped to
the screen (in blocks of eight bytes) it is not straight-forward to view these
formats on a PC with standard tools.

This library is able to read BBC graphics files saved in LdPic, ScrLoad or
memory dump format in Modes 0, 1, 2, 4 and 5.

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
