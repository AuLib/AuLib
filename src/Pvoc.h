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
  virtual const double *transform(const double *sig, uint32_t vframes);

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

  /** return spectrum as a complex<double> vector ref
      (only meaningful in forward transforms)
   */
  virtual const std::vector<std::complex<double>> &spectrum() {
    double *r = reinterpret_cast<double *>(m_cdata.data());
    std::copy(m_vector.begin(), m_vector.end(), r);
    return m_cdata;
  }
};

/*! \class Pvoc Pvoc.h AuLib/Pvoc.h
 */
}

#endif
