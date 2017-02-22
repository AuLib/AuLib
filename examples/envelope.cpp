/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <Envel.h>
#include <Oscili.h>
#include <SoundOut.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main() {
  Segments segs(0., {1., .5, .0}, {.5, 1., .5});
  Envel env(segs, 2.);
  Oscili sig;
  SoundOut output("dac");
  bool release = false;
  uint32_t end = segs.frames(), rel = env.rframes();
  cout << Info::version();
  if (env.error() == AULIB_NOERROR) {
    if (sig.error() == AULIB_NOERROR) {
      if (output.error() == AULIB_NOERROR) {
        for (uint32_t i = 0; i < end + rel + 4400; i += def_vframes) {
          env.process();
          sig.process(env, 440.);
          if (!release && i >= end) {
            env.release();
            release = true;
          }
          output.write(sig);
        }
      } else
        cout << output.error_message() << "\n";
    } else
      cout << sig.error_message() << "\n";
  } else
    cout << env.error_message() << "\n";
  return 0;
}
