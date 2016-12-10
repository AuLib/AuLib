///////////////////////////////////////////////////////////////////
// Classic wavetables based on FourierTable
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// -*- mode: c++;-*-
////////////////////////////////////////////////////////////////////
#ifndef _WAVETABLES_H
#define _WAVETABLES_H

#include "FourierTable.h"

namespace AuLib {
  
  /** Sawtooth wave table
   */
  struct SawTable : public FourierTable {
    /** SawTable constructor \n\n
	harms - number of harmonics \n
	tsize - table size \n
    */
    SawTable(uint32_t harms,
	     uint32_t tsize=def_tsize) :
      FourierTable(harms,SAW_TABLE,tsize){};
  };

  /** Square wave table
   */
  struct SquareTable : public FourierTable {
    /** SquareTable constructor \n\n
	harms - number of harmonics \n
	tsize - table size \n
    */ 
    SquareTable(uint32_t harms,
		uint32_t tsize=def_tsize) :
      FourierTable(harms, SQUARE_TABLE, tsize){};
  };

  /** Triangle wave table
   */
  struct TriangleTable : public FourierTable {
    /** TriangleTable constructor \n\n
	harms - number of harmonics \n
	tsize - table size \n
    */
    TriangleTable(uint32_t harms,
		  uint32_t tsize=def_tsize) :
      FourierTable(harms, TRIANGLE_TABLE, tsize){};
  };

}
#endif
