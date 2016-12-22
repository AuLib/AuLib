///////////////////////////////////////////////////////////////////
// Implementation of the FourierTable class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "FourierTable.h"

AuLib::FourierTable::FourierTable(uint32_t harms,
			   double *amps, double phase,
			   uint64_t tframes) :
  FuncTable(tframes)
{
  create(harms,amps,phase);
}

AuLib::FourierTable::FourierTable(uint32_t harms,
			   uint32_t type, uint64_t tframes) :
  FuncTable(tframes)
{
  std::vector<double> amps(harms,0.0);
  double phase = -0.25;
  switch(type) {
  case SAW:
    for(uint32_t i=0; i < harms; i++)
      amps[i] = 1.f/(i+1);
    break;
  case SQUARE:
    for(uint32_t i=0; i < harms; i+=2)
      amps[i] = 1.f/(i+1);
    break;
  case TRIANGLE:
    phase = 0.;
    for(uint32_t i=0; i < harms; i+=2)
      amps[i] = 1.f/((i+1)*(i+1));
    break;
  default:
    for(uint32_t i=0; i < harms; i+=1)
      amps[i] = 1.f;
  }
  create(harms,amps.data(),phase);
}

void
AuLib::FourierTable::create(uint32_t harms,
		     double *amps,
		     double phase)
{ 
  double w,a;
  if(harms) {
  phase *= twopi;
  for(uint32_t i=0; i < harms; i++) {
    for(uint32_t n=0; n < m_tframes; n++){
      a = amps != NULL ? amps[i] : 1.f;	   
      w = (i+1)*(n*twopi/m_tframes);
      m_vector[n] += (float) (a*cos(w+phase));
   }
  }
  normalise_table();
  m_vector[m_tframes] = m_vector[0];
  m_vector[m_tframes+1] = m_vector[1];
  }
}
