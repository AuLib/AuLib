/////////////////////////////////////////////////////////////////////
// Function table reader (cubic)
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TABLEREADIC_H
#define _TABLEREADIC_H
#include "TableRead.h"

namespace AuLib {

  /** Table reader with cubic interpolation
   */
  class TableReadic : public TableRead {

  public:
    /** TableReadic constructor \n\n
	ftable - function table \n
	phase - initial phase \n
	norm - normalisation switch \n
	wrap - wraparound switch \n
	vsize - vector size \n
    */
    TableReadic(const FuncTable& ftable, double phase = 0.,
		bool norm = true, bool wrap = true,
		uint32_t tsize = def_tsize,
		uint32_t vsize = def_vsize) :
      TableRead(ftable,phase,norm,wrap,vsize) { };

    /** takes in a frame of phase values
	and lookups up the table values
    */
    virtual const double* process(const double* phs);

    /** takes in a frame of phase values
	and lookups up the table values
    */
    virtual const TableReadic& process(const AudioBase& obj){
      if(obj.vsize() == m_vsize &&
	 obj.nchnls() == 1)
	process(obj.output());
      else m_error = AULIB_ERROR;
      return *this;
    }
    
  };

  /*! \class TableReadic TableReadic.h AuLib/TableReadic.h
   */

}
#endif
