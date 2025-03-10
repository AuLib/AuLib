/////////////////////////////////////////////////////////////////////
// Reson class: original band-pass resonator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _RESON_H_
#define _RESON_H_
#include "ResonR.h"

namespace AuLib {

/** Original band-pass resonator design
 */
class Reson : public ResonR {

 const double *dsp(const double *sig) override;

public:
  /** Reson constructor \n\n
      cf  - centre frequency \n
      bw  - bandwidth  \n
      vframes - vector size \n
      sr - sampling rate
  */
  Reson(double cf, double bw, uint32_t vframes = def_vframes,
        double sr = def_sr)
      : ResonR(cf, bw, vframes, sr){};
};

/*! \class Reson Reson.h AuLib/Reson.h
 */
}
#endif
