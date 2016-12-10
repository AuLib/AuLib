/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <SoundOut.h>
#include <Oscili.h>
#include <SigBus.h>

using namespace AuLib;

int main(){
  double fm = 440., fc = 220., ndx = 5.;
  Oscili mod,car;
  SigBus sig;
  SoundOut output("dac");

  if(sig.error() == AULIB_NOERROR &&
     output.error() == AULIB_NOERROR) {
    for(int i=0; i < def_sr*10; i+=def_vsize){
      mod.process(ndx,fm);
      sig.process(mod.output(),fm,fc);
      car.process(0.5,sig.output());
      output.write(car.output());
    }
  }
  return 0;
}
