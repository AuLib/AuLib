/////////////////////////////////////////////////////////////////////
// ToneLP class: first-order low-pass filter
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TONELP_H_
#define _TONELP_H_
#include "AudioBase.h"

namespace AuLib {

  /** Simple first-order low-pass filter
   */
  class ToneLP : public AudioBase {

  protected:
    double m_freq;
    double m_del;
    double m_a;
    double m_b;

    /** Update filter coefficients
     */
    virtual void update();
    
    /** filter kernel
     */
    virtual const double* filter(const double* sig){
      for(uint32_t i = 0; i < m_vframes; i++)
	m_vector[i] = m_del = m_a*sig[i] - m_b*m_del;
      return vector();
    }

    
  public:
    /** ToneLP constructor \n\n
	cf - cutoff frequency \n
	vframes - vector size \n
        sr - sampling rate
    */  
    ToneLP(double cf, uint32_t vframes = def_vframes,
	 double sr = def_sr) :
      AudioBase(1,vframes,sr), 
      m_freq(cf), m_del(0.0), m_a(0.0), m_b(0.0) {
      update();
    };

    /** process a signal sig 
     */
    virtual const double* process(const double* sig){
         return filter(sig);
    }

    /** process a signal sig with cutoff freq cf
     */
    virtual const double* process(const double* sig, double cf) {
      if(cf != m_freq){
	m_freq = cf;
	update();
      }
      return process(sig);
    }

    /** process a signal in obj
     */
    virtual const ToneLP& process(const AudioBase& obj) {
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls) {
	process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** process a signal in obj with cutoff freq cf
     */
    virtual const ToneLP& process(const AudioBase& obj, double cf) {
      if(cf != m_freq){
	m_freq = cf;
	update();
      }
      process(obj);
      return *this;
    }    

  };

  /*! \class ToneLP ToneLP.h AuLib/ToneLP.h
   */

}
#endif
