/////////////////////////////////////////////////////////////////////
// Iir class: generic second-order section
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _IIR_H_
#define _IIR_H_
#include "AudioBase.h"

namespace AuLib {

/** General-purpose 2nd-order IIR filter section
 */
class Iir : public AudioBase {

protected:
  double m_del[2];
  double m_a[3];
  double m_b[2];
  double m_scal;

  /** Filter kernel
   */
  virtual const double *filter(const double *sig);

  /** Coefficients update
   */
  virtual void update(){};

public:
  /** Iir constructor \n\n
      a - feedforward coef list (a0,a1,a2)
      b - feedback coefs (b1, b2)
      vframes - vector size \n
      sr - sampling rate
  */
  Iir(const double *a, const double *b, uint32_t vframes = def_vframes,
         double sr = def_sr)
      : AudioBase(1, vframes, sr), m_del{0., 0.}, m_a{0., 0., 0.}, m_b{0., 0.},
        m_scal(1.) {
    std::copy(a, a + 3, m_a);
    std::copy(b, b + 3, m_a);
  };

  /** Iir constructor \n\n
      vframes - vector size \n
      sr - sampling rate
  */

  Iir(uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr), m_del{0., 0.}, m_a{0., 0., 0.}, m_b{0., 0.},
        m_scal(1.){};

  /** process a signal sig
   */
  virtual const double *process(const double *sig) {
    filter(sig);
    return vector();
  }

  /** process a signal sig with coefficients lists a and b
   */
  const double *process(const double *sig, const double *a, const double *b) {
    std::copy(a, a + 3, m_a);
    std::copy(b, b + 3, m_a);
    return process(sig);
  }

  /** process a signal in obj
   */
  virtual const Iir &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** process a signal in obj with coefficients lists a and b
   */
  const Iir &process(const AudioBase &obj, const double *a,
                        const double *b) {
    std::copy(a, a + 3, m_a);
    std::copy(b, b + 3, m_a);
    return process(obj);
  }
};

/*! \class Iir Iir.h AuLib/Iir.h
 */
}
#endif
