////////////////////////////////////////////////////////////////////
// Implementation of the Tone* classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "ToneHP.h"
#include "ToneLP.h"

void
AuLib::ToneLP::update()
{
  double costh = 2. - cos(2.*pi*m_freq/m_sr);
  m_b = sqrt(costh*costh - 1.) - costh;
  m_a = (1. + m_b);
}

void
AuLib::ToneHP::update()
{
  double costh = 2. - cos(2.*pi*m_freq/m_sr);
  m_b = costh - sqrt(costh*costh - 1.);
  m_a = (1. - m_b);
}
