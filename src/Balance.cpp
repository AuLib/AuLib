////////////////////////////////////////////////////////////////////
// Implementation of the balance class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "Balance.h"

const double*
AuLib::Balance::process(const double* sig, const double *cmp){
  const double *rms_cmp = m_cmp.process(cmp);
  const double *rms_sig = m_sig.process(sig);
  for(uint32_t i=0; i < m_vframes; i++){
    m_vector[i] = sig[i]*(rms_sig[i] > 0.0 ?
			  rms_cmp[i]/rms_sig[i] : 1.);
  }
  return vector();
}
