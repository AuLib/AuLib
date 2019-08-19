/////////////////////////////////////////////////////////////////////
// Stft class: Short-time Fourier Transform
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _STFT_H_
#define _STFT_H_
#include "AudioBase.h"
#include "FuncTable.h"
#include "fft.h"

namespace AuLib {

/** Stft implements a streaming Short-time Fourier Transform
    overlapped windows are taken from the input (forward mode)
    and analysis frames in either rectangular or polar formats
    are produced in the output (N/2 bins, first bin containing
    both DC and Nyq points) containing the non-negative spectrum. \n
    On inverse mode, spectral data is read from the input and
    transformed back to the output, using an overlap-add algorithm.
*/
class Stft : public AudioBase {

protected:
  const std::complex<double> m_z;
  uint32_t m_N;
  uint32_t m_H;
  uint32_t m_D;
  bool m_dir;
  bool m_repr;
  uint64_t m_framecount;
  const FuncTable &m_win;
  std::vector<std::vector<double>> m_framebufs;
  std::vector<uint32_t> m_pos;
  std::vector<std::complex<double>> m_cdata;

  /** STFT transform
   */
  virtual const double *transform(const double *sig, uint32_t vframes);

public:
  /** Stft constructor \n\n
      win - analysis window \n
      decim - decimation \n
      dir - transform direction, forward or inverse \n
      repr - data format, fft::polar or fft::rectang \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Stft(const FuncTable &win, bool dir, uint32_t decim = def_decim,
       bool repr = fft::rectang, uint32_t vframes = def_vframes,
       double sr = def_sr)
      : AudioBase(1, dir == fft::forward ? win.tframes() : vframes, sr),
        m_z(0., 0.), m_N(win.tframes()), m_H(decim ? m_N / decim : m_N),
        m_D(decim ? decim : 1), m_dir(dir), m_repr(repr), m_framecount(0),
        m_win(win), m_framebufs(m_D, std::vector<double>(m_N)), m_pos(m_D),
        m_cdata(m_N / 2) {
    for (uint32_t i = 0; i < decim; i++)
      m_pos[i] = (m_D - i - 1) * m_H;
  };

  /** transform an signal sig, vframes is the (time-domain) processing vector
   *  size and it is required to be >= analysis hopsize, otherwise no processing
   *  takes place.
   */
  const double *process(const double *sig, uint32_t vframes = def_vframes) {
    if (vframes >= m_H)
      return transform(sig, vframes);
    else
      return vector();
  }

  /** transform a signal in obj
   */
  const Stft &process(const AudioBase &obj) {
    if (obj.nchnls() == 1) {
      transform(obj.vector(),
                m_dir == fft::forward ? obj.vframes() : m_vframes);
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** operator (a) convenience method
   */
  const Stft &operator()(const AudioBase &obj) { return process(obj); }

  /** return data format (fft::polar or fft::rectang)
   */
  bool repr() const { return m_repr; }

  /** return analysis frame count
   */
  bool framecount() const { return m_framecount; }

  /** spectrum as a complex<double> vector ref
      (only meaningful in forward transforms)
   */
  virtual const std::vector<std::complex<double>> &spectrum() {
    return m_cdata;
  }

  /** return bin data as a complex<double> ref
      (only meaningful in forward transforms)
   */
  const std::complex<double> &bin(uint32_t n) {
    return spectrum()[n < m_N / 2 ? n : m_N / 2 - 1];
  }

  /** Scale the spectral data vector
   */
  virtual const AudioBase &operator*=(double scal) {
    if (m_dir == fft::inverse) return AudioBase::operator*=(scal);
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
    if (m_dir == fft::inverse) return AudioBase::operator*=(sig);
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
  virtual const AudioBase &operator*=(const Stft &obj) {
    if (m_dir == fft::inverse) return AudioBase::operator*=(obj);
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
    if (m_dir == fft::inverse) return AudioBase::operator+=(num);
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
    if (m_dir == fft::inverse) return AudioBase::operator+=(sig);
    if (m_repr != fft::polar)
      for (uint32_t i = 0; i < m_vector.size(); i += 2)
        m_vector[i] += sig[i];
    return *this;
  }

  /** Add a vector sig from obj to the data vector
      (non-op for polar repr)
   */
  virtual const AudioBase &operator+=(const Stft &obj) {
    if (m_dir == fft::inverse) return AudioBase::operator+=(obj);
    if (m_vframes == obj.m_vframes && obj.repr() == m_repr &&
        m_repr != fft::polar)
      for (uint32_t i = 0; i < m_vector.size(); i += 2)
        m_vector[i] += obj[i];
    return *this;
  }
};

/*! \class Stft Stft.h AuLib/Stft.h
 */
}
#endif
