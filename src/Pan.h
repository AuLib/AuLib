/////////////////////////////////////////////////////////////////////
// Pan class: stereo panning
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _PAN_H_
#define _PAN_H_
#include "AudioBase.h"

namespace AuLib {

/** Pan a mono input signal using
    an equal-power algorithm
*/
class Pan : public AudioBase {

  virtual const double *dsp(const double *sig);

protected:
  double m_pos;

public:
  /** Pan constructor \n\n
      pos - pan position (0 - 1) \n
      vframes - vector size \n
  */
  Pan(double pos = .5, uint32_t vframes = def_vframes)
      : AudioBase(2, vframes), m_pos(pos){};

  /** Pan a signal sig
   */
  const double *process(const double *sig) { return dsp(sig); }

  /** Pan a signal sig according to position pos (0-1)
   */
  const double *process(const double *sig, double pos) {
    m_pos = pos;
    return dsp(sig);
  }

  /** Pan a signal in obj
   */
  const Pan &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == 1) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** Pan a signal in obj according to position pos (0-1)
   */
  const Pan &process(const AudioBase &obj, double pos) {
    m_pos = pos;
    process(obj);
    return *this;
  }

  /** operator(a) convenience method
   */
  const Pan &operator()(const AudioBase &obj) {
    return process(obj);
  }

  /** operator(a,b) convenience method
   */
  const Pan &operator()(const AudioBase &obj, double pos) {
    return process(obj,pos);
  }

  
};

/*! \class Pan Pan.h AuLib/Pan.h
 */
}
#endif
