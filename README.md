AuLib
=================

A simple audio DSP library in C++ for educational purposes. Licensed under
the GNU LGPL 3.0.

Building
----------------------
The library and example programs are built with CMake. The only
dependencies are libsndfile and portaudio and they are only required
to build the example programs that use the audio IO classes.
The code can be used within any audio processing context and it can
be incorporated in plugins and other programs with their own IO.

```
$ mkdir build
$ cd build
$ cmake .. 
$ make
$ make install
```

The CMake variable CMAKE\_INSTALL\_PREFIX can be used to select the
installation base directory.

Documentation
-----------------------
Documentation is auto-generated from the headers using doxygen
as html, latex sources and man pages. From the build directory, 

```
$ make doc
```

will build the documentation and place it under ./docs. You will
need doxygen to be installed in your system

Author
-----------------------
Victor Lazzarini, 2016
