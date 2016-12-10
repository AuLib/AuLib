////////////////////////////////////////////////////////////////////
// Implementation of the Oscil class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Oscil.h"

AuLib::Oscil::Oscil(double amp, double freq,
	     double phase, const double *table,
	     uint32_t tsize,uint32_t vsize,
	     double sr) :
  m_amp(amp), m_freq(freq),
  m_phs(phase), m_tsize(tsize),
  m_table(table), m_sine(false),
  m_am(NULL), m_fm(NULL),
  AudioBase(1,vsize,sr)
{
  m_incr = m_freq*m_tsize/m_sr;
  if(m_table == NULL) {
    double *table = new double[def_tsize+1];
    if(table != NULL) {
      for(int i=0; i < def_tsize; i++)
	table[i] = sin(i*twopi/def_tsize);
      table[def_tsize] = table[0];
      m_table = (const double *) table;
      m_sine = true;
    } else {
      m_tsize = 0;
      m_vsize = 0;  
      m_error = AULIB_MEM_ERROR;
      return;
    }
  }
  m_phs *= m_tsize;
  mod();
}

AuLib::Oscil::~Oscil(){
  if(m_sine)
    delete[] m_table;
}


void
AuLib::Oscil::process(){
  for(int i=0; i < m_vsize; i++){
    am_fm(i);
    m_output[i] = m_amp*m_table[(uint32_t)m_phs];
    m_phs += m_incr;
    mod();
  }
}

