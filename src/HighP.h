/////////////////////////////////////////////////////////////////////
// Highp class: High-pass filter using Butterworth design
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _HIGHP_H_
#define _HIGHP_H_
#include "LowP.h"

namespace AuLib {

  /**  2nd-order Butterworth high-pass filter
   */
  class HighP : public LowP {

  protected:

    /** Coefficients update
     */    
    virtual void update();
    
  public:
    
    /** Highp constructor \n\n
	cf - cutoff frequency \n
	vframes - vector size \n
        sr - sampling rate
    */  
    HighP(double cf, uint32_t vframes = def_vframes, double sr = def_sr) :
      LowP(cf,vframes,sr) {
      update();
	};
  };

  /*! \class HighP HighP.h AuLib/HighP.h
   */

}
#endif
