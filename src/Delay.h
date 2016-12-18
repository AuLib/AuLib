/////////////////////////////////////////////////////////////////////
// Delay class: Delay line
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _DELAY_H_
#define _DELAY_H_
#include "AudioBase.h"

namespace AuLib {

  /** Simple fixed-time delayline
   */
  class Delay : public AudioBase {

  protected:
    AudioBase m_delay;
    uint32_t m_pos;

  public:
    /** Delay constructor \n\n
	dtime - delay time \n
	vframes - vector size \n
        sr - sampling rate
    */  
  Delay(double dtime, uint32_t vframes = def_vframes,
	double sr = def_sr) :
       m_pos(0), m_delay(1, dtime*sr, sr),
       AudioBase(1,vframes,sr) { };

    /** process a signal sig 
     */
    virtual const double* process(const double* sig); 

    /** process a signal in obj
     */
    virtual const Delay& process(const AudioBase& obj) {
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls) {
	process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }
    
  };

  /*! \class Delay Delay.h AuLib/Delay.h
   */

}
#endif
