///////////////////////////////////////////////////////////////////
// Implementation of the TableRead class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "TableRead.h"
#include "TableReadi.h"
#include "TableReadic.h"

void
AuLib::TableRead::lookup(const double *phs){
  uint32_t pos;
  for(uint32_t i=0; i < m_vframes; i++){   
    pos = (uint32_t) mod((m_norm ?
	       phs[i]*m_tframes : phs[i])
	      + m_phs);
    m_vector[i] = m_table[pos];
  }
}

void
AuLib::TableReadi::lookup(const double* phs){
  uint32_t posi;
  double   pos, frac, a, b;
  for(uint32_t i=0; i < m_vframes; i++){   
    pos = mod((m_norm ?
	       phs[i]*m_tframes : phs[i])
	      + m_phs);
    posi = (uint32_t) pos;
    frac = pos - posi;
    a = m_table[posi];
    b = m_table[posi+1];
    m_vector[i] = a + frac*(b - a);
  }
}

void
AuLib::TableReadic::lookup(const double* phs){
  uint32_t posi;
  double   pos, frac, fracsq, fracb;
  double   tmp, a, b, c, d;
  for(uint32_t i=0; i < m_vframes; i++){   
    pos = mod((m_norm ?
	       phs[i]*m_tframes : phs[i])
	      + m_phs);
    posi = (uint32_t) pos;
    frac = pos - posi;
    a = posi > 0 ? m_table[posi-1] :
      m_table[m_tframes-1];
    b = m_table[posi];
    c = m_table[posi+1];
    d = posi < m_tframes ?
	       m_table[posi+2] :
      m_table[posi];   
    tmp = d + 3.f*b;
    fracsq = frac*frac;
    fracb = frac*fracsq;
    m_vector[i] =
	       fracb*(- a - 3.f*c + tmp)/6.f +
	       fracsq*((a+c)/2.f - b) +
	       frac*(c + (-2.f*a - tmp)/6.f) + b;
  }
}
