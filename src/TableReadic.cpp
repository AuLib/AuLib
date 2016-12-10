////////////////////////////////////////////////////////////////////
// Implementation of the TableReadic class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "TableReadic.h"

void
TableReadic::process(double *phs){
  uint32_t posi;
  double   pos, frac, fracsq, fracb;
  double   tmp, a, b, c, d;
  for(int i=0; i < m_vsize; i++){   
    pos = mod((m_norm ?
	       phs[i]*m_tsize : phs[i])
	      + m_phs);
    posi = (uint32_t) pos;
    frac = pos - posi;
    a = posi > 0 ? m_table[posi-1] :
      m_table[m_tsize-1];
    b = m_table[posi];
    c = m_table[posi+1];
    d = posi < m_tsize ?
      m_table[posi+2] :
      m_table[posi];   
    tmp = d + 3.f*b;
    fracsq = frac*frac;
    fracb = frac*fracsq;
    m_output[i] =
       fracb*(- a - 3.f*c + tmp)/6.f +
       fracsq*((a+c)/2.f - b) +
       frac*(c + (-2.f*a - tmp)/6.f) + b;
  } 
}
