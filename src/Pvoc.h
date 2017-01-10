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

public:
  /** Pvoc constructor \n\n
    win - analysis window \n
    decim - decimation \n
    fwd - true for forward, false for inverse \n
    vframes - vector size \n
    sr - sampling rate \n
*/
  Pvoc(const FuncTable &win, bool fwd = true, uint32_t decim = def_decim,
       uint32_t vframes = def_vframes, double sr = def_sr)
      : Stft(win, fwd, decim, true, vframes, sr), m_sbuf(fwd ? m_N / 2 : m_N),
        m_done(false) {}

  /** transform an signal sig, vframes is the (time-domain) processing vector
   * size.
    */
  virtual const double *process(const double *sig,
                                uint32_t vframes = def_vframes);

  /** transform a signal in obj
   */
  virtual const Stft &process(const AudioBase &obj) {
    if (obj.nchnls() == 1) {
      process(obj.vector(), m_fwd ? obj.vframes() : m_vframes);
    } else
      m_error = AULIB_ERROR;
    return *this;
  }
};

/*! \class Pvoc Pvoc.h AuLib/Pvoc.h
 */
}

#endif
