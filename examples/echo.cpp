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
#include <Comb.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv){
  if(argc > 1){
    SampleTable wave(argv[1]);
    SamplePlayer sig(wave);
    Comb del(.5, 0.5);
    SoundOut output("dac");
    cout << Info::version();
    if(wave.error() == AULIB_NOERROR) {
      if(del.error() == AULIB_NOERROR) {
	if(sig.error() == AULIB_NOERROR) {
	  if(output.error() == AULIB_NOERROR) { 
	    for(int i=0; i < def_sr*10; i+=def_vframes){
	      sig.process(0.5,1.);
	      del.process(sig);
	      output.write(sig+=del);
	    }
	  } else cout << output.error_message() << "\n";
	} else cout << del.error_message() << "\n";
      } else cout << sig.error_message() << "\n";
    } else cout << wave.error_message() << "\n";
  } else cout << "usage: " << argv[0] << " filename\n";
  return 0;
}