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
   
    std::vector<double> m_incrs;
    std::vector<uint32_t> m_durs;
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

    Segments() : m_start(0),m_incrs(0),
		 m_durs(0), m_nsegs(0),
		 m_linear(true), m_frames(0) { };
    

    /** Get the increments array
     */
    const double* incrs() const {
      return m_incrs.data();
    }

    /** Get the durations array
     */
    const uint32_t* durs() const {
      return m_durs.data();
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
    int32_t m_cseg;
    uint32_t m_rt;
    uint32_t m_cnt;
    uint32_t m_time;
    double m_incr;
    bool m_trig;
    Segments m_segs;
    
  public:
    /** Envel constructor \n\n
	segs - envelope segments
	rel - release time \n
	vframes - vector size \n
	sr - sampling rate
    */  
    Envel(const Segments& segs, double rel = 0.f,
	  uint32_t vframes = def_vframes,
	  double sr = def_sr) :
      m_segs(segs),
      m_rt(rel*sr), m_cnt(0), m_cseg(0),
      m_incr(segs.incrs()[0]),
      m_time(segs.durs()[0]), m_trig(false),
      AudioBase(1,vframes,sr){ };

    /** Envel constructor \n\n
	rel - release time \n
	vframes - vector size \n
	sr - sampling rate
    */ 
    Envel(double rel = 0.f,
	  uint32_t vframes = def_vframes,
	  double sr = def_sr) :
      m_rt(rel*sr), m_cnt(0), m_cseg(0),
      m_incr(0), m_time(0), m_trig(false),
      AudioBase(1,vframes,sr){ };
    
    /** process envelope
     */
    virtual const AudioBase& process();

    /** retrigger envelope
     */
    virtual void retrig(){
      m_y = m_segs.start();
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
      m_rt = rel*m_sr;
      m_incr = segs.incrs()[0];
      m_time = segs.durs()[0];
      m_segs = segs;
      retrig();
    }

    /** return the release duration in frames
     */
    uint32_t rframes() const {
      return m_rt;
    }
    
    /** return the env duration in frames
        (excluding release)
     */
    uint32_t frames() const {
      return m_segs.frames();
    }
    
  };

  /*! \class Envel Envel.h AuLib/Envel.h
   */

}
#endif
