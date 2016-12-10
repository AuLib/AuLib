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

FourierTable::FourierTable(uint32_t harms,
			   double *amps, double phase,
			   uint32_t tsize) :
  FuncTable(tsize)
{
  create(harms,amps,phase);
}

FourierTable::FourierTable(uint32_t harms,
			   uint32_t type, uint32_t tsize) :
  FuncTable(tsize)
{
  double *amps = new double[harms];
  if(amps != NULL) {
  double phase = -0.25;
  memset(amps, 0, sizeof(double)*harms);
  switch(type) {
  case SAW_TABLE:
    for(int i=0; i < harms; i++)
      amps[i] = 1.f/(i+1);
    break;
  case SQUARE_TABLE:
    for(int i=0; i < harms; i+=2)
      amps[i] = 1.f/(i+1);
    break;
  case TRIANGLE_TABLE:
    phase = 0.;
    for(int i=0; i < harms; i+=2)
      amps[i] = 1.f/((i+1)*(i+1));
    break;
  default:
    for(int i=0; i < harms; i+=2)
      amps[i] = 1.f;
  }
  create(harms,amps,phase);
  delete[] amps;
  } else m_error = AULIB_MEM_ERROR;
}

void
FourierTable::create(uint32_t harms,
		     double *amps,
		     double phase)
{ 
  double w,a;
  if(amps != NULL) {
  phase *= twopi;
  for(int i=0; i < harms; i++)
    for(int n=0; n < m_tsize; n++){
      a = amps != NULL ? amps[i] : 1.f;	   
      w = (i+1)*(n*twopi/m_tsize);
      m_table[n] += (float) (a*cos(w+phase));
    }
  m_table[m_tsize] = m_table[0];
  normalise_table();
  }
  else m_error = AULIB_ERROR;
}
