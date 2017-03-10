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

#include <AudioBase.h>
#include <atomic>

namespace AuLib {

class Circular : public AudioBase {

  std::atomic<uint32_t> m_samps;
  std::vector<double> m_buffer;
  std::vector<double>::iterator m_wpos;
  std::vector<double>::iterator m_rpos;

  bool dsp(const double *sig);
  const double *dsp();
  
public:
  Circular(uint32_t size, uint32_t vframes = def_vframes, double sr = def_sr) :
    AudioBase(1,vframes,sr), m_samps(0), m_buffer(npow2(size)), m_wpos(m_buffer.begin()),
    m_rpos(m_buffer.begin()) { };

  bool writes(const double *sig) {
    return dsp(sig);
  }

  const double *reads() {
    return dsp();
  }

  void write(const AudioBase &obj) {
    while(!dsp(obj.vector()));
  }

  const AudioBase &read() {
    while(!dsp());
    return *this;
  }

  void operator()(const AudioBase &obj) {
    dsp(obj.vector());
  }

  const AudioBase &operator()() {
    dsp();
    return *this;
  }
};

/*! \class Circular Circular.h AuLib/Circular.h
 */
  
}
