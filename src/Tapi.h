/////////////////////////////////////////////////////////////////////
// Tapi class: delayline tap, linear interpolation
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TAPI_H_
#define _TAPI_H_
#include "Tap.h"

namespace AuLib {

/** Creates an tap for a Delay object, using
    interpolated readout.
*/
class Tapi : public Tap {

public:
  /** Tapi constructor \n\n
      vframes - vector size \n
      sr - sampling rate
  */
  Tapi(uint32_t vframes = def_vframes, double sr = def_sr) : Tap(vframes, sr){};

  /** tap a delay object at time secs
   */
  virtual const Tapi &process(const Delay &obj, double time);

  /** tap a delay object according to time signal in secs
   */
  virtual const double *process(const Delay &obj, const double *time);

  /** tap a delay object according to time signal from obj in secs
   */
  virtual const Tapi &process(const Delay &del, const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(del, obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }
};

/*! \class Tapi Tapi.h AuLib/Tapi.h
 */
}
#endif
