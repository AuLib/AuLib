/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <Chn.h>
#include <Delay.h>
#include <Pan.h>
#include <SigBus.h>
#include <SoundIn.h>
#include <SoundOut.h>
#include <iostream>
#include <numeric>
#include <vector>
#include <cstdlib>
#include <atomic>
#include <cmath>
#include <csignal>

using namespace AuLib;
using namespace std;

// handle ctrl-c
static atomic_bool running(true);
void signal_handler(int signal) {
  running = false;
  cout << "\nexiting...\n";
}

int main(int argc, const char **argv) {

  if (argc > 4) {
    // audio input
    SoundIn input(argv[1]);
    if(input.error() != AULIB_NOERROR) {
      cout << "error opening input\n";
      return -1;
    }
    // input channels
    vector<Chn> chn(input.nchnls()); 
    // delay lines
    double fdb = fabs(atof(argv[4]));
    vector<Delay> delay(input.nchnls(),
	   Delay(atof(argv[3]), fdb < 1.0 ? fdb : 0.99,
		 def_vframes, input.sr())); 
    // stereo panning
    vector<Pan> pan(input.nchnls()); 
    // mixing bus
    SigBus mix(1./input.nchnls(), 0., false, 2);
    // audio output
    SoundOut output(argv[2], 2, def_vframes,
		    input.sr());
    if(output.error() != AULIB_NOERROR) {
      cout << "error opening output\n";
      return -1;
    }
    uint64_t end = input.dur() + 5*output.sr(), t = 0;
    // list of channels
    vector<uint32_t> channels(input.nchnls());
    iota(channels.begin(), channels.end(), 0);
    signal(SIGINT, signal_handler);

    cout << Info::version();

    while (t < end && running) {
      input();
      for(uint32_t channel : channels) {
	chn[channel](input, channel + 1);
	delay[channel](chn[channel]);
	pan[channel](delay[channel] += chn[channel],
		     (1 + channel)*input.nchnls()/2.);
	mix(pan[channel]);
      }
      t = output(mix);
      mix.clear();
    }
    return 0;
  } else
    cout << "usage: " << argv[0] 
         <<  " <source> <dest> <delay> <feedback>\n";   
  return 1;
}
