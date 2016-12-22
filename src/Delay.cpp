////////////////////////////////////////////////////////////////////
// Implementation of the Delay and derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Delay.h"
#include "Fir.h"
#include "AllPass.h"

const double*
AuLib::Delay::process(const double* sig){
  for(uint32_t i = 0; i < m_vframes; i++) {
    m_vector[i] = m_delay.set(sig[i] + m_delay.vector(m_pos)*m_fdb, m_pos);
    m_pos = m_pos == m_delay.vframes()-1 ? 0. : m_pos+1;
  }
  return vector();
}

const double*
AuLib::Delay::process(const double* sig, double dt, double fdb){
  uint32_t ds = dt < 0. ? 0 : dt*m_sr;
  int32_t  rp;
  m_fdb = fdb;
  if(ds > m_delay.vframes()) ds = m_delay.vframes();
  for(uint32_t i = 0; i < m_vframes; i++) {
      rp = m_pos - ds;
      if(rp < 0) rp += m_delay.vframes();
      m_vector[i] = m_delay.vector(rp);
      m_delay.set(sig[i] + m_vector[i]*m_fdb, m_pos);
      m_pos = m_pos == m_delay.vframes()-1 ? 0. : m_pos+1;
  }
  return vector();
}

const double*
AuLib::Delay::process(const double* sig, const double *dt, double fdb){
  double  rp,ds,a,b,frac;
  uint32_t irp;
  m_fdb = fdb;
  for(uint32_t i = 0; i < m_vframes; i++) {
        ds = dt[i] < 0. ? 0 : dt[i]*m_sr; 
        if(ds > m_delay.vframes()) ds = m_delay.vframes();
        rp = m_pos - ds;
        if(rp < 0) rp += m_delay.vframes();
        irp = (uint32_t) rp;
	frac = rp - irp;
	a = m_delay.vector(irp);
	if(++irp == m_delay.vframes()-1) irp = 0;
	b = m_delay.vector(irp);
        m_vector[i] =  a + frac*(b-a);
        m_delay.set(sig[i] + m_vector[i]*m_fdb, m_pos);
        m_pos = m_pos == m_delay.vframes()-1 ? 0. : m_pos+1;
  }
  return vector();
}

const double*
AuLib::AllPass::process(const double* sig){
  double y;
  for(uint32_t i = 0; i < m_vframes; i++) {
    y = sig[i] + m_fdb*m_delay.vector(m_pos);
    m_vector[i] = m_delay.set(y, m_pos) - m_fdb*y;
    m_pos = m_pos == m_delay.vframes()-1 ? 0. : m_pos+1;
  }
  return vector();
}

const double*
AuLib::Fir::process(const double *sig){ 
  double out=0; int32_t rp;
  for(uint32_t i=0; i < m_vframes; i++){
    for(uint32_t j=0; j < m_irsize; j++){
      rp = m_pos+j;
      rp = (rp >= 0 ? (rp < m_irsize ? rp : rp - m_irsize) : rp + m_irsize);  
      out += (m_delay.vector(rp)*m_ir[m_irsize-1-j]);
    }
    m_delay.set(sig[i], m_pos);
    m_vector[i] = out;
    out = 0.;
    m_pos = (m_pos != m_irsize-1 ? m_pos+1 : 0);
  }
  return vector();
}



