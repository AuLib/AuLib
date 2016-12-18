/////////////////////////////////////////////////////////////////////
// ToneHP class: first-order high-pass filter
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TONEHP_H_
#define _TONEHP_H_
#include "ToneLP.h"

namespace AuLib {

  /** Simple first-order high-pass filter
   */
  class ToneHP : public ToneLP {

  protected:
    
    /** Update filter coefficients
     */
    virtual void update();
    
  public:
    /** ToneHP constructor \n\n
	cf - cutoff frequency \n
	vframes - vector size \n
        sr - sampling rate
    */  
    ToneHP(double cf, uint32_t vframes = def_vframes,
	   double sr = def_sr) :
      ToneLP(cf,vframes,sr) {
      update();
    }
    
  };

  /*! \class ToneLP ToneLP.h AuLib/ToneLP.h
   */
}
#endif
