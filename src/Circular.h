////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// Circular.h: circular buffer class
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#include "AudioBase.h"
#include <atomic>

#ifndef __CIRCULAR_H__
#define __CIRCULAR_H__
namespace AuLib {

/** Circular buffer: implements a buffer
    with atomic access
*/
class Circular final : public AudioBase {

  std::atomic<uint32_t> m_samps;
  std::vector<double> m_buffer;
  std::vector<double>::iterator m_wpos;
  std::vector<double>::iterator m_rpos;

  bool dsp(const double *sig);
  const double *dsp();

public:
  /** Constructs a circular buffer with a given size in frames
   */
  Circular(uint32_t size, uint32_t nchnls = def_nchnls,
           uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(nchnls, vframes, sr), m_samps(0),
        m_buffer(npow2(size) * nchnls), m_wpos(m_buffer.begin()),
        m_rpos(m_buffer.begin()){};

  /** Writes a block of vframes() samples into the circular buffer.
      There should be at least vframes() samples in sig. If the
      buffer is full, nothing is written and the function returns false.
      Returns true on success.
   */
  bool writes(const double *sig) { return dsp(sig); }

  /** Reads a block of vframes() samples from the circular buffer into
     the object vector. If the buffer is empty, nothing is written and the
     function returns a nullptr. Otherwise, it returns a pointer to
      the vector.
  */
  const double *reads() { return dsp(); }

  /** Writes a block of frames from obj into the circular buffer
   */
  void write(const AudioBase &obj) {
    if (m_vframes == obj.vframes() && m_nchnls == obj.nchnls())
      while (!dsp(obj.vector()))
        ;
  }

  /** Reads a block of frames from the circular buffer, returns
      a reference to this object, which contains the output.
   */
  const AudioBase &read() {
    while (!dsp())
      ;
    return *this;
  }

  /** Convenience operator(), same as write()
   */
  void operator()(const AudioBase &obj) { write(obj); }

  /** Convenience operator(), same as read()
   */
  const AudioBase &operator()() { return read(); }

  /** Circular buffer size in frames
   */
  uint32_t size() const { return (uint32_t) m_buffer.size() / m_nchnls; }

  /** check if buffer is empty
   */
  bool is_empty() const { return m_samps == 0 ? true : false; }
};

/*! \class Circular Circular.h AuLib/Circular.h
 */
}
#endif
