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


AuLib::Oscil::Oscil(const Oscil& obj) :
  m_amp(obj.m_amp), m_freq(obj.m_freq),
  m_phs(obj.m_phs), m_tsize(obj.m_tsize),
  m_table(obj.m_table), m_am(obj.m_am),
  m_fm(obj.m_fm), m_incr(obj.m_incr),
  AudioBase(obj)
{
  if(obj.m_sine) {
    try {
      m_sine = new double[m_tsize+1];
    } catch (std::bad_alloc){
      m_tsize = 0;
      m_vsize = 0;  
      m_error = AULIB_MEM_ERROR;
      return;
    }
    memcpy(m_sine,obj.m_sine,
	   sizeof(double)*(m_tsize+1));
    m_table = (const double *) m_sine;
  }
}


AuLib::Oscil::Oscil(double amp, double freq,
		    double phase, uint32_t vsize,
		    double sr) :
  m_amp(amp), m_freq(freq),
  m_phs(phase), m_tsize(def_tsize),
  m_table(NULL), m_sine(NULL),
  m_am(NULL), m_fm(NULL),
  AudioBase(1,vsize,sr)
{
  try {
    m_sine = new double[m_tsize+1];
  } catch (std::bad_alloc){
    m_tsize = 0;
    m_vsize = 0;  
    m_error = AULIB_MEM_ERROR;
    return;
  }
  for(int i=0; i < def_tsize; i++)
    m_sine[i] = sin(i*twopi/def_tsize);
  m_sine[m_tsize] = m_sine[0];
  m_table = (const double *) m_sine;
  m_incr = m_freq*m_tsize/m_sr;
  m_phs *= m_tsize;
  mod();
}
AuLib::Oscil::Oscil(double amp, double freq,
		    const FuncTable& ftable, double phase,
		    uint32_t vsize, double sr) :
  m_amp(amp), m_freq(freq),
  m_phs(phase), m_tsize(ftable.size()),
  m_table(ftable.table()),m_sine(NULL),
  m_am(NULL), m_fm(NULL),
  AudioBase(1,vsize,sr)
{
  if(m_table == NULL){
    m_vsize = 0;
    m_error = AULIB_ERROR;
    return;
  }
  m_incr = m_freq*m_tsize/m_sr;
  m_phs *= m_tsize;
  mod();
}

AuLib::Oscil::~Oscil(){
  if(m_sine)
    delete[] m_sine;
}


void
AuLib::Oscil::oscillator(){
  for(int i=0; i < m_vsize; i++){
    am_fm(i);
    m_output[i] = m_amp*m_table[(uint32_t)m_phs];
    m_phs += m_incr;
    mod();
  }
}

