/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <TableRead.h>
#include <SoundOut.h>
#include <WaveTables.h>
#include <Phasor.h>

using namespace AuLib;

int main(){
  
  TriangleTable wave(50);
  Phasor   phase(440.);
  TableRead sig(wave);
  SoundOut output("dac");

  if(wave.error() == AULIB_NOERROR &&
     phase.error() == AULIB_NOERROR &&
     sig.error() == AULIB_NOERROR &&
     output.error() == AULIB_NOERROR) {
    for(int i=0; i < def_sr*10; i+=def_vsize){
      phase.process();
      sig.process(phase.output());
      output.write(sig.output());
    }
  }
  return 0;
}
