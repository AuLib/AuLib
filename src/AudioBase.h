/////////////////////////////////////////////////////////////////////
// AudioBase class: dsp base class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _AUDIOBASE_H
#define _AUDIOBASE_H

#include "AuLib.h"
#include <vector>

namespace AuLib {

/** Audio DSP base class
 */
class AudioBase {

protected:
  uint32_t m_nchnls;
  uint32_t m_vframes;
  std::vector<double> m_vector;
  double m_sr;
  uint32_t m_error;

public:
  /** AudioBase constructor
     nchnls - number of channels
     vframes - number of frames in vector
     sr - sampling rate
  */
  AudioBase(uint32_t nchnls = def_nchnls, uint32_t vframes = def_vframes,
            double sr = def_sr)
      : m_nchnls(nchnls), m_vframes(vframes), m_vector(vframes * nchnls, 0.0),
        m_sr(sr), m_error(0){};

  /** Scale the data vector
   */
  const AudioBase &operator*=(double scal) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] *= scal;
    return *this;
  }

  /** Multiply the data vector by a sig vector
   */
  const AudioBase &operator*=(const double *sig) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] *= sig[i];
    return *this;
  }

  /** Multiply the data vector by the vector from obj
   */
  const AudioBase &operator*=(const AudioBase &obj) {
    if (m_vframes == obj.m_vframes && m_nchnls == obj.m_nchnls)
      return *this *= obj.vector();
    else
      return *this;
  }

  /** DC offset the data vector
   */
  const AudioBase &operator+=(double offs) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] += offs;
    return *this;
  }

  /** Add a vector sig to the data vector
   */
  const AudioBase &operator+=(const double *sig) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] += sig[i];
    return *this;
  }

  /** Add a vector sig from obj to the data vector
   */
  const AudioBase &operator+=(const AudioBase &obj) {
    if (m_vframes == obj.m_vframes && m_nchnls == obj.m_nchnls)
      return *this += obj.vector();
    else
      return *this;
  }

  /** Get a reference of
      a single sample at sample pos ndx
      off the data vector
  */
  double &operator[](uint32_t ndx) {
    if (ndx < vsamps())
      return m_vector[ndx];
    else
      return m_vector[vsamps()];
  }

  /** Get a constant reference of
      a single sample at sample pos ndx
       off the data vector
 */
  const double &operator[](uint32_t ndx) const {
    if (ndx < vsamps())
      return m_vector[ndx];
    else
      return m_vector[vsamps()];
  }

  /** set the data vector to a sig vector
      return the AudioBase obj reference
   */
  const AudioBase &set(const double *sig) {
    const double *end = sig + m_vector.size();
    std::copy(sig, end, m_vector.data());
    return *this;
  }

  /** set the data vector to a value v
      return a pointer to the vector
   */
  const double *set(double v) {
    std::fill(m_vector.begin(), m_vector.end(), v);
    return m_vector.data();
  }

  /** set the data vector to a value v at pos p
      return the old (replaced) sample
   */
  double set(double v, uint32_t p) {
    if (p < m_vector.size()) {
      double ret = m_vector[p];
      m_vector[p] = v;
      return ret;
    } else {
      std::cout << p << "\n";
      return 0.;
    }
  }

  /** Conversion operator for const std::vector<double>&
   */
  operator const std::vector<double> &() const { return m_vector; }

  /** Conversion operator for const double*
   */
  operator const double *() const { return m_vector.data(); }

  /** Get the audio vector
   */
  const double *vector() const { return *this; }

  /** Get a single sample at frame ndx
      and channel chn off the data vector
  */
  double vector(uint32_t frndx, uint32_t chn) const {
    if (frndx < m_vframes)
      return m_vector[frndx * m_nchnls + chn];
    else
      return 0.;
  }

  /** Set the current vector size in frames. Clears
      the vector and returns the updated vector
      frame size. Vector is resized if the requested
      frame size cannot be accommodated.
   */
  uint32_t vframes(uint32_t frames) {
    m_vframes = frames;
    if (m_vframes * m_nchnls > vsamps())
      m_vector.resize(m_vframes * m_nchnls);
    set(0.);
    return m_vframes;
  }

  /** Get current vector size in frames
   */
  uint32_t vframes() const { return m_vframes; }

  /** Get (max/allocated) vector size in samples
    */
  uint32_t vsamps() const { return m_vector.size(); }

  /** Get number of channels
   */
  uint32_t nchnls() const { return m_nchnls; }

  /** Get sampling reate
   */
  uint32_t sr() const { return m_sr; }

  /** Get error code
   */
  uint32_t error() const { return m_error; }

  /** Get error message
   */
  virtual const char *error_message() const {
    return aulib_error[m_error].c_str();
  }
};

/*! \class AudioBase AudioBase.h AuLib/AudioBase.h
 */
}
#endif
