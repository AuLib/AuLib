////////////////////////////////////////////
// FourierTable class: Fourier series tables
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _FOURIERTABLE_H
#define _FOURIERTABLE_H

#include "FuncTable.h"

enum {SAW_TABLE=1,
      SQUARE_TABLE,
      TRIANGLE_TABLE};

class FourierTable : public FuncTable {

 protected:
  /** Create the table
   */ 
  void create(uint32_t harms,
	      double *amps,
	      double phase);

 public:
  /** FourierTable constructor
      harms - number of harmonics
      amps - array of harmonic amplitudes
      phase - phase offset
      tsize - table size
  */
  FourierTable(uint32_t harms,
	       double *amps=NULL,
	       double phase=0.,
	       uint32_t tsize = def_tsize);
  
  /** FourierTable constructor
      harms - number of harmonics
      type - wave type 
      tsize - table size
  */
  FourierTable(uint32_t harms,
	       uint32_t type = SAW_TABLE,
	       uint32_t tsize = def_tsize);

};

#endif
