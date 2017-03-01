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
#include <Tapi.h>
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
    SoundOut output(argv[2], 2, def_vframes, input.sr());
    uint64_t end = input.dur() + 5 * input.sr();

    std::vector<uint32_t> channels(input.nchnls());
    std::iota(channels.begin(), channels.end(), 0);

    cout << Info::version();

    while ((end -= def_vframes) > def_vframes) {
      input();
      for (uint32_t channel : channels) {
        chn[channel](input, channel + 1);
        echo[channel](chn[channel]);
        pan[channel](echo[channel] += chn[channel],
                     (1 + channel) * input.nchnls() / 2.);
        mix(pan[channel]);
      }
      output(mix);
      mix.clear();
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <dest>\n";
  return 1;
}
