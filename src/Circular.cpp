////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// Circular.cpp: circular buffer implementation
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#include "Circular.h"

bool AuLib::Circular::dsp(const double *sig) {
  if(m_samps < m_buffer.size()) {
  std::copy(sig,sig+m_vframes,m_wpos);
  m_wpos = m_wpos == m_buffer.end() ? m_buffer.begin() : m_wpos+m_vframes;
  m_samps += m_vframes;
  return true;
  } return false;
}

const double *AuLib::Circular::dsp() {
  if(m_samps) {
  std::copy(m_rpos,m_rpos+m_vframes, m_vector.begin());
  m_wpos = m_wpos == m_buffer.end() ? m_buffer.begin() : m_wpos+m_vframes; 
  m_samps -= m_vframes;
  return vector();
  } else return nullptr;
}


