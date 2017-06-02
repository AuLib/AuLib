/////////////////////////////////////////////////////////////////////////////////
// SamplePlayer class: sampled-sound player
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _SAMPLEPLAYER_H
#define _SAMPLEPLAYER_H

#include "Oscili.h"

namespace AuLib {

/** Sample playback oscillator
    with linear interpolation
*/
class SamplePlayer : public Oscili {

protected:
  /** Oscillator for multichannel tables
   */
  virtual void dsp();

  /** AM/FM processing
  */
  virtual void am_fm(uint32_t ndx) {
    if (m_am != nullptr)
      m_amp = m_am[ndx];
    if (m_fm != nullptr) {
      m_freq = m_fm[ndx] * m_sr / m_tframes;
      m_incr = m_fm[ndx];
    }
  }

  /** set the sampling increment
   */
  virtual void set_incr(double f) {
    m_freq = f * m_sr / m_tframes;
    m_incr = f;
  }

public:
  /** SamplePlayer constructor \n\n
      table - function table \n
      amp - amplitude \n
      pitch - playback pitch \n
      phase - init phase (0-1) \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  SamplePlayer(const FuncTable &ftable, double amp = 0., double pitch = 0.,
               double phase = 0., uint32_t vframs = def_vframes,
               double sr = def_sr)
      : Oscili(amp, 0., ftable, phase, vframs, sr) {
    m_incr = pitch;
    m_nchnls = ftable.nchnls();
    vframes(m_vframes);
  }
};

/*! \class SamplePlayer SamplePlayer.h AuLib/SamplePlayer.h
 */
}
#endif
