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
  
  /** Coefficients update
   */
  void update() override;

protected:
  double m_bw;

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

  /** using base class overloads
   */
  using LowP::process;

  /** process a signal sig with cutoff freq cf and bandwidth bw
   */
  const double *process(const double *sig, double cf, double bw) {
    if (m_bw != bw || m_freq != cf) {
      m_bw = bw > 0. ? bw : m_bw;
      m_freq = cf;
      update();
    }
    process(sig);
    return vector();
  }

  /** process a signal in obj with cutoff freq cf and
      bandwidth bw
  */
  const BandP &process(const AudioBase &obj, double cf, double bw) {
    if (m_bw != bw || m_freq != cf) {
      m_bw = bw > 0. ? bw : m_bw;
      m_freq = cf;
      update();
    }
    process(obj.vector());
    return *this;
  }
};

/*! \class BandP BandP.h AuLib/BandP.h
 */
}
#endif
