////////////////////////////////////////////////////////////////////
// Implementation of the Phasor class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Phasor.h"

AuLib::Phasor::Phasor(double freq, double phase, uint32_t vframes, double sr)
    : AudioBase(1, vframes, sr), m_freq(freq), m_phs(phase) {
  m_incr = m_freq / m_sr;
  mod();
}

const AuLib::Phasor &AuLib::Phasor::process() {
  for (uint32_t i = 0; i < m_vframes; i++) {
    m_vector[i] = m_phs;
    m_phs += m_incr;
    mod();
  }
  return *this;
}
