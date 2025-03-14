////////////////////////////////////////////////////////////////////////////////
// Implementation of the Oscil and derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#include "Oscil.h"
#include "Oscili.h"
#include "Oscilic.h"
#include "SamplePlayer.h"

const AuLib::FourierTable AuLib::Oscil::m_sine;

AuLib::Oscil::Oscil(double amp, double freq, double phase, uint32_t vframes,
                    double sr)
    : AudioBase(1, vframes, sr), m_table(Oscil::m_sine), m_phs(phase),
      m_amp(amp), m_freq(freq), m_am(nullptr), m_fm(nullptr),
      m_tframes(Oscil::m_sine.tframes()) {
  m_incr = m_freq * m_tframes / m_sr;
  mod();
}

AuLib::Oscil::Oscil(double amp, double freq, const FuncTable &ftable,
                    double phase, uint32_t vframes, double sr)
    : AudioBase(1, vframes, sr), m_table(ftable.vector()), m_phs(phase),
      m_amp(amp), m_freq(freq), m_am(nullptr), m_fm(nullptr),
      m_tframes(ftable.tframes()) {
  if (m_table == nullptr) {
    m_vframes = 0;
    m_error = AULIB_ERROR;
  }
  m_incr = m_freq * m_tframes / m_sr;
  mod();
}

void AuLib::Oscil::dsp() {
  for (uint32_t i = 0; i < m_vframes; i++) {
    am_fm(i);
    m_vector[i] = m_amp * m_table[(uint32_t)m_phs];
    m_phs += m_incr;
    mod();
  }
}

void AuLib::Oscili::dsp() {
  uint32_t phi;
  double frac, a, b;
  for (uint32_t i = 0; i < m_vframes; i++) {
    am_fm(i);
    phi = (uint32_t)m_phs;
    frac = m_phs - phi;
    a = m_table[phi];
    b = m_table[phi + 1];
    m_vector[i] = m_amp * (a + frac * (b - a));
    m_phs += m_incr;
    mod();
  }
}

void AuLib::Oscilic::dsp() {
  uint32_t phi;
  double frac, a, b, c, d;
  double tmp, fracsq, fracb;
  for (uint32_t i = 0; i < m_vframes; i++) {
    am_fm(i);
    phi = (uint32_t)m_phs;
    frac = m_phs - phi;
    a = phi == 0 ? m_table[0] : m_table[phi - 1];
    b = m_table[phi];
    c = m_table[phi + 1];
    d = m_table[phi + 2];
    tmp = d + 3.f * b;
    fracsq = frac * frac;
    fracb = frac * fracsq;
    m_vector[i] = m_amp * (fracb * (-a - 3.f * c + tmp) / 6.f +
                           fracsq * ((a + c) / 2.f - b) +
                           frac * (c + (-2.f * a - tmp) / 6.f) + b);
    m_phs += m_incr;
    mod();
  }
}

void AuLib::SamplePlayer::dsp() {
  if (m_nchnls > 1) {
    uint32_t phi;
    double frac, a, b;
    for (uint32_t i = 0; i < m_vframes; i++) {
      phi = (uint32_t)m_phs;
      frac = m_phs - phi;
      phi *= m_nchnls;
      for (uint32_t j = 0; j < m_nchnls; j++) {
        a = m_table[phi + j];
        b = m_table[phi + j + m_nchnls];
        m_vector[i * m_nchnls + j] = m_amp * (a + frac * (b - a));
      }
      m_phs += m_incr;
      mod();
    }
  } else
    Oscili::process();
}
