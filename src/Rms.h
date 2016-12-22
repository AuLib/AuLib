/////////////////////////////////////////////////////////////////////
// Rms class: root-mean-square estimation
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _RMS_H_
#define _RMS_H_
#include "ToneLP.h"

namespace AuLib {

  /** Estimates the root-means-square of a signal
   */
  class Rms : public ToneLP {

  protected:

    /** rectification
     */
    double rect(double x) {
      return x < 0.0 ? -x : x;
    }

     /** RMS processing
     */
    virtual const double* filter(const double* sig){
      for(uint32_t i = 0; i < m_vframes; i++)
	m_vector[i] = m_del = m_a*rect(sig[i]) - m_b*m_del;
      return vector();
    }
    
  public:
    /** Rms constructor \n\n
	cf - cutoff frequency \n
	vframes - vector size \n
        sr - sampling rate
    */  
    Rms(double cf=10., uint32_t vframes = def_vframes,
	   double sr = def_sr) :
      ToneLP(cf,vframes,sr) {
    }

   
  };

  /*! \class Rms Rms.h AuLib/Rms.h
   */
}
#endif
