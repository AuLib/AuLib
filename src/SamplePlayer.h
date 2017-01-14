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
  virtual void oscillator();

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
               double phase = 0., uint32_t vframes = def_vframes,
               double sr = def_sr)
      : Oscili(amp, 0., ftable, phase, vframes, sr) {
    m_incr = pitch;
    m_nchnls = ftable.nchnls();
    if (m_nchnls > 1)
      m_vector.resize(m_vframes * m_nchnls);
  }

  /** Process one vector of audio
   */
  virtual const SamplePlayer &process() {
    m_nchnls > 1 ? oscillator() : Oscili::oscillator();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp
  */
  virtual const SamplePlayer &process(double amp) {
    m_amp = amp;
    process();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp and
      pitch transposition
  */
  virtual const SamplePlayer &process(double amp, double pitch) {
    m_amp = amp;
    m_freq = pitch * m_sr / m_tframes;
    m_incr = pitch;
    process();
    return *this;
  }

  /** Process one vector of audio
      with amplitude modulation from obja
  */
  virtual const Oscil &process(const AudioBase &obja) {
    if (obja.vframes() == m_vframes) {
      m_am = obja.vector();
      process();
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** Process one vector of audio
      with amplitude modulation from obja
      and pitch pitch
  */
  virtual const Oscil &process(const AudioBase &obja, double pitch) {
    m_freq = pitch * m_sr / m_tframes;
    m_incr = pitch;
    return process(obja);
  }
};

/*! \class SamplePlayer SamplePlayer.h AuLib/SamplePlayer.h
 */
}
#endif
