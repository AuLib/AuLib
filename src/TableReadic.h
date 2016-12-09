////////////////////////////////////////////
// Function table reader (cubic
//  interpolating)
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _TABLEREADIC_H
#define _TABLEREADIC_H
#include "TableRead.h"

class TableReadic : public TableRead {

 public:
  /** TableReadic constructor
     table - function table
     phase - initial phase
     norm - normalisation switch
     wrap - wraparound switch
     tsize - table size
     vsize - vector size
  */
  TableReadic(double *table, double phase = 0.,
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
