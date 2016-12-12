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
#include <Pan.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main(){
  
  Oscili sig;
  Pan  panner;
  SoundOut output("dac", 2);
  const uint32_t end = def_sr*10;

  if(sig.error() == AULIB_NOERROR) {
    if(output.error() == AULIB_NOERROR) {
      for(int i=0; i < end; i+=def_vsize){
	sig.process(0.5, 440.);
	panner.process(sig, double(i)/end);
	output.write(panner);
      }
    } else cout << output.error_message() << "\n";
  } else cout << sig.error_message() << "\n";
  
  return 0;
}
