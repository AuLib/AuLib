AuLib
=================

A simple lightweight audio DSP library in C++ (ISO C++14). Licensed under
the GNU LGPL 3.0.

Building
----------------------
The library and example programs are built with CMake.The only
*optional* dependencies are libsndfile and portaudio for audio IO, but
the library can be built without these (but with no built-in support
for soundfile or realtime audio IO).

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
need doxygen to be installed in your system.

The latest build of the documentation is available at

[http://vlazzarini.github.io/aulib](http://vlazzarini.github.io/aulib)

Author
-----------------------
Victor Lazzarini, 2016-7
