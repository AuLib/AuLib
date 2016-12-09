////////////////////////////////////////////
// FourierTable class: Fourier series tables
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _FOURIERTABLE_H
#define _FOURIERTABLE_H

#include "FuncTable.h"

/** Table type constants
 */
enum {SAW_TABLE=1,
      SQUARE_TABLE,
      TRIANGLE_TABLE};

/** Function tables based on
    Fourier Series
*/
class FourierTable : public FuncTable {

 protected:
  /** Create the table
   */ 
  void create(uint32_t harms,
	      double *amps,
	      double phase);

 public:
  /** FourierTable constructor \n\n
      harms - number of harmonics \n
      amps - array of harmonic amplitudes \n
      phase - phase offset \n
      tsize - table size \n
  */
  FourierTable(uint32_t harms,
	       double *amps=NULL,
	       double phase=0.,
	       uint32_t tsize = def_tsize);
  
  /** FourierTable constructor \n\n
      harms - number of harmonics \n
      type - wave type \n
      tsize - table size \n
  */
  FourierTable(uint32_t harms,
	       uint32_t type = SAW_TABLE,
	       uint32_t tsize = def_tsize);

};

#endif
