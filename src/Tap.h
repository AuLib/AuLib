/////////////////////////////////////////////////////////////////////
// Tap class: delayline tap
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TAP_H_
#define _TAP_H_
#include "AudioBase.h"
#include "Delay.h"

namespace AuLib {

/** Creates a tap for a Delay object
    truncating readout.
*/
class Tap : public AudioBase {

public:
  /** Tap constructor \n\n
      vframes - vector size \n
      sr - sampling rate
  */
  Tap(uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr){};

  /** tap a delay object at time secs
   */
  virtual const Tap &process(const Delay &obj, double time);
};

/*! \class Tap Tap.h AuLib/Tap.h
 */
}
#endif
