////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// PConv.cpp: partitioned convolution implementation
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#include <PConv.h>

AuLib::PConv::PConv(const FuncTable &ir, uint32_t psize, uint32_t vframes,
                    double sr)
    : AudioBase(1, vframes, sr), m_count(0), m_p(0), m_psize(npow2(psize)),
      m_nparts(std::ceil((double)ir.vframes() / m_psize)), m_in(m_psize * 2),
      m_out(m_psize * 2), m_saved(m_psize),
      m_part(m_nparts, std::vector<std::complex<double>>(m_psize)),
      m_del(m_nparts, std::vector<std::complex<double>>(m_psize)),
      m_mix(m_psize) {
  auto src = ir.cbegin();
  for (auto &part : m_part) {
    for (auto in = m_in.begin(); in != m_in.begin() + m_psize; in++)
      *in = src != ir.cend() ? *src++ : 0.;
    fft::transform(part, m_in.data());
  }
}

const double *AuLib::PConv::dsp(const double *sig) {

  for (uint32_t n = 0; n < m_vframes; n++) {
    m_in[m_count] = sig[n];
    m_vector[n] = m_out[m_count] + m_saved[m_count];
    m_saved[m_count] = m_out[m_count + m_psize];
    if (++m_count == m_psize) {
      fft::transform(m_del[m_p], m_in.data());
      std::fill(m_mix.begin(), m_mix.end(), 0.);
      m_p = m_p == m_nparts - 1 ? 0 : m_p + 1;
      auto del = m_del.begin() + m_p;
      for (auto part = m_part.rbegin(); part != m_part.rend(); part++, del++) {
        auto psamp = part->begin();
        if (del == m_del.end())
         del = m_del.begin();
        auto dsamp = del->begin();
	for (auto &mix : m_mix){
          mix += (*psamp++) *  (*dsamp++);
	}
           
       }
      fft::transform(m_out.data(), m_mix);
      m_count = 0;
    }
  }
  return vector();
}
