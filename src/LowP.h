/////////////////////////////////////////////////////////////////////
// LowP class: low-pass filter using Butterworth design
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _LOWP_H_
#define _LOWP_H_
#include "Filter.h"

namespace AuLib {

/** 2nd-order Butterworth low-pass filter
 */
class LowP : public Filter {

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
      : Filter(vframes, sr), m_freq(cf) {
    update();
  };

  /** process a signal
   */
  virtual const double *process(const double *sig) { return filter(sig); }

  /** process a signal in obj
   */
  virtual const LowP &process(const AudioBase &obj) {
    filter(obj.vector());
    return *this;
  }

  /** process a signal sig with cutoff freq cf
   */
  virtual const double *process(const double *sig, double cf) {
    if (m_freq != cf) {
      m_freq = cf;
      update();
    }
    filter(sig);
    return vector();
  }

  /** process a signal in obj with cutoff freq cf
   */
  virtual const LowP &process(const AudioBase &obj, double cf) {
    if (m_freq != cf) {
      m_freq = cf;
      update();
    }
    filter(obj.vector());
    return *this;
  }
};

/*! \class LowP LowP.h AuLib/LowP.h
 */
}
#endif
