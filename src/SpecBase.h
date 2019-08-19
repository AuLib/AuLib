/////////////////////////////////////////////////////////////////////
// SpecBase class: Streaming Spectral Processing
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SPECBASE_H_
#define _SPECBASE_H_
#include "AudioBase.h"

namespace AuLib {

class SpecBase : public AudioBase {

protected:
  uint32_t m_H;
  bool m_repr;
  uint32_t m_cnt;
  uint32_t m_tdframes;

  /** test for new frame at input
      returns true if ready for processing
      should be called for every time-domain
      block processed.
   */
  bool ready() {
    m_cnt += m_tdframes;
    if(m_cnt >= m_H) {
      m_cnt -= m_H;
      return true;
    }
    else return false;
  }

public:
  /** SpecBase constructor \n\n
      framesize - size of spectral frame.
      decim - decimation \n
      repr - data format, fft::polar or fft::rectang \n
      tdframes - time-domain vector size \n
      sr - sampling rate \n
  */
  SpecBase(uint32_t framesize, uint32_t decim = def_decim, bool repr = fft::rectang,
	   uint32_t tdframes = def_vframes, double sr = def_sr)
    :   AudioBase(1, framesize, sr), 
        m_H(decim ? framesize / decim : framesize),
        m_repr(repr), m_cnt(0), m_tdframes(tdframes) { };

  /** return data format (fft::polar, fft::rectang)
   */
  bool repr() const { return m_repr; }

  /** return analysis frame count
   */
  bool framecount() const { return m_framecount; }

  /** spectrum as a complex<double> pointer
   */
  const std::complex<double> *spectrum() {
    return reinterpret_cast<std::complex<double> *>(m_vector.data());
  }

  /** return bin data as a complex<double> ref
   */
  const std::complex<double> &bin(uint32_t n) {
    return spectrum()[n < m_N / 2 ? n : m_N / 2 - 1];
  }

  /** Scale the spectral data vector
   */
  virtual const AudioBase &operator*=(double scal) {
    if (m_repr == fft::polar) {
      for (uint32_t i = 0; i < m_vector.size(); i += 2) {
        m_vector[i] *= scal;
      }
    } else
      return AudioBase::operator*=(scal);
    return *this;
  }

  /** Multiply the data vector by a spectral vector
   */
  virtual const AudioBase &operator*=(const double *sig) {
    if (m_repr == fft::polar) {
      for (uint32_t i = 0; i < m_vector.size(); i += 2) {
        m_vector[i] *= sig[i];
        m_vector[i + 1] *= sig[i + 1];
      }
    } else
      return AudioBase::operator*=(sig);
    return *this;
  }

  /** Multiply the data vector by the vector from obj
   */
  virtual const AudioBase &operator*=(const SpecBase &obj) {
    if (m_vframes == obj.m_vframes && m_repr == obj.repr()) {
      if (m_repr == fft::polar) {
        for (uint32_t i = 0; i < m_vector.size(); i += 2) {
          m_vector[i] *= obj[i];
          m_vector[i + 1] *= obj[i + 1];
        }
      } else
        AudioBase::operator*=(obj);
    }
    return *this;
  }

  /** Add a double to the spectral data
      (non-op for polar repr)
  */
  virtual const AudioBase &operator+=(double num) {
    if (m_repr != fft::polar)
      for (uint32_t i = 0; i < m_vector.size(); i += 2) {
        m_vector[i] += num;
      }
    return *this;
  }

  /** Add a vector sig to the data vector
      (non-op for polar repr)
   */
  virtual const AudioBase &operator+=(const double *sig) {
    if (m_repr != fft::polar)
      for (uint32_t i = 0; i < m_vector.size(); i += 2)
        m_vector[i] += sig[i];
    return *this;
  }

  /** Add a vector sig from obj to the data vector
      (non-op for polar repr)
   */
  virtual const AudioBase &operator+=(const SpecBase &obj) {
    if (m_vframes == obj.m_vframes && obj.repr() == m_repr &&
        m_repr != fft::polar)
      for (uint32_t i = 0; i < m_vector.size(); i += 2)
        m_vector[i] += obj[i];
    return *this;
  }
};

/*! \class SpecBase SpecBase.h AuLib/SpecBase.h
 */
}
#endif
