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
	tframes - table size \n
    */
    SawTable(uint32_t harms,
	     uint64_t tframes=def_tframes) :
      FourierTable(harms,SAW,tframes){};
  };

  /** Square wave table
   */
  struct SquareTable : public FourierTable {
    /** SquareTable constructor \n\n
	harms - number of harmonics \n
	tframes - table size \n
    */ 
    SquareTable(uint32_t harms,
		uint64_t tframes=def_tframes) :
      FourierTable(harms, SQUARE, tframes){};
  };

  /** Triangle wave table
   */
  struct TriangleTable : public FourierTable {
    /** TriangleTable constructor \n\n
	harms - number of harmonics \n
	tframes - table size \n
    */
    TriangleTable(uint32_t harms,
		  uint64_t tframes=def_tframes) :
      FourierTable(harms, TRIANGLE, tframes){};
  };

  
  /*! \class SawTable WaveTables.h AuLib/WaveTables.h
   */
  /*! \class SquareTable WaveTables.h AuLib/WaveTables.h
   */
  /*! \class TriangleTable WaveTables.h AuLib/WaveTables.h
   */

}
#endif
