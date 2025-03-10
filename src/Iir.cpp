/////////////////////////////////////////////////////////////////////////////////
// Implementation of the Iir and Iir-derived classes
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////////////////
#include "Iir.h"
#include "BandP.h"
#include "BandR.h"
#include "HighP.h"
#include "LowP.h"
#include "Reson.h"
#include "ResonR.h"

const double *AuLib::Iir::dsp(const double *sig) {
  double w, y;

  for (uint32_t i = 0; i < m_vframes; i++) {
    w = m_scal * sig[i] - m_b[0] * m_del[0] - m_b[1] * m_del[1];
    y = w * m_a[0] + m_a[1] * m_del[0] + m_a[2] * m_del[1];
    m_del[1] = m_del[0];
    m_del[0] = w;
    m_vector[i] = y;
  }
  return vector();
}

void AuLib::LowP::update() {
  double l = 1/tan(pi * m_freq / m_sr);
  double sqrt2l = sqrt(2.) * l;
  double lsq = l * l;
  m_a[0] = 1. / (1. + sqrt2l + lsq);
  m_a[1] = 2. * m_a[0];
  m_a[2] = m_a[0];
  m_b[0] = 2. * (1. - lsq) * m_a[0];
  m_b[1] = (1. - sqrt2l + lsq) * m_a[0];
}

void AuLib::HighP::update() {
  double l = tan(pi * m_freq / m_sr);
  double sqrt2l = sqrt(2.) * l;
  double lsq = l * l;
  m_a[0] = 1. / (1. + sqrt2l + lsq);
  m_a[1] = -2. * m_a[0];
  m_a[2] = m_a[0];
  m_b[0] = 2. * (lsq - 1.) * m_a[0];
  m_b[1] = (1. - sqrt2l + lsq) * m_a[0];
}

void AuLib::BandP::update() {
  double l = 1. / tan(pi * m_bw / m_sr);
  double cosl = 2. * cos(2 * pi * m_freq / m_sr);
  m_a[0] = 1. / (1. + l);
  m_a[1] = 0;
  m_a[2] = -m_a[0];
  m_b[0] = -l * cosl * m_a[0];
  m_b[1] = (l - 1.) * m_a[0];
}

void AuLib::BandR::update() {
  double l = tan(pi * m_bw / m_sr);
  double cosl = 2. * cos(2 * pi * m_freq / m_sr);
  m_a[0] = 1. / (1. + l);
  m_a[1] = -cosl * m_a[0];
  m_a[2] = m_a[0];
  m_b[0] = m_a[1];
  m_b[1] = (1. - l) * m_a[0];
}

void AuLib::ResonR::update() {
  double r, rsq, rr, costh;
  r = exp(-m_bw * pi / m_sr);
  rr = 2. * r;
  rsq = r * r;
  costh = (rr / (1. + rsq)) * cos(2 * pi * m_freq / m_sr);
  m_scal = (1 - rsq) * sin(acos(costh));
  m_b[0] = -rr * costh;
  m_b[1] = rsq;
  m_a[2] = -r;
  m_a[0] = 1.;
  m_a[1] = 0;
}

const double *AuLib::Reson::dsp(const double *sig) {
  double y;
  for (uint32_t i = 0; i < m_vframes; i++) {
    y = sig[i] * m_scal - m_b[0] * m_del[0] - m_b[1] * m_del[1];
    m_del[1] = m_del[0];
    m_vector[i] = m_del[0] = y;
  }
  return vector();
}
