/////////////////////////////////////////////////////////////////////
// ResonZ class: another band-pass resonator design
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _RESONZ_H_
#define _RESONZ_H_
#include "ResonR.h"

namespace AuLib {

/** Band-pass resonator with better amplitude response at
    low frequencies (alternative design).
*/
class ResonZ : public ResonR {

protected:
  /** Coefficients update
   */
  virtual void update() {
    ResonR::update();
    m_a[2] = -1.;
  };

public:
  /** ResonZ constructor \n\n
      cf  - centre frequency \n
      bw  - bandwidth  \n
      vframes - vector size \n
      sr - sampling rate
  */
  ResonZ(double cf, double bw, uint32_t vframes = def_vframes,
         double sr = def_sr)
      : ResonR(cf, bw, vframes, sr) {
    update();
  };
};

/*! \class ResonZ ResonZ.h AuLib/ResonZ.h
 */
}
#endif
