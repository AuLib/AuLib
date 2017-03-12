/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <SoundIn.h>
#include <SoundOut.h>
#include <SampleTable.h>
#include <PConv.h>
#include <iostream>
#include <numeric>
#include <vector>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv) {

  if (argc > 3) {

    SoundIn input(argv[1]);
    SampleTable ir(argv[2]);
    PConv  reverb(ir);
    SoundOut output(argv[3], 1, def_bframes, input.sr());
    uint64_t end = input.dur() + 5 * input.sr(), t = 0;

    while (t < end) {
      input();
      reverb(input);
      t = output(reverb);
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <ir> <dest>\n";
  return 1;
}
