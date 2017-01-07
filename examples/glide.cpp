/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <Adsr.h>
#include <EnvelTable.h>
#include <Oscili.h>
#include <SoundOut.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main() {
  Segments segs(1., {2., 1.5, 1.}, {.25, 1.25, .5}, false);
  Adsr amp(1., 0.01, 1., 0.7, 0.1);
  EnvelTable etab(segs, false);
  Oscili freq(440., 1., etab);
  Oscili sig;
  SoundOut output("dac");
  bool release = false;
  uint32_t end = amp.frames() + 2 * def_sr, rel = amp.rframes() * 2;
  cout << Info::version();
  if (freq.error() == AULIB_NOERROR) {
    if (amp.error() == AULIB_NOERROR) {
      if (sig.error() == AULIB_NOERROR) {
        if (output.error() == AULIB_NOERROR) {
          for (int i = 0; i < end + rel; i += def_vframes) {
            freq.process();
            amp.process();
            sig.process(amp, freq);
            if (!release && i >= end) {
              amp.release();
              release = true;
            }
            output.write(sig);
          }
        } else
          cout << output.error_message() << "\n";
      } else
        cout << sig.error_message() << "\n";
    } else
      cout << amp.error_message() << "\n";
  } else
    cout << freq.error_message() << "\n";
  return 0;
}
