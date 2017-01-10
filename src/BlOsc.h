////////////////////////////////////////////////////////////////////
// BlOsc class: bandlimited waveform oscillator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _BLOSC_H
#define _BLOSC_H

#include "FourierTable.h"
#include "Oscili.h"

namespace AuLib {

const int32_t octs = 10;
const double base = 20;

/** A set of tables for BlOsc
 **/
class TableSet {

  std::vector<FuncTable> m_waves;

public:
  /** TableSet constructor
      type - SAW, SQUARE or TRIANGLE
      sr - sampling rate
  */
  TableSet(uint32_t type, double sr = def_sr) : m_waves(octs) {
    double nyq = sr / 2.;
    for (uint32_t i = 0; i < octs; i++) {
      double fr = base * pow(2, i);
      m_waves[i] = FourierTable(nyq / fr, type);
    }
  }

  /** returns a read-only pointer to
      the array of FuncTables
  */
  const FuncTable *tables() const { return m_waves.data(); }
};

/** Bandlimited wavetable oscillator
 */
class BlOsc : public Oscili {

protected:
  const FuncTable *m_waves;

  /** table selection
   */
  void tselect() {
    int32_t num = (int32_t)log2(m_freq / base);
    num = num < 0 ? 0 : (num > octs - 1 ? octs - 1 : num);
    m_table = m_waves[num].table();
  }

public:
  /** BlOsc constructor \n\n
      amp - amplitude   \n
      freq - frequency in Hz \n
      waveset - TableSet reference with the set of
      bandlimited tables \n
      phase - init phase (0-1) \n
      vframes - vector size \n
      sr - sampling rate \n\n
  */
  BlOsc(double amp, double freq, const TableSet &waveset, double phase = 0.,
        uint32_t vframes = def_vframes, double sr = def_sr)
      : Oscili(amp, freq, waveset.tables()[0], phase, vframes, sr),
        m_waves(waveset.tables()) {
    if (m_waves == nullptr) {
      m_vframes = 0;
      m_error = AULIB_ERROR;
      return;
    }
    tselect();
  }

  /** Process one vector of audio
   */
  virtual const BlOsc &process() {
    Oscili::lookup();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp
  */
  virtual const BlOsc &process(double amp) {
    m_amp = amp;
    Oscili::lookup();
    return *this;
  }

  /** Process one vector of audio
      with amplitude amp and
      pitch transposition
  */
  virtual const BlOsc &process(double amp, double freq) {
    m_amp = amp;
    m_freq = freq;
    m_incr = m_freq * m_tframes / m_sr;
    tselect();
    Oscili::lookup();
    return *this;
  }
};

/*! \class BlOsc BlOsc.h AuLib/BlOsc.h
 */
}
#endif
