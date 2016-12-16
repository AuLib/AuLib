/////////////////////////////////////////////////////////////////////
// Adsr class: Attack-Decay-Sustain-Release envelope
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _ADSR_H_
#define _ADSR_H_
#include "Envel.h"

namespace AuLib {

  /** Adsr description
   */
  class Adsr : public Envel {
    
  public:
    /** Adsr constructor \n\n
	amp - amplitude after attack \n
        att - attack time \n
        dec - decay time \n
        rel - release time \n
	vframes - vector size \n
        sr - sampling rate
    */  
    Adsr(double amp, double att, double dec, double sus,
	 double rel, uint32_t vframes = def_vframes, double sr = def_sr) :
      Envel(rel,vframes,sr) {
      double amps[2] = {amp, sus};
      double times[2] = {att, dec};
      m_segs = Segments(0.,amps,times,2);     
    };

    /** reset the envelope parameters and
	retrigger 
    */
    void reset(double amp, double att, double dec,
	       double sus, double rel){
      double amps[2] = {amp, sus};
      double times[2] = {att, dec};
      m_segs = Segments(0.,amps,times,2);  
      retrig();
    }
    
  };

  /*! \class Adsr Adsr.h AuLib/Adsr.h
   */

}
#endif
