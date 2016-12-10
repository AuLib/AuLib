/////////////////////////////////////////////////////////////////////
// AudioBase class: dsp base class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _AUDIOBASE_H
#define _AUDIOBASE_H

#include "AuLib.h"
#include <iostream>

/** Audio DSP base class
 */
class AudioBase {

 protected:
  uint32_t m_nchnls;
  double m_sr;
  uint32_t m_vsize;
  double *m_output;

 public:
  /** AudioBase constructor \n\n
      nchnls - number of channels \n
      sr - sampling rate \n
      vsize - vector size (frames) \n
  */
  AudioBase(uint32_t nchnls = def_nchnls,
	   double sr = def_sr,
	   double vsize = def_vsize) :
    m_nchnls(nchnls), m_sr(sr),
    m_vsize(vsize) {
    m_output = new double[m_vsize*m_nchnls];
    
  }
  
  /** AudioBase destructor
   */
  virtual ~AudioBase(){
    delete[] m_output;
  }

  /** Get the audio output vector
   */ 
  double *output(){
    return m_output;
  }
  
  /** Get a single sample at ndx 
      off the output audio vector
  */  
  double output(uint32_t ndx){
    return m_output[ndx];
  }
  
};
#endif
