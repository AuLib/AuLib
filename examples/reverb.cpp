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
#include <Fir.h>
#include <PConv.h>
#include <SampleTable.h>
#include <SoundIn.h>
#include <SoundOut.h>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv) {

  if (argc > 4) {
    const uint32_t dfrms = 32;
    const uint32_t part = 1024;
    SoundIn input(argv[1]);
    SampleTable ir(argv[2], 0);
    ir.rescale(std::atof(argv[3]));
    std::vector<Chn> chn(input.nchnls());
    std::vector<Fir> early(input.nchnls(),
                           Fir(ir, 0, dfrms, def_vframes, input.sr()));
    std::vector<PConv> mid(input.nchnls(), PConv(ir, dfrms, 0, dfrms, part,
                                                 def_vframes, input.sr()));
    std::vector<PConv> tail(
        input.nchnls(), PConv(ir, part, 0, part, 0, def_vframes, input.sr()));
    SoundOut output(argv[4], input.nchnls(), def_bframes, input.sr());
    uint64_t end = input.dur() + 3 * input.sr(), t = 0;
    std::vector<uint32_t> channels(input.nchnls());
    std::iota(channels.begin(), channels.end(), 0);

    // if the impulse response is multichannel
    // and matches the number of input channels
    // re-init the PConv/Fir objects to take advantage of this
    if (ir.nchnls() == input.nchnls())
      for (auto c : channels) {
        early[c] = Fir(ir, c, dfrms, def_vframes, input.sr());
        mid[c] = PConv(ir, dfrms, c, dfrms, part, def_vframes, input.sr());
        tail[c] = PConv(ir, part, c, part, 0, def_vframes, input.sr());
      }

    while (t < end) {
      input();
      for (auto c : channels) {
        early[c](chn[c](input));
        mid[c](chn[c]);
        output(c, early[c] += mid[c] += tail[c](chn[c]));
      }
      t = output.timestamp();
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <ir> <ir_gain> <dest>\n";
  return 1;
}
