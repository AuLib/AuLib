/////////////////////////////////////////////////////////////////////
// Implementation of the Envel and Segment classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Envel.h"

AuLib::Segments::Segments(double start,
			  const double *endpts,
			  const double *times,
			  uint32_t nsegs,
			  bool linear,
			  double sr) :
  m_start(start),
  m_nsegs(nsegs),
  m_linear(linear),
  m_frames(0),
  m_incrs(nsegs),
  m_durs(nsegs)
{  
  double y0 = linear ? start :
    start > 0 ? start : (m_start = db_min);
  double y1;
  for(int i=0; i < m_nsegs; i++) {
    y1 = endpts[i];
    m_durs[i] = times[i]*sr;
    m_frames += m_durs[i];
    if(m_durs[i] <= 0)
      m_durs[i] = 1;
    if(m_linear)
      m_incrs[i] = (y1 - y0)/m_durs[i];
    else {
      if(y1 <= 0) y1 = db_min;
      m_incrs[i] = pow((y1/y0), 1./m_durs[i]);
    } 
    y0 = y1;
  }
}

const AuLib::AudioBase&
AuLib::Envel::process() {
  bool linear = m_segs.isLinear();
  uint32_t nsegs = m_segs.nsegs();
  for(int i=0; i < m_vframes; i++) {
    m_vector[i] = m_y;
    if(linear) m_y += m_incr;
    else m_y *= m_incr;
    if(m_cseg >= 0) m_cnt++;
    if(m_trig && m_rt > 0) {
      m_incr = linear ? -m_y/m_rt
	  : pow(m120dBfs/m_y,1./m_rt);
        m_time = m_rt;
	m_cnt = 0;
	m_trig = false;
	m_cseg = nsegs;
    } else
    if(m_cnt == m_time
	    && m_cseg != -1) {
      m_cseg++;
      if(m_cseg < nsegs) {
	m_incr = m_segs.incrs()[m_cseg];
	m_time = m_segs.durs()[m_cseg];
	m_cnt = 0;
      }
      else {
	m_incr = linear ?  0. : 1.;
        m_cseg = -1;
      }
    }
  }
  return *this;
}
