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
#include <SampleTable.h>
#include <SamplePlayer.h>

using namespace AuLib;

int main(int argc, const char **argv){

  if(argc > 1){
    
    SampleTable wave(argv[1]);
    SamplePlayer sig(wave);
    SoundOut output("dac");
  
    if(wave.error() == AULIB_NOERROR &&
       sig.error() == AULIB_NOERROR &&
       output.error() == AULIB_NOERROR) { 
      for(int i=0; i < def_sr*10; i+=def_vsize){
	sig.process(0.5,1.);
	output.write(sig);
      }
    }
  }
  return 0;
}
