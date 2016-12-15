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

    double m_aincrs[2];
    uint32_t m_atimes[2];

    void ads_set(double amp, double att,
		 double dec, double sus) {    
      m_atimes[0] = att*m_sr;
      m_aincrs[0] = amp/m_atimes[0];
      m_atimes[1] = dec*m_sr;
      m_aincrs[1] = (sus - amp)/m_atimes[1];
      m_incrs = m_aincrs;
      m_times = m_atimes;
      m_incr = m_aincrs[0];
      m_time = m_atimes[0];
    }
    
  public:
    /** Adsr constructor \n\n
	amp - amplitude after attack \n
        att - attack time \n
        dec - decay time \n
        rel - release time \n
	vsize - vector size \n
        sr - sampling rate
    */  
    Adsr(double amp, double att, double dec, double sus,
	 double rel,uint32_t vsize = def_vsize, double sr = def_sr) :
      Envel(0.,NULL,NULL,3,rel,true,vsize,sr) {
      ads_set(amp,att,dec,sus);     
    };

    /** reset the envelope parameters and
	retrigger 
    */
    void reset(double amp, double att, double dec,
	       double sus, double rel){
      ads_set(amp,att,dec,sus); 
      retrig();
    }

    /** Main envelope duration (att+dec) in frames
     */
    uint32_t frames() const {
      return m_times[0] +  m_times[1];
    }
    
  };

  /*! \class Adsr Adsr.h AuLib/Adsr.h
   */

}
#endif
