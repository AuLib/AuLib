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
    double *m_vector;
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
      swap(obja.m_vector,objb.m_vector);
    }

    /* Copy assignment operator
     */
    const AudioBase& operator=(AudioBase obj){ 
      swap(*this, obj);
      return *this;
    }
 
    /** AudioBase copy constructor 
     */
    AudioBase(const AudioBase& obj);
    
    /** AudioBase constructor \n\n
	nchnls - number of channels \n
	sr - sampling rate \n
	vsize - vector size (frames) \n
    */
    AudioBase(uint32_t nchnls = def_nchnls,
	      double vsize = def_vsize,
	      double sr = def_sr);
  
    /** AudioBase destructor
     */
    virtual ~AudioBase(){
      if(m_vector)
	delete[] m_vector;
    }

    /** Scale the data vector
     */
    const AudioBase& operator*=(double scal){
      for(int i=0; i < m_vsize*m_nchnls;i++)
	m_vector[i] *= scal;
      return *this;
    }

    /** Multiply the data vector by a sig vector
     */
    const AudioBase& operator*=(const double *sig){
      for(int i=0; i < m_vsize*m_nchnls;i++)
	m_vector[i] *= sig[i];
      return *this;
    }

    /** Multiply the data vector by the vector from obj
     */
    const AudioBase& operator*=(const AudioBase& obj){
      if(m_vsize == obj.m_vsize &&
	 m_nchnls == obj.m_nchnls)
	return *this *= obj.vector();
      else return *this;
    }

    /** DC offset the data vector
     */
    const AudioBase& operator+=(double offs){
      for(int i=0; i < m_vsize*m_nchnls;i++)
	m_vector[i] += offs;
      return *this;
    }

    /** Add a vector sig to the data vector
     */
    const AudioBase& operator+=(const double *sig){
      for(int i=0; i < m_vsize*m_nchnls;i++)
	m_vector[i] += sig[i];
      return *this;
    }

    /** Add a vector sig from obj to the data vector
     */
    const AudioBase& operator+=(const AudioBase& obj){
      if(m_vsize == obj.m_vsize &&
	 m_nchnls == obj.m_nchnls)
	return *this += obj.vector();
      else return *this;
    }
    
    /** set the data vector to a sig vector
     */
    const AudioBase& set(const double *sig){
      memcpy(m_vector,sig,(m_vsize*m_nchnls)*sizeof(double));
      return *this;
    }

    /** set the data vector to a value v 
     */
    const AudioBase& set(double v){
      std::fill(m_vector,m_vector+m_vsize*m_nchnls,v);
      return *this;
    } 
    
    /** Get the audio vector vector
     */ 
    const double* vector() const {
      return m_vector;
    }
  
    /** Get a single sample at frame ndx
        and channel chn off the data vector
    */  
    double vector(uint32_t frndx, uint32_t chn=0)
      const {
      if(frndx < m_vsize)
	return m_vector[frndx*m_nchnls+chn];
      else return 0.;
    }

    /** Get a single sample at sample pos ndx 
	off the data vector
    */  
    double vector(uint32_t ndx) const {
      if(ndx < m_vsize*m_nchnls)
	return m_vector[ndx];
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
