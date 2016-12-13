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
#include <iostream>

using namespace AuLib;
using namespace std;

int main(){
  
  Oscil sig;
  SoundOut output("dac");

  if(sig.error() == AULIB_NOERROR) {
    if(output.error() == AULIB_NOERROR) {
      for(int i=0; i < def_sr*10; i+=def_vsize){
	sig.process(0.5, 440.);
	output.write(sig);
      }
    } else cout << output.error_message() << "\n";
  } else cout << sig.error_message() << "\n";
  
  return 0;
}
