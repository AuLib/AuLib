/////////////////////////////////////////////////////////////////////
// Signal Bus
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SIGBUS_H_
#define _SIGBUS_H_
#include "AudioBase.h"

namespace AuLib {

/** A signal bus
    with optional scaling, offset and overwriting
*/
class SigBus : public AudioBase {

  virtual const double *dsp(const double *sig) {
    for (uint32_t i = 0; i < m_vframes * m_nchnls; i++)
      m_vector[i] = m_ovw ? m_scal * sig[i] + m_offs
                          : m_scal * sig[i] + m_offs + m_vector[i];
    return vector();
  }

protected:
  double m_scal;
  double m_offs;
  bool m_ovw;

public:
  /** SigBus constructor \n\n
      scal - amplitude scaling \n
      offs - amplitude offset \n
      overwrite - overwrite bus \n
      nchnls - number of channels \n
      vframes - vector size \n
  */
  SigBus(double scal = 1., double offs = 0., bool overwrite = false,
         uint32_t nchnls = def_nchnls, uint32_t vframes = def_vframes)
      : AudioBase(nchnls, vframes), m_scal(scal), m_offs(offs),
        m_ovw(overwrite){};

  /** Adds a signal vector to the bus.
      Requires an explicit call to clear()
      if overwrite is switched off
      once the bus data has been consumed
  */
  const double *process(const double *sig) { return dsp(sig); }

  /** Applies a gain scaling and optional
      offset to a signal vector. If
      overwrite is true, the vector vector is
      overwritten and a call to
      clear() is not required.
  */
  const SigBus &process(const double *sig, double scal, double offs = 0.,
                        bool overwrite = true) {
    m_scal = scal;
    m_offs = offs;
    m_ovw = overwrite;
    dsp(sig);
    return *this;
  }

  /** Adds a signal vector to the bus from obj.
      Requires an explicit call to clear()
      once the bus data has been consumed
  */
  const SigBus &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      dsp(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** Applies a gain scaling and optional
      offset to a signal vector from obj. If
      overwrite is true, the vector vector is
      overwritten and a call to
      clear() is not required.
  */
  const SigBus &process(const AudioBase &obj, double scal, double offs = 0.,
                        bool overwrite = true) {
    m_scal = scal;
    m_offs = offs;
    m_ovw = overwrite;
    dsp(obj.vector());
    return *this;
  }

  /** operator (a) convenience method
   */
  const SigBus &operator()(const AudioBase &obj) {
     return process(obj);
  }

  /** operator (a,b,c,d) convenience method
   */
  const SigBus &operator()(const AudioBase &obj, double scal, double offs = 0.,
                        bool overwrite = true) {
    return process(obj,scal,offs,overwrite);
  }
  
  /** Clears the vector vector
   */
  void clear() { set(0.); }
};

/*! \class SigBus SigBus.h AuLib/SigBus.h
 */
}
#endif
