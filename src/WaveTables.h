////////////////////////////////////////////
// Classic wavetables based on FourierTable
// (c) V Lazzarini, 2016-7
// -*- mode: c++;-*-
////////////////////////////////////////////
#ifndef _WAVETABLES_H
#define _WAVETABLES_H

#include "FourierTable.h"

struct SawTable : public FourierTable {
  /** SawTable constructor
      harms - number of harmonics
      tsize - table size
  */
 SawTable(uint32_t harms,
	  uint32_t tsize=def_tsize) :
  FourierTable(harms,SAW_TABLE,tsize){};
};

struct SquareTable : public FourierTable {
  /** SquareTable constructor
      harms - number of harmonics
      tsize - table size
  */ 
 SquareTable(uint32_t harms,
	     uint32_t tsize=def_tsize) :
  FourierTable(harms, SQUARE_TABLE, tsize){};
};

struct TriangleTable : public FourierTable {
  /** TriangleTable constructor
      harms - number of harmonics
      tsize - table size
  */
 TriangleTable(uint32_t harms,
	       uint32_t tsize=def_tsize) :
  FourierTable(harms, TRIANGLE_TABLE, tsize){};
};

#endif
