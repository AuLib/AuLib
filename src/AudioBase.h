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
    double m_sr;
    double m_vframes;
    std::vector<double> m_vector;
    uint32_t m_error;

  public:
    AudioBase(uint32_t nchnls = def_nchnls,
	      double vframes = def_vframes,
	      double sr = def_sr) :
      m_nchnls(nchnls),
      m_vframes(vframes),
      m_vector(vframes*nchnls,0.0),
      m_error(0), m_sr(sr) { };
    
    /** Scale the data vector
     */
    const AudioBase& operator*=(double scal){
      for(int i=0; i < m_vector.size();i++)
	m_vector[i] *= scal;
      return *this;
    }

    /** Multiply the data vector by a sig vector
     */
    const AudioBase& operator*=(const double *sig){
      for(int i=0; i < m_vector.size();i++)
	m_vector[i] *= sig[i];
      return *this;
    }

    /** Multiply the data vector by the vector from obj
     */
    const AudioBase& operator*=(const AudioBase& obj){
      if(m_vframes == obj.m_vframes &&
	 m_nchnls == obj.m_nchnls)
	return *this *= obj.vector();
      else return *this;
    }

    /** DC offset the data vector
     */
    const AudioBase& operator+=(double offs){
      for(int i=0; i < m_vector.size();i++)
	m_vector[i] += offs;
      return *this;
    }

    /** Add a vector sig to the data vector
     */
    const AudioBase& operator+=(const double *sig){
      for(int i=0; i < m_vector.size();i++)
	m_vector[i] += sig[i];
      return *this;
    }

    /** Add a vector sig from obj to the data vector
     */
    const AudioBase& operator+=(const AudioBase& obj){
      if(m_vframes == obj.m_vframes &&
	 m_nchnls == obj.m_nchnls)
	return *this += obj.vector();
      else return *this;
    }
    
    /** set the data vector to a sig vector
     */
    const AudioBase& set(const double *sig){
      const double *end = sig+m_vector.size();
      std::copy(sig,end,m_vector.data());
      return *this;
    }

    /** set the data vector to a value v 
     */
    const AudioBase& set(double v){
      std::fill(m_vector.begin(),m_vector.end(),v);
      return *this;
    } 
    
    /** Get the audio vector vector
     */ 
    const double* vector() const {
      return m_vector.data();
    }
  
    /** Get a single sample at frame ndx
        and channel chn off the data vector
    */  
    double vector(uint32_t frndx, uint32_t chn=0)
      const {
      if(frndx < m_vframes)
	return vector()[frndx*m_nchnls+chn];
      else return 0.;
    }

    /** Get a single sample at sample pos ndx 
	off the data vector
    */  
    double vector(uint32_t ndx) const {
      if(ndx < m_vframes*m_nchnls)
	return vector()[ndx];
      else return 0.;
    }

    /** Get vector size in frames
     */
    uint32_t vframes() const {
      return m_vframes;
    }

   /** Get vector size in samples
     */
    uint32_t vsamps() const {
      return m_vector.size();
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
