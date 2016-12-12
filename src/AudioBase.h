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
#include <algorithm>
#include <utility>

namespace AuLib {
  
  /** Audio DSP base class
   */
  class AudioBase {

  protected:
    uint32_t m_nchnls;
    double m_sr;
    uint32_t m_vsize;
    double *m_output;
    uint32_t m_error;

  public:
    /** swap function for copy assignment 
     */
    friend void swap(AudioBase& obja,
		     AudioBase& objb) 
    {
      using std::swap;
      swap(obja.m_nchnls,objb.m_nchnls);
      swap(obja.m_vsize,objb.m_vsize);
      swap(obja.m_sr,objb.m_sr);
      swap(obja.m_error,objb.m_error);
      swap(obja.m_output,objb.m_output);
    }

    /* Copy assignment operator
     */
    AudioBase& operator=(AudioBase obj){ 
      swap(*this, obj);
      return *this;
    }

     /** AudioBase copy constructor 
     */
    AudioBase(const AudioBase& obj) :
      m_nchnls(obj.m_nchnls), m_sr(obj.m_sr),
      m_vsize(obj.m_vsize), m_error(obj.m_error){
      if(m_vsize != 0) {
       	try {
	    m_output = new double[m_vsize*m_nchnls];
	  } catch (std::bad_alloc){
	  m_error = AULIB_MEM_ERROR;
	  m_vsize = 0;
	  return;
	}
	memcpy(m_output,obj.m_output,
	       sizeof(double)*m_vsize);
      }
      else m_output = NULL;
    }
    
    /** AudioBase constructor \n\n
	nchnls - number of channels \n
	sr - sampling rate \n
	vsize - vector size (frames) \n
    */
    AudioBase(uint32_t nchnls = def_nchnls,
	      double vsize = def_vsize,
	      double sr = def_sr) :
      m_nchnls(nchnls), m_sr(sr),
      m_vsize(vsize), m_error(AULIB_NOERROR) {
      if(m_vsize != 0) {
	try {
	    m_output = new double[m_vsize*m_nchnls];
	  } catch (std::bad_alloc){
	  m_error = AULIB_MEM_ERROR;
	  m_vsize = 0;
	  return;
	}
	memset(m_output, 0, m_vsize*sizeof(double));
      } else m_output = NULL; 
    }
  
    /** AudioBase destructor
     */
    virtual ~AudioBase(){
      delete[] m_output;
    }

    /** Get the audio output vector
     */ 
    const double* output() const {
      return (const double *) m_output;
    }
  
    /** Get a single sample at ndx 
	off the output audio vector
    */  
    double output(uint32_t ndx) const {
      if(ndx < m_vsize)
	return m_output[ndx];
      else return 0.;
    }

    /** Get vector size
     */
    uint32_t vsize() const {
      return m_vsize;
    }
    
    /** Get number of channels
     */
    uint32_t nchnls() const {
      return m_nchnls;
    }

    /** Get sampling reate
     */
    uint32_t sr() const {
      return m_sr;
    }


    /** Get error code
     */
    uint32_t error() const {
      return m_error;
    }

    /** Get error message
     */
    virtual const char* error_message() const {
      return aulib_error[m_error];
    }
 
  };

  /*! \class AudioBase AudioBase.h AuLib/AudioBase.h
   */
}
#endif
