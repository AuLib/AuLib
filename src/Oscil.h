/////////////////////////////////////////////////////////////////////////////////
// Oscil class: truncating oscillator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _OSCIL_H
#define _OSCIL_H

#include "AudioBase.h"
#include "FourierTable.h"
#include "FuncTable.h"

namespace AuLib {

/** Truncating oscillator
 */
class Oscil : public AudioBase {

protected:
  std::unique_ptr<FourierTable> m_sine;
  const double *m_table;
  double m_phs;
  double m_amp;
  double m_freq;
  double m_incr;
  const double *m_am;
  const double *m_fm;
  uint64_t m_tframes;

  /** truncating oscillator process
   */
  virtual void dsp();

  /** AM/FM processing
   */
  virtual void am_fm(uint32_t ndx) {
    if (m_am != nullptr)
      m_amp = m_am[ndx];
    if (m_fm != nullptr) {
      m_freq = m_fm[ndx];
      m_incr = m_freq * m_tframes / m_sr;
    }
  }

  /** set the sampling increment
   */
  virtual void set_incr(double f) {
    m_freq = f;
    m_incr = m_freq * m_tframes / m_sr;
  }

  /** modulo function
   */
  void mod() {
    while (m_phs >= m_tframes)
      m_phs -= m_tframes;
    while (m_phs < 0)
      m_phs += m_tframes;
  }

public:
  /** Sinusoidal Oscil constructor \n\n
      amp - amplitude   \n
      freq - frequency in Hz \n
      phase - init phase (0-1) \n
      vframes - vector size \n
      sr - sampling rate \n\n
      Uses internal sine wave table
  */
  Oscil(double amp = 0., double freq = 0., double phase = 0.,
        uint32_t vframes = def_vframes, double sr = def_sr);

  /** Oscil constructor \n\n
      amp - amplitude   \n
      freq - frequency in Hz \n
      ftable - function table \n
      phase - init phase (0-1) \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Oscil(double amp, double freq, const FuncTable &ftable, double phase = 0.,
        uint32_t vframes = def_vframes, double sr = def_sr);

  /** Process one vector of audio
   */
  const Oscil &process() {
    dsp();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp
  */
  const Oscil &process(double amp) {
    m_amp = amp;
    return process();
  }

  /** Process one vector of audio
      with amplitude amp and
      frequency freq
  */
  const Oscil &process(double amp, double freq) {
    m_amp = amp;
    set_incr(freq);
    return process();
  }

  /** Process one vector of audio
      with amplitude and freq modulation
  */
  const double *process(const double *amp, const double *freq) {
    m_am = amp;
    m_fm = freq;
    dsp();
    return vector();
  }

  /** Process one vector of audio
        with amplitude modulation
   */
  const double *process(const double *amp) { return process(amp, nullptr); }

  /** Process one vector of audio
      with amplitude amp
      and freq modulation
  */
  const double *process(double amp, const double *freq) {
    m_amp = amp;
    m_fm = freq;
    dsp();
    return vector();
  }

  /** Process one vector of audio
      with amplitude modulation
      and frequency freq
  */
  const double *process(const double *amp, double freq) {
    m_am = amp;
    set_incr(freq);
    dsp();
    return vector();
  }

  /** Process one vector of audio
      with amplitude modulation from obja
  */
  const Oscil &process(const AudioBase &obja) {
    if (obja.vframes() == m_vframes) {
      m_am = obja.vector();
      dsp();
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** Process one vector of audio
      with amplitude modulation from obja
      and frequency freq
  */
  const Oscil &process(const AudioBase &obja, double freq) {
    m_freq = freq;
    set_incr(freq);
    process(obja);
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp
      and freq modulation from objf
  */
  const Oscil &process(double amp, const AudioBase &objf) {
    if (objf.vframes() == m_vframes && objf.nchnls() == 1) {
      m_amp = amp;
      m_fm = objf.vector();
      dsp();
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** Process one vector of audio
      with amplitude from obja and freq modulation from objf
  */
  const Oscil &process(const AudioBase &obja, const AudioBase &objf) {
    if (obja.vframes() == m_vframes && objf.vframes() == m_vframes &&
        obja.nchnls() == 1 && objf.nchnls() == 1) {
      m_am = obja.vector();
      m_fm = objf.vector();
      dsp();
    } else
      m_error = AULIB_ERROR;
    return *this;
  }
};

/*! \class Oscil Oscil.h AuLib/Oscil.h
 */
}
#endif
