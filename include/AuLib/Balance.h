/////////////////////////////////////////////////////////////////////
// Balance class: balancing the RMS amp of a signal with a
//   comparator signal
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _BALANCE_H_
#define _BALANCE_H_
#include "AudioBase.h"
#include "Rms.h"

namespace AuLib {

/** Balance description
*/
class Balance : public AudioBase {

  virtual const double *dsp(const double *sig, const double *cmp);

protected:
  Rms m_cmp;
  Rms m_sig;

public:
  /** Balance constructor \n\n
      cf - LP cutoff freq \n
      nchnls - number of channels \n
      vframes - vector size \n
      sr - sampling rate
  */
  Balance(double cf = 10., uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr), m_cmp(cf, vframes, sr),
        m_sig(cf, vframes, sr){};

  /** process a sig with a comparator cmp
   */
  const double *process(const double *sig, const double *cmp) {
    return dsp(sig, cmp);
  }

  /** process a signal in obj
   */
  const Balance &process(const AudioBase &obj, const AudioBase &cmp) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls &&
        cmp.vframes() == m_vframes && cmp.nchnls() == m_nchnls) {
      dsp(obj.vector(), cmp.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** operator(a,b) convenience method, same as process()
   */
  const Balance &operator()(const AudioBase &obj, const AudioBase &cmp) {
    return process(obj, cmp);
  }
};

/*! \class Balance Balance.h AuLib/Balance.h
 */
}
#endif
