/////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include <TableReadi.h>
#include <Oscili.h>
#include <SoundOut.h>
#include <WaveTables.h>
#include <Phasor.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main(){
  
  TriangleTable wave(10);
  Phasor   phase(440.);
  //Oscili  sig(0.5,440,wave);
  TableReadi sig(wave);
  SoundOut output("dac");
  cout << Info::version();
  if(wave.error() == AULIB_NOERROR) {
    if(phase.error() == AULIB_NOERROR) {
      if(sig.error() == AULIB_NOERROR) {
	if(output.error() == AULIB_NOERROR) {
	  for(int i=0; i < def_sr*10; i+=def_vframes){
	    phase.process();
	    sig.process(phase);
	    output.write(sig*=0.9);
	  } 
	} else cout << output.error_message() << "\n";
      } else cout << sig.error_message() << "\n";
    } else cout << phase.error_message() << "\n";
  } else cout << wave.error_message() << "\n";
  return 0;
}
