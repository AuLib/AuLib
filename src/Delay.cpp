////////////////////////////////////////////////////////////////////////////////
// Implementation of the Delay and derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#include "Delay.h"
#include "AllPass.h"
#include "Fir.h"

const double *AuLib::Delay::dsp(const double *sig) {
  for (uint32_t i = 0; i < m_vframes; i++) {
    m_vector[i] = m_delay.set(sig[i] + m_delay[m_pos] * m_fdb, m_pos);
    m_pos = m_pos == m_delay.vframes() - 1 ? 0. : m_pos + 1;
  }
  return vector();
}

const double *AuLib::Delay::dsp(const double *sig, double dt) {
  uint32_t ds = dt * m_sr;
  int32_t rp;
  if (ds > m_delay.vframes())
    ds = m_delay.vframes();
  for (uint32_t i = 0; i < m_vframes; i++) {
    rp = m_pos - ds;
    if (rp < 0)
      rp += m_delay.vframes();
    m_vector[i] = m_delay[rp];
    m_delay[m_pos] = sig[i] + m_vector[i] * m_fdb;
    m_pos = m_pos == m_delay.vframes() - 1 ? 0. : m_pos + 1;
  }
  return vector();
}

const double *AuLib::Delay::dsp(const double *sig, const double *dt) {
  double rp, ds, a, b, frac;
  uint32_t irp;
  for (uint32_t i = 0; i < m_vframes; i++) {
    ds = dt[i] < 0. ? 0 : dt[i] * m_sr;
    if (ds > m_delay.vframes())
      ds = m_delay.vframes();
    rp = m_pos - ds;
    if (rp < 0)
      rp += m_delay.vframes();
    irp = (uint32_t)rp;
    frac = rp - irp;
    a = m_delay[irp];
    if (++irp == m_delay.vframes() - 1)
      irp = 0;
    b = m_delay[irp];
    m_vector[i] = a + frac * (b - a);
    m_delay[m_pos] = sig[i] + m_vector[i] * m_fdb;
    m_pos = m_pos == m_delay.vframes() - 1 ? 0. : m_pos + 1;
  }
  return vector();
}

const double *AuLib::AllPass::dsp(const double *sig) {
  double y;
  for (uint32_t i = 0; i < m_vframes; i++) {
    y = sig[i] + m_fdb * m_delay[m_pos];
    m_vector[i] = m_delay.set(y, m_pos) - m_fdb * y;
    m_pos = m_pos == m_delay.vframes() - 1 ? 0. : m_pos + 1;
  }
  return vector();
}

const double *AuLib::Fir::dsp(const double *sig) {
  double out = 0;
  uint32_t N = m_delay.vframes();
  uint32_t nchnls = m_ir_nchnls;
  for (uint32_t i = 0; i < m_vframes; i++) {
    m_delay[m_pos] = sig[i];
    m_pos = m_pos != N - 1 ? m_pos + 1 : 0;
    for (uint32_t j = 0, rp = m_pos; j < N; j += nchnls ) {
      out += m_delay[rp] * m_ir[N - 1 - j + m_chn];
      rp = rp != N - 1  ? rp + 1 : 0;
      }
    m_vector[i] = out;
    out = 0.;
    }
  return vector();
}
