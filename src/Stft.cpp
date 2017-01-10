////////////////////////////////////////////////////////////////////////////////
// Implementation of the Stft class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#include "Stft.h"

const double *AuLib::Stft::process(const double *sig) {
  for (uint32_t i = 0; i < m_vframes; i++, m_cnt++) {
    if (m_cnt == m_H) {
      uint32_t offset = m_cur * m_H;
      double *r = reinterpret_cast<double *>(m_cdata.data());
      if (m_fwd) {
        for (uint32_t j = 0; j < m_N; j++) {
          r[(i + offset) % m_N] = m_framebufs[m_cur][i] * m_win[i];
        }
        fft::transform(m_cdata, r);
        std::copy(r, r + m_N, m_vector.begin());
      } else {
        std::copy(sig, sig + m_N, r);
        fft::transform(r, m_cdata);
        for (uint32_t j = 0; j < m_N; j++) {
          m_framebufs[m_cur][i] = r[(i + offset) % m_N] * m_win[i];
        }
      }
      m_cur = m_cur == m_D ? 0 : m_cur + 1;
      m_cnt = 0;
    }
    if (!m_fwd)
      m_vector[i] = 0.;
    for (uint32_t j = 0; j < m_D; j++) {
      if (m_fwd) {
        m_framebufs[j][m_pos[j]] = sig[i];
      } else {
        m_vector[i] += m_framebufs[j][m_pos[j]];
      }
      m_pos[j] = m_pos[j] == m_N - 1 ? 0 : m_pos[j] + 1;
    }
  }
  return vector();
}
