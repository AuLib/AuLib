////////////////////////////////////////////////////////////////////
// Function table reader
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TABLEREAD_H
#define _TABLEREAD_H
#include "AudioBase.h"
#include "FuncTable.h"

namespace AuLib {

  /** Table reader with truncated lookup
   */
  class TableRead : public AudioBase {

  protected:
    double m_phs;
    bool  m_wrap;
    bool  m_norm;
    uint32_t m_tsize;
    const double *m_table;

    double mod(double pos){
      if(m_wrap) {
	while(pos >= m_tsize) pos -= m_tsize;
	while(pos < 0) pos += m_tsize;
      } else {
	pos = pos < 0 ? 0. :
	  (pos >= m_tsize ? m_tsize : pos); 
      }
      return pos;
    }

  public:
    /** TableRead constructor \n\n
	table - function table \n
	phase - initial phase \n
	norm - normalisation switch \n
	wrap - wraparound switch \n
	vsize - vector size \n
    */
    TableRead(const FuncTable& ftable, double phase = 0.,
	      bool norm = true, bool wrap = true,
	      uint32_t vsize = def_vsize):
      m_table(ftable.table()), m_phs(phase),
      m_norm(norm), m_wrap(wrap),
      m_tsize(ftable.size()), AudioBase(1,vsize)
    {
      if(m_table == NULL) {
	m_vsize = 0;
	m_error = AULIB_ERROR;
      } 
    };
  
    /** takes in a frame of phase values
	and lookups up the table values
    */
    virtual void process(const double* phs);

    /** takes in a frame of phase values
	and lookups up the table values
    */
    virtual void process(const AudioBase& obj){
      if(obj.vsize() == m_vsize &&
	 obj.nchnls() == 1)
	process(obj.output());
      else m_error = AULIB_ERROR;
    }

  };

  /*! \class TableRead TableRead.h AuLib/TableRead.h
   */
  
}
#endif
