/////////////////////////////////////////////////////////////////////
// Implementation of the AudioBase class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "AudioBase.h"

AuLib::AudioBase::AudioBase(const AudioBase& obj) :
  m_nchnls(obj.m_nchnls), m_sr(obj.m_sr),
  m_vsize(obj.m_vsize), m_error(obj.m_error)
{
  if(m_vsize != 0) {
    try {
      m_vector = new double[m_vsize*m_nchnls];
    } catch (std::bad_alloc){
      m_error = AULIB_MEM_ERROR;
      m_vsize = 0;
      m_vector = NULL;
      return;
    }
    memcpy(m_vector,obj.m_vector,
	   sizeof(double)*m_vsize);
  }
  else m_vector = NULL;
}
    
AuLib::AudioBase::AudioBase(uint32_t nchnls,
		 double vsize, double sr) :
  m_nchnls(nchnls), m_sr(sr),
  m_vsize(vsize), m_error(AULIB_NOERROR)
{
  if(m_vsize != 0) {
    try {
      m_vector = new double[m_vsize*m_nchnls];
    } catch (std::bad_alloc){
      m_error = AULIB_MEM_ERROR;
      m_vsize = 0;
      m_vector = NULL;
      return;
    }
    memset(m_vector, 0, m_vsize*sizeof(double));
  } else m_vector = NULL; 
}
