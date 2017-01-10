/////////////////////////////////////////////////////////////////////
// Stft class:
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _NEWCLASS_H_
#define _NEWCLASS_H_
#include "AudioBase.h"
#include "FuncTable.h"
#include "fft.h"

namespace AuLib {

/** Stft description
*/
class Stft : public AudioBase {

protected:
  uint32_t m_N;
  uint32_t m_H;
  uint32_t m_D;
  uint32_t m_cnt;
  uint32_t m_cur;
  bool m_fwd;
  bool m_polar;
  const FuncTable &m_win;
  std::vector<std::vector<double>> m_framebufs;
  std::vector<uint32_t> m_pos;
  std::vector<std::complex<double>> m_cdata;

public:
  /** Stft constructor \n\n
      win - analysis window \n
      decim - decimation \n
      fwd - true for forward, false for inverse \n
      polar - true for polar format, false for rectangular \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Stft(const FuncTable &win, uint32_t decim, bool fwd = true,
       bool polar = false, uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr), m_N(win.tframes()),
        m_H(decim ? m_N / decim : m_N), m_D(decim ? decim : 1), m_cnt(0),
        m_cur(0), m_fwd(fwd), m_polar(polar), m_win(win),
        m_framebufs(m_D, std::vector<double>(m_N)), m_pos(m_D),
        m_cdata(m_N / 2) {
    for (uint32_t i = 0; i < decim; i++)
      m_pos[i] = m_D - i - 1;
  };

  /** transform an signal sig
   */
  virtual const double *process(const double *sig);

  /** transform a signal in obj
   */
  virtual const Stft &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** return format (true for polar)
   */
  bool polar() const { return m_polar; }
};

/*! \class Stft Stft.h AuLib/Stft.h
 */
}
#endif
