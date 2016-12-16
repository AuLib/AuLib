/////////////////////////////////////////////////////////////////////
// Implementation of the Oscilic class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Oscilic.h"

void
AuLib::Oscilic::oscillator(){
  uint32_t phi;
  double frac,a,b,c,d;
  double tmp, fracsq, fracb;
  for(int i=0; i < m_vframes; i++){
    am_fm(i);
    phi = (uint32_t) m_phs;
    frac = m_phs - phi;
    a = phi == 0 ?  m_table[0] : m_table[phi-1];
    b = m_table[phi];
    c = m_table[phi+1];
    d =  m_table[phi+2];    
    tmp = d + 3.f*b;
    fracsq = frac*frac;
    fracb = frac*fracsq;
    m_vector[i] = m_amp*
      (fracb*(- a - 3.f*c + tmp)/6.f +
       fracsq*((a+c)/2.f - b) +
       frac*(c + (-2.f*a - tmp)/6.f) + b);
    m_phs += m_incr;
    mod();
  }
}

