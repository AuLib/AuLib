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
#include <algorithm>
#include <utility>

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
    /** swap function for copy assignment 
     */
    friend void swap(FuncTable& obja,
		     FuncTable& objb) 
    {
      using std::swap;
      swap(obja.m_tsize,objb.m_tsize);
      swap(obja.m_table,objb.m_table);
      swap(obja.m_error,objb.m_error);
    }

    /* Copy assignment operator
     */
    FuncTable& operator=(FuncTable obj){ 
      swap(*this, obj);
      return *this;
    }
    
    FuncTable(const FuncTable& obj) :
      m_tsize(obj.m_tsize), m_error(obj.m_error)
    {
      if(m_tsize > 0) {
	try {
	  m_table = new double[m_tsize+1];
	} catch (std::bad_alloc) {
	  m_tsize = 0;
	  m_error = AULIB_MEM_ERROR;
	  m_table = NULL;
	  return;
	} 
	memcpy(m_table,obj.m_table,sizeof(double)*(m_tsize+1));
      } else m_table = NULL;
    }
    
    /** FuncTable constructor \n\n
	tsize - table size \n
    */ 
    FuncTable(uint32_t tsize = def_tsize)
      : m_tsize(tsize), m_error(AULIB_NOERROR) {
      if(m_tsize > 0) {
	try {
	  m_table = new double[m_tsize+1];
	} catch (std::bad_alloc) {
	  m_tsize = 0;
	  m_error = AULIB_MEM_ERROR;
	  return;
	} 
	memset(m_table,0,sizeof(double)*(m_tsize+1));
      } else m_table = NULL;
    }
    ~FuncTable(){
      if(m_table)
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
