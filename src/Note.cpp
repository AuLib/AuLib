////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// Note.cpp: note and instrument implementation
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#include <Note.h>

bool AuLib::Note::note_on(int32_t chn, uint32_t num, uint32_t vel,
                          uint64_t tstamp) {
  if (m_chn < 0 || m_chn == chn) {
    if (!m_on) {
      m_on = true;
      m_num = num;
      m_vel = vel;
      m_tstamp = tstamp;
      m_amp = m_vel / 128.;
      m_cps = 440. * pow(2., (m_num - 69.) / 12.);
      on_note();
    }
  }
  return m_on;
}

bool AuLib::Note::note_off(int32_t chn, uint32_t num, uint32_t vel) {
  if (m_chn < 0 || m_chn == chn) {
    if (m_num == num && m_on) {
      m_on = false;
      m_vel = vel;
      m_num = num;
      m_amp = 0.;
      off_note();
    }
  }
  return m_on;
}
bool AuLib::Note::note_off() {
  if (m_on) {
    m_on = false;
    m_amp = 0.;
    off_note();
  }
  return m_on;
}
