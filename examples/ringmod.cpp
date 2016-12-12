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

using namespace AuLib;

int main(){
  double fm = 230., fc = 410.;
  Oscili mod,car;
  SoundOut output("dac");
 
  if(mod.error() == AULIB_NOERROR &&
     car.error() == AULIB_NOERROR &&
     output.error() == AULIB_NOERROR) {
    for(int i=0; i < def_sr*10; i+=def_vsize){
      mod.process(1.,fm);
      car.process(0.5,fc);
      output.write(car *= mod);     
    }
  }
  return 0;
}
