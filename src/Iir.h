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
  virtual const double *dsp(const double *sig);

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
  const double *process(const double *sig) { return dsp(sig); }

  /** process a signal sig with coefficients lists a and b
   */
  const double *process(const double *sig, const double *a, const double *b) {
    std::copy(a, a + 3, m_a);
    std::copy(b, b + 3, m_a);
    return dsp(sig);
  }

  /** process a signal in obj
   */
  const Iir &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      dsp(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** process a signal in obj with coefficient lists c as an iir or fir
   */
  const Iir &process(const AudioBase &obj, const double *c, bool iir = true) {
    if (iir)
      std::copy(c, c + 2, m_b);
    else
      std::copy(c, c + 3, m_a);
    return process(obj);
  }

  /** process a signal in obj with coefficients lists a and b
   */
  const Iir &process(const AudioBase &obj, const double *a, const double *b) {
    std::copy(a, a + 3, m_a);
    std::copy(b, b + 3, m_a);
    return process(obj);
  }

  /** operator(a) convenience
   */
  const Iir &operator()(const AudioBase &obj) { return process(obj); }

  /** operator(a, b) convenience
   */
  template <typename T> const Iir &operator()(const AudioBase &obj, T a) {
    return process(obj, a);
  }

  /** operator(a, b, c) convenience
   */
  template <typename T, typename U>
  const Iir &operator()(const AudioBase &obj, T a, U b) {
    return process(obj, a, b);
  }
};

/*! \class Iir Iir.h AuLib/Iir.h
 */
}
#endif
