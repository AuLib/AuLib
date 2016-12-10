/////////////////////////////////////////////////////////////////////
// Implementation of the SigBus class: signal bus, scaling, offset
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "SigBus.h"

void SigBus::process(const double *sig) {
  for(int i=0; i < m_vsize*m_nchnls; i++)
    m_output[i] += sig[i];
}

void SigBus::process(const double *sig, double scal, double offs,
		     bool overwrite) {
  for(int i=0; i < m_vsize*m_nchnls; i++) 
    m_output[i] = overwrite ? sig[i]*scal + offs :
      sig[i]*scal + offs + m_output[i];     
}
