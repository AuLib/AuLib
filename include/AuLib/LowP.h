/////////////////////////////////////////////////////////////////////////////////
// LowP class: low-pass filter using Butterworth design
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _LOWP_H_
#define _LOWP_H_
#include "Iir.h"

namespace AuLib {

/** 2nd-order Butterworth low-pass filter
 */
class LowP : public Iir {

protected:
  double m_freq;

  /** Coefficients update
   */
  virtual void update();

public:
  /** LowP constructor \n\n
      cf - cutoff freq \n
      vframes - vector size \n
      sr - sampling rate
  */
  LowP(double cf, uint32_t vframes = def_vframes, double sr = def_sr)
      : Iir(vframes, sr), m_freq(cf) {
    update();
  };

  /** using base class overloads
   */
  using Iir::process;

  /** process a signal sig with cutoff freq cf
   */
  const double *process(const double *sig, double cf) {
    if (m_freq != cf) {
      m_freq = cf;
      update();
    }
    dsp(sig);
    return vector();
  }

  /** process a signal in obj with cutoff freq cf
   */
  const LowP &process(const AudioBase &obj, double cf) {
    if (m_freq != cf) {
      m_freq = cf;
      update();
    }
    dsp(obj.vector());
    return *this;
  }

  const LowP &operator()(const AudioBase &obj, double cf) {
    return process(obj, cf);
  }
};

/*! \class LowP LowP.h AuLib/LowP.h
 */
}
#endif
