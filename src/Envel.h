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

  class Segments {

    NONCOPYABLE(Segments);
    
    double *m_incrs;
    uint32_t *m_durs;
    double m_start;
    uint32_t m_nsegs;
    bool m_linear;
    uint32_t m_frames;
    
  public:
    
    /* Segments constructor
       start - start value
       endpts - array of segment endpoints
       times - array of segment time durations
       nsegs - number of segments
       linear - true if linear envelope, false is exponential
       sr - sampling rate
    */
    Segments(double start,
	     const double *endpts,
	     const double *times,
	     uint32_t nsegs,
	     bool linear = true,
	     double sr = def_sr);
    
    /** Segments destructor
     */
    ~Segments() {
      delete[] m_incrs;
      delete[] m_durs;
    }

    /** Get the increments array
     */
    const double* incrs() const {
      return m_incrs;
    }

    /** Get the durations array
     */
    const uint32_t* durs() const {
      return m_durs;
    }
    
    /** Get the number of segments
     */    
    uint32_t nsegs() const {
      return m_nsegs;
    }

    /** Get the start value
     */  
    double start() const {
      return m_start;
    }

    /** check if the envelope is designed
        to be linear or else exponential
    */  
    bool isLinear() const {
      return m_linear;
    }
    
    /** Get the envelope duration in frames
     */  
    uint32_t frames() const {
      return m_frames;
    }
    
  };
  

  /** Envel description
   */
  class Envel : public AudioBase {

  protected:
    double m_y;
    double m_y0;
    int32_t m_cseg;
    uint32_t m_nsegs;
    uint32_t m_rt;
    uint32_t m_cnt;
    uint32_t m_time;
    const double *m_incrs;
    const uint32_t *m_times;
    double m_incr;
    bool m_trig;
    bool m_linear;
    
  public:
    /** Envel constructor \n\n
	segs - envelope segments
	rel - release time \n
	vsize - vector size \n
	sr - sampling rate
    */  
    Envel(const Segments& segs, double rel = 0.f,
	  uint32_t vsize = def_vsize,
	  double sr = def_sr) :
      m_y(segs.start()), m_y0(segs.start()), m_nsegs(segs.nsegs()),
      m_rt(rel*sr), m_cnt(0), m_cseg(0), m_incr(segs.incrs()[0]),
      m_incrs(segs.incrs()), m_time(segs.durs()[0]),
      m_times(segs.durs()), m_linear(segs.isLinear()),
      m_trig(false),  AudioBase(1,vsize,sr)
    {
      if(m_nsegs == 0) {
	m_vsize = 0;
	m_error = AULIB_ERROR;
      }
    };
    /** Envel constructor \n\n
	start - start value \n
	incrs - array of segment increments \n
	times - array of segment durations (frames) \n
	nsegs - number of segments \n
	rel - release time \n
	linear - true if linear envelope, false is exponential \n
	vsize - vector size \n
	sr - sampling rate
    */  
    Envel(double start, const double* incrs,
	  const uint32_t *times, uint32_t nsegs, double rel = 0.f,
	  bool linear = true, uint32_t vsize = def_vsize,
	  double sr = def_sr) :
      m_y(start), m_y0(start), m_nsegs(nsegs),
      m_rt(rel*sr), m_cnt(0), m_cseg(0),
      m_incr(0.), m_incrs(incrs), m_time(0),
      m_times(times), m_linear(linear),
      m_trig(false), AudioBase(1,vsize,sr)
    {
      if(m_nsegs == 0) {
	m_vsize = 0;
	m_error = AULIB_ERROR;
      }
    };

    /** process envelope
     */
    virtual const AudioBase& process();

    /** retrigger envelope
     */
    virtual void retrig(){
      m_y = m_y0;
      m_cnt = 0;
      m_cseg = 0;
    }

    /** trigger release 
     */
    void release() {
      m_trig = true;
    }

    /** reset parameters and retrigger
     */    
    void reset(const Segments& segs,
	       double rel = 0.f){
      m_y0 = segs.start();
      m_rt = rel*m_sr;
      m_incr = segs.incrs()[0];
      m_incrs = segs.incrs();
      m_time = segs.durs()[0];
      m_times = segs.durs();
      m_linear = segs.isLinear();
      m_nsegs = segs.nsegs();
      retrig();
    }

    /** return the release duration in frames
     */
    uint32_t rframes() const {
      return m_rt;
    }
    
  };

  /*! \class Envel Envel.h AuLib/Envel.h
   */

}
#endif
