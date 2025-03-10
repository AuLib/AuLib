/////////////////////////////////////////////////////////////////////
// Stft class: streaming Phase Vocoder
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _PVOC_H_
#define _PVOC_H_
#include "Stft.h"

namespace AuLib {

class Pvoc : public Stft {
  
  std::vector<double> m_sbuf;
  bool m_done;

  /** PVOC transform
   */
  const double *transform(const double *sig, uint32_t vframes) override;


    /** Scale the spectral data vector
 */
  const AudioBase &prod(double scal) override {
    return Stft::operator*=(scal);
  }

  /** Multiply the data vector by a spectral vector
   */
  const AudioBase &prod(const double *sig) override {
    if (m_dir == fft::inverse) return AudioBase::operator*=(sig);
    for (uint32_t i = 0; i < m_vector.size(); i += 2) {
      m_vector[i] *= sig[i];
    }
    return *this;
  }

  /** Multiply the data vector by the vector from obj
   */
  const AudioBase &prod(const AudioBase &obj) override {
    if (m_dir == fft::inverse) return AudioBase::operator*=(obj);
    if (m_vframes == obj.vframes()) {
      for (uint32_t i = 0; i < m_vector.size(); i += 2) {
        m_vector[i] *= obj[i];
      }
    }
    return *this;
  }

  /** Add a double to the magnitude of spectral data
  */
  const AudioBase &sum(double num) override {
    if (m_dir == fft::inverse) return AudioBase::operator+=(num);
    for (uint32_t i = 0; i < m_vector.size(); i += 2) {
      m_vector[i] += num;
    }
    return *this;
  }

  /** Mix a spectral vector into this object
   */
  const AudioBase &sum(const double *sig) override {
    if (m_dir == fft::inverse) return AudioBase::operator+=(sig);
    for (uint32_t i = 0; i < m_vector.size(); i += 2) {
      if (m_vector[i] < sig[i]) {
        m_vector[i] = sig[i];
        m_vector[i + 1] = sig[i + 1];
      }
    }
    return *this;
  }

  /** Mix a vector sig from obj into this object
   */
  const AudioBase &sum(const AudioBase &obj) override {
    if (m_dir == fft::inverse) return AudioBase::operator+=(obj);
    if (m_vframes == obj.vframes())
      for (uint32_t i = 0; i < m_vector.size(); i += 2) {
        if (m_vector[i] < obj[i]) {
          m_vector[i] = obj[i];
          m_vector[i + 1] = obj[i + 1];
        }
      }
    return *this;
  }

  /** return spectrum as a complex<double> vector ref
      (only meaningful in forward transforms)
   */
  const std::vector<std::complex<double>> &spec() override {
    double *r = reinterpret_cast<double *>(m_cdata.data());
    std::copy(m_vector.begin(), m_vector.end(), r);
    return m_cdata;
  }

public:
  /** Pvoc constructor \n\n
    win - analysis window \n
    decim - decimation \n
    fwd - true for forward, false for inverse \n
    vframes - vector size \n
    sr - sampling rate \n
*/
  Pvoc(const FuncTable &win, bool dir, uint32_t decim = def_decim,
       uint32_t vframes = def_vframes, double sr = def_sr)
      : Stft(win, dir, decim, fft::polar, vframes, sr),
        m_sbuf(dir == fft::forward ? m_N / 2 : m_N), m_done(false) {}

};

/*! \class Pvoc Pvoc.h AuLib/Pvoc.h
 */
}

#endif
