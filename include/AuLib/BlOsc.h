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

  /** returns a function table in the set.
  */
  const FuncTable &operator[](uint32_t num) const {
    return num < m_waves.size() ? m_waves[num] : m_waves.back();
  }
};

/** Bandlimited wavetable oscillator
 */
class BlOsc : public Oscili {

protected:
  const TableSet &m_waves;

  /** AM/FM processing
 */
  virtual void am_fm(uint32_t ndx) {
    if (m_am != nullptr)
      m_amp = m_am[ndx];
    if (m_fm != nullptr) {
      m_freq = m_fm[ndx];
      m_incr = m_freq * m_tframes / m_sr;
      tselect();
    }
  }

  /** table selection
   */
  void tselect() {
    int32_t num = (int32_t)log2(m_freq / base);
    num = num < 0 ? 0 : (num > octs - 1 ? octs - 1 : num);
    m_table = m_waves[num].vector();
  }

  /** set the sampling increment
   */
  virtual void set_incr(double f) {
    Oscil::set_incr(f);
    tselect();
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
      : Oscili(amp, freq, waveset[0], phase, vframes, sr), m_waves(waveset) {
    tselect();
  }
};

namespace waveset {

/**
   Sawtooth table set
*/
static const TableSet saw(SAW);

/**
   Triangle table set
*/
static const TableSet triangle(TRIANGLE);

/**
   Square table set
*/
static const TableSet square(SQUARE);
}

struct SqOsc : BlOsc {
  SqOsc(double amp = 0.f, double freq = 440.f, double phase = 0.,
        uint32_t vframes = def_vframes, double sr = def_sr)
      : BlOsc(amp, freq, waveset::square, phase, vframes, sr){};
};

struct TriOsc : BlOsc {
  TriOsc(double amp = 0.f, double freq = 440.f, double phase = 0.,
         uint32_t vframes = def_vframes, double sr = def_sr)
      : BlOsc(amp, freq, waveset::triangle, phase, vframes, sr){};
};

struct SawOsc : BlOsc {
  SawOsc(double amp = 0.f, double freq = 440.f, double phase = 0.,
         uint32_t vframes = def_vframes, double sr = def_sr)
      : BlOsc(amp, freq, waveset::saw, phase, vframes, sr){};
};

/*! \class BlOsc BlOsc.h AuLib/BlOsc.h
 */
}
#endif
