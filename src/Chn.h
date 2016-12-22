/////////////////////////////////////////////////////////////////////
// Chn class: extracts a channel from a multichannel input
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _CHN_H_
#define _CHN_H_
#include "AudioBase.h"

namespace AuLib {

  /** Extracts a single channel from a multichannel
      interleaved input
  */
  class Chn : public AudioBase {

  protected:
    uint32_t m_chn;
  
  public:
    /** Chn constructor \n\n
	channel - channel to extract \n
	vframes - vector size \n
        sr - sampling rate
    */  
  Chn(uint32_t channel = 1, uint32_t vframes = def_vframes) :
    AudioBase(1,vframes), 
    m_chn(channel) { };

    /** extracts a channel from sig holding nchnls channels
     */
    virtual const double* process(const double* sig, uint32_t nchnls){
      if(m_chn && m_chn <= nchnls)
	for(uint32_t i = 0, j = m_chn-1;
	    i < m_vframes; i++, j += nchnls)
	  m_vector[i] = sig[j];
      else set(0.);
      return vector();
    }

    /** extracts channel chn from sig holding nchnls channels
     */
    virtual const double* process(const double* sig, uint32_t chn,
				  uint32_t nchnls) {
      m_chn = chn;
      return process(sig, nchnls);
    }

    /** extracts a channel from obj 
     */
    virtual const Chn& process(const AudioBase& obj) {
      if(obj.vframes() == m_vframes){
	process(obj.vector(), obj.nchnls());
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** extracts channel chn from obj 
     */
    virtual const Chn& process(const AudioBase& obj, uint32_t chn) {
      m_chn = chn;
      process(obj);
      return *this;
    }    

  };

  /*! \class Chn Chn.h AuLib/Chn.h
   */

}
#endif
