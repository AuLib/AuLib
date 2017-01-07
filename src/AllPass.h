/////////////////////////////////////////////////////////////////////
// AllPass class: All-pass filter
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _ALLPASS_H_
#define _ALLPASS_H_
#include "Delay.h"

namespace AuLib {

/** All-pass filter
 */
class AllPass : public Delay {

public:
  /** AllPass constructor \n\n
      dtime - delay time \n
      fdb - feedback gain \n
      vframes - vector size \n
      sr - sampling rate
  */
  AllPass(double dtime, double fdb, uint32_t vframes = def_vframes,
          double sr = def_sr)
      : Delay(dtime, fdb, vframes, sr){};

  virtual const double *process(const double *sig);

  virtual const double *process(const double *sig, double fdb) {
    m_fdb = fdb;
    return process(sig);
  }

  virtual const Delay &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  virtual const Delay &process(const AudioBase &obj, double fdb) {
    m_fdb = fdb;
    return process(obj);
  }
};

/*! \class AllPass AllPass.h AuLib/AllPass.h
 */
}
#endif
