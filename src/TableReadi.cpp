///////////////////////////////////////////////////////////////////
// Implementation of the TableReadi class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "TableReadi.h"

void
AuLib::TableReadi::process(double *phs){
  uint32_t posi;
  double   pos, frac, a, b;
  for(int i=0; i < m_vsize; i++){   
    pos = mod((m_norm ?
	       phs[i]*m_tsize : phs[i])
	      + m_phs);
    posi = (uint32_t) pos;
    frac = pos - posi;
    a = m_table[posi];
    b = m_table[posi+1];
    m_output[i] = a + frac*(b - a);
  } 
}
