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
		    double phase, uint32_t vframes,
		    double sr) :
  m_amp(amp), m_freq(freq), m_sine(1),
  m_phs(phase), m_tsize(def_tsize),
  m_table(NULL), m_am(NULL),
  m_fm(NULL), AudioBase(1,vframes,sr)
{
  m_table = m_sine.table();
  m_incr = m_freq*m_tsize/m_sr;
  m_phs *= m_tsize;
  mod();
}

AuLib::Oscil::Oscil(double amp, double freq,
		    const FuncTable& ftable, double phase,
		    uint32_t vframes, double sr) :
  m_amp(amp), m_freq(freq),
  m_phs(phase), m_tsize(ftable.tsize()),
  m_table(ftable.table()),
  m_am(NULL), m_fm(NULL),
  m_sine(0,NULL,0.,0),
  AudioBase(1,vframes,sr)
{
  if(m_table == NULL){
    m_vframes = 0;
    m_error = AULIB_ERROR;
    return;
  }
  m_incr = m_freq*m_tsize/m_sr;
  m_phs *= m_tsize;
  mod();
}

void
AuLib::Oscil::oscillator(){
  for(int i=0; i < m_vframes; i++){
    am_fm(i);
    m_vector[i] = m_amp*m_table[(uint32_t)m_phs];
    m_phs += m_incr;
    mod();
  }
}

