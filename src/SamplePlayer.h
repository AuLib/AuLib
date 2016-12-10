////////////////////////////////////////////////////////////////////
// SamplePlayer class: sampled-sound player
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SAMPLEPLAYER_H
#define _SAMPLEPLAYER_H

#include "Oscili.h"

namespace AuLib {

  /** Sample playback oscillator 
      with linear interpolation
  */
  class SamplePlayer : public Oscili {

  public:
    /** SamplePlayer constructor \n\n
	amp - amplitude \n
	pitch - playback pitch \n
	phase - init phase (0-1) \n 
	table - function table \n
	tsize - table size \n
	sr - sampling rate \n
	vsize - vector size \n
    */
    SamplePlayer(double amp = 0., double pitch = 0.,
		 double phase = 0.,
		 const double *table = NULL,
		 uint32_t tsize = def_tsize,
		 uint32_t vsize = def_vsize, 
		 double sr = def_sr) :
      Oscili(amp,pitch*sr/tsize,phase,table,
	     tsize,vsize,sr) { };


    /** Process one vector of audio
     */
    virtual void process() {
      Oscili::process();
    }

    /** Process one vector of audio
	with amplitude amp
    */
    virtual void process(double amp) {
      Oscili::process(amp);
    }


    /** Process one vector of audio
	with amplitude amp and
	pitch transposition
    */
    virtual void process(double amp, double pitch){
      m_amp = amp;
      m_freq = pitch*m_sr/m_tsize;
      m_incr = pitch;
      Oscili::process();
    }
  
  };

}
#endif
