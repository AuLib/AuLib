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

/** definition of polar
 */
const bool polar = true;

/** definition of rectang
 */
const bool rectang = false;

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
      repr - data format, polar or rectang \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Stft(const FuncTable &win, bool dir, uint32_t decim = def_decim,
       bool repr = rectang, uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, dir == fft::forward ? win.tframes() : vframes, sr),
        m_z(0., 0.), m_N(win.tframes()), m_H(decim ? m_N / decim : m_N),
        m_D(decim ? decim : 1), m_dir(dir), m_repr(repr), m_framecount(0),
        m_win(win), m_framebufs(m_D, std::vector<double>(m_N)), m_pos(m_D),
        m_cdata(m_N / 2) {
    for (uint32_t i = 0; i < decim; i++)
      m_pos[i] = (m_D - i - 1) * m_H;
  };

  /** transform an signal sig, vframes is the (time-domain) processing vector
   * size.
   */
  virtual const double *process(const double *sig,
                                uint32_t vframes = def_vframes) {
    return transform(sig, vframes);
  }

  /** transform a signal in obj
   */
  virtual const Stft &process(const AudioBase &obj) {
    if (obj.nchnls() == 1) {
      transform(obj.vector(),
                m_dir == fft::forward ? obj.vframes() : m_vframes);
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** return data format (polar or rectang)
   */
  bool repr() const { return m_repr; }

  /** return analysis frame count
   */
  bool framecount() const { return m_framecount; }

  /** return bin data as a complex<double> ref
      (only in forward transforms)
   */
  const std::complex<double> &bin(uint32_t n) {
    if (m_dir == fft::forward)
      return m_cdata[n < m_N ? n : m_N - 1];
    else
      return m_z;
  }
};

/*! \class Stft Stft.h AuLib/Stft.h
 */
}
#endif
