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
#include <vector>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv) {

  if (argc > 2) {

    SoundIn input(argv[1]);
    std::vector<Chn> chn(input.nchnls());
    std::vector<Delay> echo(input.nchnls(),
                            Delay(0.5, 0.75, def_vframes, input.sr()));
    std::vector<Pan> pan(input.nchnls());
    SigBus mix(1. / input.nchnls(), 0., false, 2);
    SoundOut output(argv[2], 2, def_vframes, input.sr());

    cout << Info::version();

    for (uint64_t i = 0; i < input.dur(); i += def_vframes) {
      input.read();
      for (uint32_t j = 0; j < input.nchnls(); j++) {
        chn[j].process(input, j + 1);
        echo[j].process(chn[j]);
        pan[j].process(echo[j] += chn[j], (1 + j) * input.nchnls() / 2.);
        mix.process(pan[j]);
      }
      output.write(mix);
      mix.clear();
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <dest>\n";
  return 1;
}
