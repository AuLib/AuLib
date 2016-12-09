////////////////////////////////////////////
// Function table reader (interpolating)
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _TABLEREADI_H
#define _TABLEREADI_H
#include "TableRead.h"

/** Table reader with linear interpolation
 */
class TableReadi : public TableRead {

 public:
  /** TableReadi constructor \n\n
     table - function table \n
     phase - initial phase \n
     norm - normalisation switch \n
     wrap - wraparound switch \n
     tsize - table size \n
     vsize - vector size \n
  */
  TableReadi(double *table, double phase = 0.,
	    bool norm = true, bool wrap = true,
	    uint32_t tsize = def_tsize,
	     uint32_t vsize = def_vsize) :
  TableRead(table,phase,norm,wrap,
	    tsize, vsize) { };

  /** takes in a frame of phase values
      and lookups up the table values
  */
  virtual void process(double *phs);

};
#endif
