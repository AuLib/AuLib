///////////////////////////////////////////////////////////////////
// Implementation of the Oscili class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Oscili.h"

void
AuLib::Oscili::oscillator(){
  uint32_t phi;
  double frac,a,b;
  for(int i=0; i < m_vsize; i++){
    am_fm(i);
    phi = (uint32_t) m_phs;
    frac = m_phs - phi;
    a = m_table[phi];
    b = m_table[phi+1];
    m_vector[i] = 
      m_amp*(a + frac*(b - a));
    m_phs += m_incr;
    mod();
  }
}
