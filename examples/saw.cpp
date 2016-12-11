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
#include <WaveTables.h>
#include <Oscilic.h>

using namespace AuLib;

int main(){
  
  SawTable wave(50);
  Oscilic sig(0.5, 440.,wave);
  SoundOut output("dac");

  if(wave.error() == AULIB_NOERROR &&
     sig.error() == AULIB_NOERROR &&
     output.error() == AULIB_NOERROR) {
    for(int i=0; i < def_sr*10; i+=def_vsize){
      sig.process();
      output.write(sig);
    }
  }
  return 0;
}
