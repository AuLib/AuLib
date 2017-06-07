[![Build Status](https://travis-ci.org/AuLib/AuLib.svg?branch=master)](https://travis-ci.org/AuLib/AuLib)
<a href="https://scan.coverity.com/projects/vlazzarini-aulib">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/11643/badge.svg"/>
</a>

<img src="aulib-spiral.png" width="128" height="128" />

[AuLib](http://github.com/AuLib/AuLib) is a simple lightweight C++ (ISO C++14) class library for audio DSP.

Build
===========================================
 
  The library and example programs are built with CMake. The only
  optional dependencies are libsndfile, portaudio, and portmidi, but
  these are not strictly needed to build and use AuLib.
 
  The code can be used within any audio processing context and it can
  be incorporated in plugins and other programs with their own IO.
 

``` 
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make
 $ make install
```

Documentation
----------------------------------------------

Documentation can be built from the build directory using 

```
 $ make doc
```

HTML, latex and manual files are placed in the `docs` subdirectory under
the build directory.

A recently-generated PDF manual is found in the toplevel `./docs`
directory, along with other aulib-related documents. The HTML manual
can also be found in [https://aulib.github.io](https://aulib.github.io).

Using
=====================================
 
  A number of simple examples are supplied to demonstrate how the
  library can be used. However, these are only very simple programs
  that only hint at the possible applications. Generally speaking,
  objects of the different processing classes are instantiated and
  then used to generate or process vectors of audio data, which are
  kept the library objects.
 
  It is possible to access the output samples of each object directly,
  and processing functions can take both objects (const AudioObj&) and
  audio vectors (const double) as inputs (as well as other
  parameters, depending on the object type). A number of operations
  can be applied to objects (such as scaling, offsetting, mixing to,
  etc.).
 
  A minimal example is shown below where an echo effect is applied
  to an arbitrary input, using SoundIn, SoundOut, Chn, Pan, SigBus
  and Delay objects.
 
```
#include <Chn.h>
#include <Delay.h>
#include <Pan.h>
#include <SigBus.h>
#include <SoundIn.h>
#include <SoundOut.h>
#include <iostream>
#include <numeric>
#include <vector>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv) {

  if (argc > 2) {

    SoundIn input(argv[1]);
    std::vector<Chn> chn(input.nchnls());
    std::vector<Delay> echo(input.nchnls(),
                            Delay(0.5, 0.5, def_vframes, input.sr()));
    std::vector<Pan> pan(input.nchnls());
    SigBus mix(1. / input.nchnls(), 0., false, 2);
    SoundOut output(argv[2], 2, def_bframes, input.sr());
    uint64_t end = input.dur() + 5 * input.sr(), t = 0;

    std::vector<uint32_t> channels(input.nchnls());
    std::iota(channels.begin(), channels.end(), 0);

    cout << Info::version();

    while (t < end) {
      input();
      for (uint32_t channel : channels) {
        chn[channel](input, channel + 1);
        echo[channel](chn[channel]);
        pan[channel](echo[channel] += chn[channel],
                     (1 + channel) * input.nchnls() / 2.);
        mix(pan[channel]);
      }
      t = output(mix);
      mix.clear();
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <dest>\n";
  return 1;
}

```
 
Extending
============================

  The library classes can be easily extended. There is quite a
  bit freedom to do this, as there are not strict prescriptions
  on the form or signature of processing methods. Usually we
  would inherit from the AudioBase class to allow easy
  integration with existing objects, and to avail of basic audio
  processing facilities provided there. Supplying a processing
  method that consumes a vector of samples and another that reads
  from an AudioObj& is the minimum necessary to allow full
  integration. The recommended approach is to separate
  interface from implementation, and to provide a means for
  overriding this. 
 
  The following example is a skeleton of an AudioBase-derived
  class that demonstrates these ideas:
 
```
namespace AuLib {

class NewClass : public AudioBase {

  virtual const double *dsp();

protected:
  double m_par;

public:
  NewClass(double param = .5,
           uint32_t nchnls = def_nchnls uint32_t vframes = def_vframes,
           double sr = def_sr)
      : m_param(param), AudioBase(nchnls, vframes, sr){};

  const double process(const double sig) {
    return dsp(sig);
 }

  const double process(const double sig, double par) {
    m_par = par;
    return process(sig);
  }

  const NewClass &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return this;
  }

  const NewClass &process(const AudioBase &obj, double par) {
    m_par = par;
    process(obj);
    return this;
	}

  const NewClass &operator()(const AudioBase &obj) {
     return process(obj);
  } 

  const NewClass &operator()(const AudioBase &obj, double par) {
     return process(obj, par);
  }
};
}
```
 
License
=====================

  Copyright (C) 2016-7 V Lazzarini
 
  This software is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this software; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 /
