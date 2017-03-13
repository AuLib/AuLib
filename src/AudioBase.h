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
  /** AudioBase constructor  \n
     nchnls - number of channels \n
     vframes - number of frames in vector. This is set to next power-of-two no
      greater than the requested number of frames.
      Objects requiring arbitrary vector sizes should explicity re-size
      the vector using the resize_exact() method. \n
     sr - sampling rate
  */
  AudioBase(uint32_t nchnls = def_nchnls, uint32_t vframes = def_vframes,
            double sr = def_sr)
      : m_nchnls(nchnls), m_vframes(npow2(vframes)),
        m_vector(m_vframes * nchnls, 0.0), m_sr(sr), m_error(0){};

  /** Scale the data vector
   */
  virtual const AudioBase &operator*=(double scal) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] *= scal;
    return *this;
  }

  /** Multiply the data vector by a sig vector
   */
  virtual const AudioBase &operator*=(const double *sig) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] *= sig[i];
    return *this;
  }

  /** Multiply the data vector by the vector from obj
   */
  virtual const AudioBase &operator*=(const AudioBase &obj) {
    if (m_vframes == obj.m_vframes && m_nchnls == obj.m_nchnls)
      return *this *= obj.vector();
    else
      return *this;
  }

  /** DC offset the data vector
   */
  virtual const AudioBase &operator+=(double offs) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] += offs;
    return *this;
  }

  /** Add a vector sig to the data vector
   */
  virtual const AudioBase &operator+=(const double *sig) {
    for (uint32_t i = 0; i < m_vector.size(); i++)
      m_vector[i] += sig[i];
    return *this;
  }

  /** Add a vector sig from obj to the data vector
   */
  virtual const AudioBase &operator+=(const AudioBase &obj) {
    if (m_vframes == obj.m_vframes && m_nchnls == obj.m_nchnls)
      return *this += obj.vector();
    else
      return *this;
  }

  /** Get a reference of
      a single sample at sample pos ndx
      off the data vector
  */
  double &operator[](uint32_t ndx) { return m_vector.at(ndx); }

  /** Get a constant reference of
      a single sample at sample pos ndx
       off the data vector
  */
  const double &operator[](uint32_t ndx) const { return m_vector.at(ndx); }

  /** iterator for this class
   */
  typedef std::vector<double>::iterator iterator;

  /** const iterator for this class
  */
  typedef std::vector<double>::const_iterator const_iterator;

  /** returns an iterator to the beginning
   */
  iterator begin() { return m_vector.begin(); }

  /** returns an iterator to the end
   */
  iterator end() { return m_vector.end(); }

  /** returns a const iterator to the beginning
   */
  const_iterator cbegin() const { return m_vector.cbegin(); }

  /** returns a const iterator to the end
   */
  const_iterator cend() const { return m_vector.cend(); }

  /** set the data vector to an input obj vector
      return the AudioBase obj reference
  */
  const AudioBase &set(const AudioBase &obj) {
    if (obj.vsamps() <= m_vector.size())
      std::copy(obj.cbegin(), obj.cend(), begin());
    return *this;
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
    } else
      return 0.;
  }

  /** Conversion operator for const std::vector<double>&
   */
  operator const std::vector<double> &() const { return m_vector; }

  /** Conversion operator for const double*
   */
  explicit operator const double *() const { return m_vector.data(); }

  /** Stream output operator
   */
  friend std::ostream &operator<<(std::ostream &os, const AudioBase &obj) {
    for (uint32_t i = 0; i < obj.m_vframes * obj.m_nchnls; i++)
      os << obj.m_vector[i] << std::endl;
    return os;
  }

  /** Stream input operator
   */
  friend std::istream &operator>>(std::istream &is, AudioBase &obj) {
    for (uint32_t i = 0; i < obj.m_vframes * obj.m_nchnls; i++)
      is >> obj.m_vector[i];
    return is;
  }

  /** Get the audio vector
   */
  const double *vector() const { return static_cast<const double *>(*this); }

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
      Size is set to next power-of-two no greater
      than the requested number of frames.
   */
  uint32_t vframes(uint32_t frames) {
    m_vframes = npow2(frames);
    m_vector.resize(m_vframes * m_nchnls);
    set(0.);
    return m_vframes;
  }

  /** Resize the vector to given number of frames,
      exactly. This is used to set the vector size
      to values other than power-of-two sizes, for
      dedicated applications where this is required.
      Clears the vector and returns the updated vector
      frame size.
  */
  uint32_t resize_exact(uint32_t frames) {
    m_vector.resize(frames * m_nchnls);
    m_vframes = frames;
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
