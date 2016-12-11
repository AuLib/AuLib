////////////////////////////////////////////////////////////////////
// FuncTable class: function tables
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _FUNCTABLE_H
#define _FUNCTABLE_H

#include "AuLib.h"

namespace AuLib {
  /** Function table base class
   */
  class FuncTable {

  protected:
    double *m_table;
    uint32_t m_tsize;
    uint32_t m_error;
 
    /** Normalise the table
     */ 
    void normalise_table(){
      uint32_t n;
      double max = 0.;
      for(n=0; n < m_tsize+1; n++)
	max = m_table[n] > max ?
	  m_table[n] : max;
      if(max)
	for(n=0; n < m_tsize+1; n++)
	  m_table[n] /= max;
    }

  public:
    /** FuncTable constructor \n\n
	tsize - table size \n
    */ 
    FuncTable(uint32_t tsize = def_tsize)
      : m_tsize(tsize), m_error(AULIB_NOERROR) {
      if(m_tsize > 0) {
	m_table = new double[m_tsize+1];
	if(m_table == NULL) {
	  m_tsize = 0;
	  m_error = AULIB_MEM_ERROR;
	} else 
	  memset(m_table,0,sizeof(double)*(m_tsize+1));
      } else m_table = NULL;
    }
    ~FuncTable(){
      delete[] m_table;
    }

    /** Get the function table
     */ 
    const double* table() const {
      return (const double *) m_table;
    }

    /** Get a single point at pos
	from the function table
    */ 
    double table(uint32_t pos) const {
      return m_table[pos];
    }

    /** Get table size
     */
    uint32_t size() const {
      return m_tsize;
    }

    /** Get error code
     */
    uint32_t error(){
      return m_error;
    }

    /** Get error message
     */
    virtual const char* error_message() const {
      return aulib_error[m_error];
    }
 
  };

  /*! \class FuncTable FuncTable.h AuLib/FuncTable.h
   */

}
#endif
