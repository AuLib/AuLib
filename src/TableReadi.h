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

namespace AuLib {

  /** Table reader with linear interpolation
   */
  class TableReadi : public TableRead {

  protected:

    /** truncated table lookup
     */
    virtual void lookup(const double *phs);
    
  public:
    /** TableReadi constructor \n\n
	ftable - function table \n
	phase - initial phase \n
	norm - normalisation switch \n
	wrap - wraparound switch \n
	vframes - vector size \n
    */
    TableReadi(const FuncTable& ftable, double phase = 0.,
	       bool norm = true, bool wrap = true,
	       uint32_t vframes = def_vframes) :
      TableRead(ftable,phase,norm,wrap,vframes) { };

  };

  /*! \class TableReadi TableReadi.h AuLib/TableReadi.h
   */
  
}
#endif
