////////////////////////////////////////////////////////////////////
// Oscil class: truncating oscillator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _OSCIL_H
#define _OSCIL_H

#include "AudioBase.h"

/** Truncating oscillator
 */
class Oscil : public AudioBase {

protected:
  
  double m_phs;  
  double m_freq;
  double m_amp;
  double m_incr;
  uint32_t m_tsize;
  const double *m_table;
  bool m_sine;
  
  /** phase modulo
   */
  void mod(){
    while(m_phs >= m_tsize) m_phs -= m_tsize;
    while(m_phs < 0) m_phs += m_tsize;
  }

public:
  /** Oscil constructor \n\n
      amp - amplitude   \n
      freq - frequency in Hz \n
      phase - init phase (0-1) \n 
      table - function table \n
      tsize - table size \n
      sr - sampling rate \n
      vsize - vector size \n
  */
  Oscil(double amp, double freq,
	double phase = .0,
	const double *table = NULL,
	uint32_t tsize = def_tsize,
	uint32_t vsize = def_vsize,
	double sr = def_sr);

  /** Oscil destructor
   */
  virtual ~Oscil();

  /** Process one vector of audio
   */
  virtual void process();
  
  /** Process one vector of audio
      with amplitude amp
  */
  virtual void process(double amp) {
    m_amp = amp;
    process();
  }

  /** Process one vector of audio
      with amplitude amp and
      frequency freq
  */
  virtual void process(double amp, double freq){
    m_amp = amp;
    m_freq = freq;
    m_incr = m_freq*m_tsize/m_sr;
    process();
  }

};

#endif
