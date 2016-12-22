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
#include <SoundIn.h>
#include <iostream>

using namespace AuLib;
using namespace std;

int main(int argc, const char **argv){
  if(argc > 1){
  SoundIn input(argv[1]);
  SoundOut output("dac", input.nchnls());
  cout << Info::version();
  if(input.error() == AULIB_NOERROR) {
      if(output.error() == AULIB_NOERROR) {
	for(uint64_t i=0;i < input.dur();i+=def_vframes){
	  input.read();
	  output.write(input);
	}
      } else cout << output.error_message() << "\n";
    } else cout << input.error_message() << " \n";
  cout << "wrote samples to " << output.dest() << "\n";
  } else cout << "usage: " << argv[0] << " filename\n";
  return 0;
}
