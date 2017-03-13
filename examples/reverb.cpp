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
#include <PConv.h>
#include <SampleTable.h>
#include <SoundIn.h>
#include <SoundOut.h>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>
#include <Fir.h>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv) {

  if (argc > 4) {
    SoundIn input(argv[1]);
    SampleTable ir(argv[2], 1);
    ir.rescale(std::atof(argv[3]));
    std::vector<Chn> chn(input.nchnls());
    std::vector<Fir>  early(input.nchnls(), Fir(ir,0,32)); 
    std::vector<PConv> mid(input.nchnls(), PConv(ir,32,0,32,256));
    std::vector<PConv> tail(input.nchnls(), PConv(ir,256,0,256,1024));
    SoundOut output(argv[4], 1);
    uint64_t end = input.dur() + 3 * input.sr(), t = 0;

    // if the impulse response is multichannel
    // and matches the number of input channels
    // re-init the PConv/Fir objects to take advantage of this
    if(ir.nchnls() == input.nchnls()){
      uint32_t c = 0;
      for(auto &obj : early)
	obj = Fir(ir,c++,32);
       c = 0;
       for(auto &obj : mid)
	 obj = PConv(ir,32,c++,32,256);
       c = 0;
       for(auto &obj : tail)
         obj = PConv(ir,256,c++,256);
      }
    
    while (t < end) {
      input();
      
      for (uint32_t i = 1; i < 2; i++) {
	early[i](chn[i](input));
	mid[i](chn[i]);
        output(1, early[i] += mid[i] += tail[i](chn[i]));
      }
      t = output.timestamp();
    }

    return 0;

  } else
    std::cout << "usage: " << argv[0] << " <source> <ir> <ir_gain> <dest>\n";
  return 1;
}
