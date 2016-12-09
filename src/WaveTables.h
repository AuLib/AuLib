////////////////////////////////////////////
// Classic wavetables based on FourierTable
// (c) V Lazzarini, 2016-7
// -*- mode: c++;-*-
////////////////////////////////////////////
#ifndef _WAVETABLES_H
#define _WAVETABLES_H

#include "FourierTable.h"

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

#endif
