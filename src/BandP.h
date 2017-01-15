/////////////////////////////////////////////////////////////////////
// BandP class: band-pass filter using Butterworth design
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _BANDP_H_
#define _BANDP_H_
#include "LowP.h"

namespace AuLib {

/**  2nd-order Butterworth band-pass filter
 */
class BandP : public LowP {

protected:
  double m_bw;

  /** Coefficients update
   */
  virtual void update();

public:
  /** BandP constructor \n\n
      cf  - centre frequency \n
      bw  - bandwidth  \n
      vframes - vector size \n
      sr - sampling rate
  */
  BandP(double cf, double bw, uint32_t vframes = def_vframes,
        double sr = def_sr)
      : LowP(cf, vframes, sr), m_bw(bw) {
    update();
  };

  /** process a signal
   */
  virtual const double *process(const double *sig) { return filter(sig); }

  /** process a signal in obj
   */
  virtual const BandP &process(const AudioBase &obj) {
    filter(obj.vector());
    return *this;
  }

  /** process a signal sig with cutoff freq cf
   */
  virtual const double *process(const double *sig, double cf) {
    return LowP::process(sig, cf);
  }

  /** process a signal sig with cutoff freq cf and bandwidth bw
   */
  virtual const double *process(const double *sig, double cf, double bw) {
    if (m_bw != bw || m_freq != cf) {
      m_bw = bw > 0. ? bw : m_bw;
      m_freq = cf;
      update();
    }
    filter(sig);
    return vector();
  }

  /** process a signal in obj with cutoff freq cf
   */
  virtual const BandP &process(const AudioBase &obj, double cf) {
    LowP::process(obj, cf);
    return *this;
  }

  /** process a signal in obj with cutoff freq cf and
      bandwidth bw
  */
  virtual const BandP &process(const AudioBase &obj, double cf, double bw) {
    if (m_bw != bw || m_freq != cf) {
      m_bw = bw > 0. ? bw : m_bw;
      m_freq = cf;
      update();
    }
    filter(obj.vector());
    return *this;
  }
};

/*! \class BandP BandP.h AuLib/BandP.h
 */
}
#endif
