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
    Oscil::process();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp
  */
  virtual const SamplePlayer &process(double amp) {
    m_amp = amp;
    return process();
  }

  /** Process one vector of audio
      with amplitude amp and
      pitch transposition
  */
  virtual const SamplePlayer &process(double amp, double pitch) {
    m_amp = amp;
    m_freq = pitch * m_sr / m_tframes;
    m_incr = pitch;
    return process();
  }

  /** Process one vector of audio
        with amplitude modulation
   */
  virtual const double *process(const double *amp) {
    return Oscil::process(amp);
  }

  /** Process one vector of audio
      with amplitude amp
      and pitch modulation
  */
  virtual const double *process(double amp, const double *pitch) {
    Oscil::process(amp, pitch);
    return vector();
  }

  /** Process one vector of audio
      with amplitude modulation
      and pitch pitch
  */
  virtual const double *process(const double *amp, double pitch) {
    m_am = amp;
    m_freq = pitch * m_sr / m_tframes;
    m_incr = pitch;
    Oscil::process();
    return vector();
  }

  /** Process one vector of audio
      with amplitude modulation from obja
      and pitch pitch
  */
  virtual const SamplePlayer &process(const AudioBase &obja, double pitch) {
    m_freq = pitch * m_sr / m_tframes;
    m_incr = pitch;
    Oscil::process(obja);
    return *this;
  }

  /** Process one vector of audio
    with amplitude amp
    and pitch modulation from objf
*/
  virtual const SamplePlayer &process(double amp, const AudioBase &objf) {
    Oscil::process(amp, objf);
    return *this;
  }

  /** Process one vector of audio
      with amplitude from obja and pitch modulation from objf
  */
  virtual const Oscil &process(const AudioBase &obja, const AudioBase &objf) {
    Oscil::process(obja, objf);
    return *this;
  }
};

/*! \class SamplePlayer SamplePlayer.h AuLib/SamplePlayer.h
 */
}
#endif
