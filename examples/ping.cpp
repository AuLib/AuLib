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
#include <Expon.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main(){
  
  Oscil sig(0.,1000.);
  Expon  env(.001,1.,.01);
  SoundOut output("dac");

  if(env.error() == AULIB_NOERROR) {
    if(sig.error() == AULIB_NOERROR) {
      if(output.error() == AULIB_NOERROR) {
	for(int i=0; i < def_sr*.01; i+=def_vsize){
	  sig.process(env.process());
	  output.write(sig);
	}
	env.reset(1.,.001, 1.5);
	for(int i=0; i < def_sr*1.5; i+=def_vsize){
	  sig.process(env.process());
	  output.write(sig);
	}
      } else cout << output.error_message() << "\n";
    } else cout << sig.error_message() << "\n";
  } else cout << env.error_message() << "\n";
  return 0;
}
