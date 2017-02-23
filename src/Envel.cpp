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

AuLib::Segments::Segments(double start, const std::vector<double> endpts,
                          const std::vector<double> times, bool linear,
                          double sr)
    : m_start(start), m_nsegs(endpts.size()), m_linear(linear), m_frames(0),
      m_incrs(endpts.size()), m_durs(endpts.size()), m_endpts(endpts.size()) {
  double y0 = linear ? start : start > 0 ? start : (m_start = db_min);
  double y1;
  for (uint32_t i = 0; i < m_nsegs; i++) {
    y1 = m_endpts[i] = endpts[i];
    if (times.size() > i)
      m_durs[i] = times[i] * sr;
    else
      m_durs[i] = 1;
    m_frames += m_durs[i];
    if (m_durs[i] <= 0)
      m_durs[i] = 1;
    if (m_linear)
      m_incrs[i] = (y1 - y0) / m_durs[i];
    else {
      if (y1 <= 0)
        y1 = db_min;
      m_incrs[i] = pow((y1 / y0), 1. / m_durs[i]);
    }
    y0 = y1;
  }
}

void
AuLib::Segments::reset(double start, const std::vector<double> endpts,
		       const std::vector<double> times, double sr) {
  double y0 = m_linear ? start : start > 0 ? start : (m_start = db_min);
  double y1;
  for (uint32_t i = 0; i < m_nsegs; i++) {
    if(i >= endpts.size() ||
       i >= times.size()) break;
    y1 = m_endpts[i] = endpts[i];
    if (times.size() > i)
      m_durs[i] = times[i] * sr;
    else
      m_durs[i] = 1;
    m_frames += m_durs[i];
    if (m_durs[i] <= 0)
      m_durs[i] = 1;
    if (m_linear)
      m_incrs[i] = (y1 - y0) / m_durs[i];
    else {
      if (y1 <= 0)
        y1 = db_min;
      m_incrs[i] = pow((y1 / y0), 1. / m_durs[i]);
    }
    y0 = y1;
  }
}


const AuLib::AudioBase &AuLib::Envel::dsp() {
  bool linear = m_segs.isLinear();
  uint32_t nsegs = m_segs.nsegs();
  for (uint32_t i = 0; i < m_vframes; i++) {
    m_vector[i] = m_y;
    if (m_trig) {
      m_releasing = m_trig;
      m_trig = false;
      m_incr = linear ? -m_y / m_rt : pow(m120dBfs / m_y, 1. / m_rt);
      m_time = m_rt;
      m_cnt = 0;
      m_rcnt = m_rt;
      m_cseg = nsegs;
    }

    if (linear)
      m_y += m_incr;
    else
      m_y *= m_incr;

    if (!m_releasing) {
      if (m_cseg >= 0)
        m_cnt++;
      if (m_cnt == m_time && m_cseg != -1) {
        m_cseg++;
        if ((uint32_t)m_cseg < nsegs) {
          m_y = m_segs.endpts()[m_cseg - 1];
          m_incr = m_segs.incrs()[m_cseg];
          m_time = m_segs.durs()[m_cseg];
          m_cnt = 0;
        } else {
          if ((uint32_t)m_cseg == nsegs)
            m_y = m_segs.endpts()[m_cseg - 1];
          m_incr = linear ? 0. : 1.;
          m_cseg = -1;
        }
      }
    } else {
      if (m_rcnt == 0) {
        m_incr = linear ? 0. : 1.;
        m_y = 0;
	m_done = true;
      } else
        m_rcnt--;
    }
  }

  return *this;
}
