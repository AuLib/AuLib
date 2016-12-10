/////////////////////////////////////////////////////////////////////
// Function table reader (interpolating)
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
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
  TableReadi(const double *table, double phase = 0.,
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
