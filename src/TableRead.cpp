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

void
AuLib::TableRead::lookup(const double *phs){
  uint32_t pos;
  for(int i=0; i < m_vframes; i++){   
    pos = (uint32_t) mod((m_norm ?
	       phs[i]*m_tsize : phs[i])
	      + m_phs);
    m_vector[i] = m_table[pos];
  }
}
