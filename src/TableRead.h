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
     tsize - table size \n
     vsize - vector size \n
  */
  TableRead(const double *table, double phase = 0.,
	    bool norm = true, bool wrap = true,
	    uint32_t tsize = def_tsize,
	    uint32_t vsize = def_vsize):
  m_table(table), m_phs(phase),
  m_norm(norm), m_wrap(wrap),
  m_tsize(tsize),
    AudioBase(1,def_sr,vsize) {
    if(m_table == NULL) {
      m_vsize = 0;
      m_tsize = 0;
    }
   };
  
  /** takes in a frame of phase values
      and lookups up the table values
  */
  virtual void process(const double *phs);

};
#endif
