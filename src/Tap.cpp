////////////////////////////////////////////////////////////////////
// Implementation of the Tap and derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Tap.h"
#include "Tapi.h"

const AuLib::Tap &AuLib::Tap::process(const Delay &obj, double time) {
  if (obj.vframes() == m_vframes && obj.sr() == m_sr) {
    const AudioBase &delay = obj.delayline();
    int32_t dt = (uint32_t)(time > 0. ? time * m_sr : 0), rp ;
    int32_t pos = obj.pos() - m_vframes;
    int32_t max = delay.vsamps();
    if (dt > max)
      dt = max;
    if (pos < 0)
      pos += max;
    rp = pos - dt;
    if (rp < 0)
      rp += max;
    for (uint32_t i = 0; i < m_vframes; i++) {
      m_vector[i] = delay[rp];
      rp = rp == max - 1 ? 0 : rp + 1;
    }
  }
  return *this;
}

const AuLib::Tapi &AuLib::Tapi::process(const Delay &obj, double time) {
  if (obj.vframes() == m_vframes && obj.sr() == m_sr) {
    const AudioBase &delay = obj.delayline();
    double a, b;
    double frac, rp, dt = (time > 0. ? time * m_sr : 0);
    int32_t pos = obj.pos() - m_vframes;
    int32_t rpi, max = delay.vsamps();
    if (dt > max)
      dt = (double)max;
    if (pos < 0)
      pos += max;
    rp = pos - dt;
    if (rp < 0)
      rp += max;
    rpi = (uint32_t)rp;
    frac = rp - rpi;
    for (uint32_t i = 0; i < m_vframes; i++) {
      a = delay[rpi];
      b = rpi + 1 < max ? delay[rpi] : delay[0];
      m_vector[i] = a + (b - a) * frac;
      rpi = rpi == max - 1 ? 0 : rpi + 1;
    }
  } else
    m_error = AULIB_ERROR;
  return *this;
}

const double *AuLib::Tapi::process(const Delay &obj, const double *time) {
  if (obj.vframes() == m_vframes && obj.sr() == m_sr) {
    const AudioBase &delay = obj.delayline();
    double a, b;
    double frac, rp, dt;
    int32_t pos = obj.pos() - m_vframes;
    int32_t rpi, max = delay.vsamps();
    if (pos < 0)
      pos += max;
    for (uint32_t i = 0; i < m_vframes; i++) {
      dt = (time[i] > 0. ? time[i] * m_sr : 0);
      if (dt > max)
        dt = (double)max;
      rp = pos - dt;
      if (rp < 0)
        rp += max;
      rpi = (uint32_t)rp;
      frac = rp - rpi;
      a = delay[rpi];
      b = rpi + 1 < max ? delay[rpi] : delay[0];
      m_vector[i] = a + (b - a) * frac;
      rpi = rpi == max - 1 ? 0 : rpi + 1;
    }
  }
  return vector();
}
