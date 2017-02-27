/////////////////////////////////////////////////////////////////////
// Line class: linear signal generator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _LINE_H_
#define _LINE_H_
#include "AudioBase.h"

namespace AuLib {

/** Generates a signal based on a linear curve between two
    points over a given duration.
*/
class Line : public AudioBase {

protected:
  double m_y;
  double m_y0;
  double m_y1;
  uint32_t m_x1;
  double m_incr;
  uint64_t m_cnt;

  /** process the output vector
   */
  virtual void dsp() {
    for (uint32_t i = 0; i < m_vframes; i++) {
      m_vector[i] = m_y;
      if (m_cnt < m_x1) {
        m_y += m_incr;
        m_cnt++;
      } else
        m_y = m_y1;
    }
  }

  virtual void restart() {
    m_cnt = 0;
    m_y = m_y0;
    m_incr = (m_y1 - m_y0) / m_x1;
  }

public:
  /** Line constructor \n\n
      start - start value \n
      end - end value \n
      time - duration(s) \n
      vframes - vector size \n
      sr - sampling rate
  */
  Line(double start = .0, double end = 1., double time = 1.,
       uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr), m_y(start), m_y0(start), m_y1(end),
        m_x1(time * sr), m_incr((end - start) / (time * sr)), m_cnt(0){};

  /** process and return a reference to the object
   */
  const Line &process() {
    dsp();
    return *this;
  }

  /** operator () convenience method
   */
  const Line &operator()(){
    return process();
  }

  /** retrigger
   */
  void retrig() { restart(); }

  /** reset and retrigger
   */
  void reset(double start, double end, double time) {
    m_y0 = start;
    m_y1 = end;
    m_x1 = time * m_sr;
    restart();
  }
};

/*! \class Line Line.h AuLib/Line.h
 */
}
#endif
