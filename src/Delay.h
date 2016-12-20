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

  /** Fixed or variable delay line with optional feedback 
      (Delay, Comb filter, Flanger)
   */
  class Delay : public AudioBase {

  protected:
    AudioBase m_delay;
    uint32_t m_pos;
    double m_fdb;

  public:
    /** Delay constructor \n\n
	dtime - delay time \n
	vframes - vector size \n
        sr - sampling rate
    */  
    Delay(double dtime, double fdb, uint32_t vframes = def_vframes,
	  double sr = def_sr) : m_fdb(fdb),
				m_pos(0), m_delay(1, dtime*sr, sr),
				AudioBase(1,vframes,sr) { };

    /** delay a signal sig for a fixed time
     */
    virtual const double* process(const double* sig); 

    /** delay a signal for dt seconds and with optional feedback fdb
     */
    virtual const double* process(const double* sig, double dt, double fdb=0.);

    /** delay a signal for delay time taken from the signal dt and 
	with optional feedback fdb 
    */
    virtual const double* process(const double* sig, const double* dt, double fdb=0.);


    /** delay a signal in obj for a fixed time
     */
    virtual const Delay& process(const AudioBase& obj) {
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls) {
	  process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }
    
    /** delay a signal in obj, optionally for dt seconds 
        and with optional feedback fdb.
     */
    virtual const Delay& process(const AudioBase& obj, double dt, double fdb=0.) {
      m_fdb = fdb;
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls) {
	if(dt < 0)
	  process(obj.vector());
	else
	  process(obj.vector(), dt); 
      } else m_error = AULIB_ERROR;
      return *this;
    }
    
    /** delay a signal in obj for dt sec with variable delaytime sig
	and with optional feedback fdb.
     */
    virtual const Delay& process(const AudioBase& obj, const AudioBase& dt,
				 double fdb=0.) {
      m_fdb = fdb;
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls &&
	 dt.vframes() == m_vframes &&
	 dt.nchnls() == m_nchnls) {
	process(obj.vector(), dt.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }

  };

  /*! \class Delay Delay.h AuLib/Delay.h
   */

}
#endif
