/////////////////////////////////////////////////////////////////////
// Envel class: multi-segment envelope generator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _ENVEL_H_
#define _ENVEL_H_
#include "AudioBase.h"

namespace AuLib {

/** Curve segments for envelope generators and
    tables
*/
class Segments {

  double m_start;
  uint32_t m_nsegs;
  bool m_linear;
  uint32_t m_frames;
  std::vector<double> m_incrs;
  std::vector<uint32_t> m_durs;
  std::vector<double> m_endpts;

public:
  /* Segments constructor \n\n
     start - start value \n
     endpts - array of segment endpoints \n
     times - array of segment time durations \n
     nsegs - number of segments \n
     linear - true if linear envelope, false is exponential \n
     sr - sampling rate \n
  */
  Segments(double start, const std::vector<double> endpts,
           const std::vector<double> times, bool linear = true,
           double sr = def_sr);

  Segments()
      : m_start(0), m_nsegs(0), m_linear(true), m_frames(0), m_incrs(0),
        m_durs(0), m_endpts(0){};

  /** Get the increments array
   */
  const double *incrs() const { return m_incrs.data(); }

  /** Get the durations array
   */
  const uint32_t *durs() const { return m_durs.data(); }

  /** Get the endpts array
   */
  const double *endpts() const { return m_endpts.data(); }

  /** Get the number of segments
   */
  uint32_t nsegs() const { return m_nsegs; }

  /** Get the start value
   */
  double start() const { return m_start; }

  /** check if the envelope is designed
      to be linear or else exponential
  */
  bool isLinear() const { return m_linear; }

  /** Get the envelope duration in frames
   */
  uint32_t frames() const { return m_frames; }
};

/** Multi-segment envelope generator
 */
class Envel : public AudioBase {

  virtual const AudioBase &dsp();

protected:
  double m_y;
  int32_t m_cseg;
  uint32_t m_rt;
  uint32_t m_cnt;
  uint32_t m_time;
  double m_incr;
  bool m_trig;
  bool m_releasing;
    int32_t m_rcnt;
  Segments m_segs;

public:
  /** Envel constructor \n\n
      segs - envelope segments \n
      rel - release time \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Envel(const Segments &segs, double rel = 0.f, uint32_t vframes = def_vframes,
        double sr = def_sr)
      : AudioBase(1, vframes, sr), m_y(segs.start()), m_cseg(0), m_rt(rel * sr), m_cnt(0),
        m_time(segs.durs()[0]), m_incr(segs.incrs()[0]), m_trig(false), m_releasing(false), m_rcnt(rel * sr),
        m_segs(segs){};

  /** Envel constructor \n\n
      rel - release time \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  Envel(double rel = 0.f, uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes, sr), m_y(0.), m_cseg(0), m_rt(rel * sr), m_cnt(0),
        m_time(0), m_incr(0), m_trig(false),  m_releasing(false), m_rcnt(rel * sr) {};

  /** process envelope
   */
  virtual const AudioBase &process() { return dsp(); }

  /** retrigger envelope
   */
  virtual void retrig() {
    m_time = m_segs.durs()[0];
    m_incr = m_segs.incrs()[0];
    m_cnt = 0;
    m_cseg = 0;
    m_rcnt = m_rt;
    m_trig = false;
    m_releasing = false;
  }

  /** trigger release
   */
  void release() { m_trig = true; }

  /** reset parameters and retrigger
   */
  void reset(const Segments &segs, double rel = 0.f) {
    m_rt = rel * m_sr;
    m_segs = segs;
    retrig();
  }

  /** return the release duration in frames
   */
  uint32_t rframes() const { return m_rt; }

  /** return the env duration in frames
      (excluding release)
  */
  uint32_t frames() const { return m_segs.frames(); }
};

/*! \class Envel Envel.h AuLib/Envel.h
 */
}
#endif
