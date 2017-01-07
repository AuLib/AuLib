/////////////////////////////////////////////////////////////////////
// ResonR class: Resonator band-pass filter
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _RESONR_H_
#define _RESONR_H_
#include "BandP.h"

namespace AuLib {

/** Band-pass resonator with better amplitude response at
    low frequencies.
*/
class ResonR : public BandP {

protected:
  /** Coefficients update
   */
  virtual void update();

public:
  /** ResonR constructor \n\n
      cf  - centre frequency \n
      bw  - bandwidth  \n
      vframes - vector size \n
      sr - sampling rate
  */
  ResonR(double cf, double bw, uint32_t vframes = def_vframes,
         double sr = def_sr)
      : BandP(cf, bw, vframes, sr) {
    update();
  };
};

/*! \class ResonR ResonR.h AuLib/ResonR.h
 */
}
#endif
