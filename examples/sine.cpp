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

int main(int argc, const char **argv){
  if(argc > 1){
  Oscil sig;
  SoundOut output(argv[1]);
  cout << Info::version();
  if(sig.error() == AULIB_NOERROR) {
      if(output.error() == AULIB_NOERROR) {
	for(int i=0;i < def_sr*2;i+=def_vframes){
	  sig.process(0.5, 440.);
	  output.write(sig);
	}
      } else cout << output.error_message() << "\n";
    } else cout << sig.error_message() << "\n";
  cout << "wrote " << argv[1] << "\n";
  } else cout << "usage: " << argv[0] << " filename\n";
  return 0;
}
