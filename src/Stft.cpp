/////////////////////////////////////////////////////////////////////////////////
// Implementation of the Stft and derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////////////////
#include "Stft.h"
#include "Pvoc.h"

const double *AuLib::Stft::transform(const double *sig, uint32_t vframes) {
  for (uint32_t i = 0; i < vframes; i++) {
    if (m_dir == fft::inverse)
      m_vector[i] = 0.;
    for (uint32_t j = 0; j < m_D; j++) {
      if (m_dir == fft::forward) {
        m_framebufs[j][m_pos[j]++] = sig[i];
        if (m_pos[j] == m_N) {
          uint32_t offset = j * m_H;
          double *r = reinterpret_cast<double *>(m_cdata.data());
          for (uint32_t n = 0; n < m_N; n++) {
            r[(n + offset) % m_N] = m_framebufs[j][n] * m_win[n];
          }
          fft::transform(m_cdata, r);
          if (m_repr == fft::polar) {
            m_vector[0] = m_cdata[0].real(), m_vector[1] = m_cdata[0].imag();
            for (uint32_t n = 2, k = 1; n < m_N; n += 2, k++) {
              m_vector[n] = std::abs(m_cdata[k]);
              m_vector[n + 1] = std::arg(m_cdata[k]);
            }
          } else
            std::copy(r, r + m_N, m_vector.begin());
          m_pos[j] = 0;
          m_framecount++;
        }
      } else {
        m_vector[i] += m_framebufs[j][m_pos[j]++];
        if (m_pos[j] == m_N) {
          uint32_t offset = j * m_H;
          double *r = reinterpret_cast<double *>(m_cdata.data());
          if (m_repr == fft::polar) {
	    m_cdata[0].real(sig[0]), m_cdata[0].imag(sig[1]);
            for (uint32_t n = 2, k = 1; n < m_N; n += 2, k++) {
              m_cdata[k] = std::polar(sig[n], sig[n + 1]);
            }
          } else
            std::copy(sig, sig + m_N, r);
          fft::transform(r, m_cdata);
          for (uint32_t n = 0; n < m_N; n++) {
            m_framebufs[j][n] = r[(n + offset) % m_N] * m_win[n];
          }
          m_pos[j] = 0;
          m_framecount++;
        }
      }
    }
  }
  return vector();
}

const double *AuLib::Pvoc::transform(const double *sig, uint32_t vframes) {
  double delta, c = m_sr / m_N, d = m_sr / (twopi * m_H);
  uint32_t fmcnt = m_framecount;
  if (m_dir == fft::forward) {
    Stft::process(sig, vframes);
    if (m_framecount > fmcnt)
      m_done = false;
    if (!m_done) {
      for (uint32_t i = 2, j = 1; i < m_N; i += 2, j++) {
        delta = m_vector[i + 1] - m_sbuf[j];
        m_sbuf[j] = m_vector[i + 1];
        if (delta >= pi)
          delta -= twopi;
        if (delta < -pi)
          delta += twopi;
        m_vector[i + 1] = j * c + delta * d;
      }
      m_done = true;
    }
  } else {
    if (!m_done) {
      for (uint32_t i = 2, j = 1; i < m_N; i += 2, j++) {
        m_sbuf[i] = sig[i];
        delta = (sig[i + 1] - c * j) / d;
        m_sbuf[i + 1] += delta;
      }
      m_done = true;
    }
    Stft::process(m_sbuf.data(), vframes);
    if (m_framecount > fmcnt)
      m_done = false;
  }
  return vector();
}
