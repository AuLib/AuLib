/////////////////////////////////////////////////////////////////////
// Implementation of the EnvelTable class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "EnvelTable.h"

AuLib::EnvelTable::EnvelTable(const Segments& segs,
			      bool norm)
  : FuncTable(){

  bool linear = segs.isLinear();
  uint32_t nsegs = segs.nsegs();
  double y = segs.start();
  double incr = segs.incrs()[0];
  double time = segs.durs()[0];
  int32_t cseg = 0;
  uint32_t cnt = 0;
  
  m_tframes = segs.frames();
  m_vector.resize(m_tframes+2);
  m_vframes = m_vector.size();
  for(uint32_t i=0; i < m_vframes; i++) {
    m_vector[i] = y;
    if(linear) y += incr;
    else y *= incr;
    if(cseg >= 0) cnt++;
    if(cnt == time && cseg >= 0) {
      cseg++;
      if(cseg < nsegs) {
	y = segs.endpts()[cseg-1];
	incr = segs.incrs()[cseg];
	time = segs.durs()[cseg];
	cnt = 0;
      }
      else {
	y = segs.endpts()[cseg-1];
	incr = linear ? 0. : 1.;
        cseg = -1;
      }
    }
  }
  if(norm)
    normalise_table();
}
