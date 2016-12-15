/////////////////////////////////////////////////////////////////////
// Line class: linear signal generator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _EXPON_H_
#define _EXPON_H_
#include "Line.h"

namespace AuLib {

  /** Generates a signal based on an exponential curve between two
      points over a given duration.
  */
  class Expon : public Line {

  protected:
    virtual void generate(){
      for(int i=0; i < m_vsize; i++){
	m_vector[i] = m_y;
	if(m_cnt < m_x1) {
	  m_y *= m_incr;
	  m_cnt++;
	}
      }
    }
  
  public:
    /** Expon constructor \n\n
        start - start value \n
        end - end value \n
        time - duration(s) \n
	vsize - vector size \n
        sr - sampling rate
    */  
    Expon(double start = db_min, double end = 1. ,
	  double time = 1., uint32_t vsize = def_vsize,
	  double sr = def_sr) :
      Line(start,end,time,vsize,sr) {
      retrig();       
    };

    /** retrigger 
     */
    virtual void retrig(){
      m_cnt = 0;
      if(m_y <= 0.) m_y = db_min;
      if(m_y0 <= 0.) m_y0 = db_min;
      if(m_y1 <= 0.) m_y1 = db_min;
      m_incr = pow(m_y1/m_y0,1./m_x1); 
    }
  };

  /*! \class Expon Expon.h AuLib/Expon.h
   */

}
#endif
