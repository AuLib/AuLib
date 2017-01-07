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
#include <Oscili.h>
#include <SoundOut.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main() {
  Adsr env(0.9, 1, 2., 0.4, 0.5);
  Oscil sig;
  SoundOut output("dac");
  uint32_t end = env.frames() + (uint32_t)(def_sr), rel = env.rframes();
  bool release = false;
  cout << Info::version();
  if (sig.error() == AULIB_NOERROR) {
    if (output.error() == AULIB_NOERROR) {
      for (int i = 0; i < end + rel; i += def_vframes) {
        env.process();
        sig.process(env, 440.);
        output.write(sig);
        if (i >= end && !release) {
          env.release();
          release = true;
        }
      }
    } else
      cout << output.error_message() << "\n";
  } else
    cout << sig.error_message() << "\n";
  return 0;
}
