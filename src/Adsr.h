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

public:
  /** Adsr constructor \n\n
      amp - amplitude after attack \n
      att - attack time \n
      dec - decay time \n
      rel - release time \n
      vframes - vector size \n
      sr - sampling rate
  */
  Adsr(double amp, double att, double dec, double sus, double rel,
       uint32_t vframes = def_vframes, double sr = def_sr)
      : Envel(rel, vframes, sr) {
    m_segs = Segments(0., {amp, sus}, {att, dec});
    m_time = m_segs.durs()[0];
    m_incr = m_segs.incrs()[0];
    m_y = m_segs.start();
  };

  /** reset the envelope parameters and
      retrigger
  */
  void reset(double amp, double att, double dec, double sus, double rel) {
    m_segs.reset(m_y, {amp, sus}, {att, dec}, m_sr);
    m_rt = rel * m_sr;
    retrig();
  }
};

/*! \class Adsr Adsr.h AuLib/Adsr.h
 */
}
#endif
