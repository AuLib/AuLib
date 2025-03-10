/////////////////////////////////////////////////////////////////////
// ToneHP class: first-order high-pass filter
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TONEHP_H_
#define _TONEHP_H_
#include "ToneLP.h"

namespace AuLib {

/** Simple first-order high-pass filter
 */
class ToneHP : public ToneLP {

  void update() override {
    double costh = 2. - cos(2. * pi * m_freq / m_sr);
    m_b = costh - sqrt(costh * costh - 1.);
    m_a = (1. + m_b);
  }

  const double *dsp(const double *sig) override {
    for (uint32_t i = 0; i < m_vframes; i++) {
      double w = m_a * sig[i] + m_b * m_del;
      m_vector[i] = m_del - w;
      m_del = w;
    }
    return vector();
  }


public:
  /** ToneHP constructor \n\n
      cf - cutoff frequency \n
      vframes - vector size \n
      sr - sampling rate
  */
  ToneHP(double cf, uint32_t vframes = def_vframes, double sr = def_sr)
      : ToneLP(cf, vframes, sr) {
    update();
  }
};

/*! \class ToneLP ToneLP.h AuLib/ToneLP.h
 */
}
#endif
