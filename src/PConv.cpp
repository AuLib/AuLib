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

AuLib::PConv::PConv(const FuncTable &ir, uint32_t psize, uint32_t chn,
                    uint32_t begin, uint32_t end, uint32_t vframes, double sr)
    : AudioBase(1, vframes, sr), m_count(0), m_p(0), m_psize(npow2(psize)),
      m_nparts(std::ceil((end ? npow2(end - begin) : ir.vframes() -
		     (begin ? npow2(begin) :  0)) / (double) m_psize)),
      m_algo(PConv::OLA), m_in(m_psize * 2), m_out(m_psize * 2),
      m_saved(m_psize),
      m_part(m_nparts, std::vector<std::complex<double>>(m_psize + 1)),
      m_del(m_nparts, std::vector<std::complex<double>>(m_psize + 1)),
      m_mix(m_psize + 1) {
  update(ir, chn, begin, end);
}

AuLib::PConv::PConv(uint32_t algo, const FuncTable &ir, uint32_t psize,
                    uint32_t chn, uint32_t begin, uint32_t end,
                    uint32_t vframes, double sr)
    : AudioBase(1, vframes, sr), m_count(0), m_p(0), m_psize(npow2(psize)),
      m_nparts(
          std::ceil((end ? npow2(end - begin) : ir.vframes() - npow2(begin)) /
                    (double)m_psize)),
      m_algo(algo), m_in(m_psize * 2), m_out(m_psize * 2), m_saved(m_psize),
      m_part(m_nparts, std::vector<std::complex<double>>(m_psize + 1)),
      m_del(m_nparts, std::vector<std::complex<double>>(m_psize + 1)),
      m_mix(m_psize + 1) {
  update(ir, chn, begin, end);
}

void AuLib::PConv::update(const FuncTable &ir, uint32_t chn, uint32_t begin,
                          uint32_t end) {
  uint32_t nchnls = ir.nchnls();
  if (chn >= nchnls)
    chn = nchnls - 1;
  auto src = ir.cbegin() + chn + npow2(begin) * nchnls;
  auto fin = ir.cend() -
             (ir.vframes() - (end > 0 ? npow2(end) : ir.vframes())) * nchnls;
  for (auto &part : m_part) {
    for (auto in = m_in.begin(); in != m_in.begin() + m_psize;
         in++, src += nchnls) {
      *in = (src < fin ? *src : 0.) * m_psize;
    }
    fft::transform(part, m_in.data(), !fft::packed);
    std::fill(m_in.begin(), m_in.end(), 0.0);
  }
}

void AuLib::PConv::convolution() {
  fft::transform(m_del[m_p], m_in.data(), !fft::packed);
  std::fill(m_mix.begin(), m_mix.end(), 0.);
  m_p = m_p == m_nparts - 1 ? 0 : m_p + 1;
  auto del = m_del.begin() + m_p;
  for (auto part = m_part.rbegin(); part != m_part.rend(); part++, del++) {
    if (del == m_del.end())
      del = m_del.begin();
    auto dsamp = del->begin();
    auto psamp = part->begin();
    for (auto &mix : m_mix)
      mix += *dsamp++ * *psamp++;
  }
  fft::transform(m_out.data(), m_mix, !fft::packed);
}

const double *AuLib::PConv::ola(const double *sig) {
  for (uint32_t n = 0; n < m_vframes; n++) {
    m_in[m_count] = sig[n];
    m_vector[n] = m_out[m_count] + m_saved[m_count];
    m_saved[m_count] = m_out[m_count + m_psize];
    if (++m_count == m_psize) {
      convolution();
      m_count = 0;
    }
  }
  return vector();
}

const double *AuLib::PConv::ols(const double *sig) {
  for (uint32_t n = 0; n < m_vframes; n++) {
    m_in[m_psize + m_count] = sig[n];
    m_vector[n] = m_out[m_psize + m_count];
    if (++m_count == m_psize) {
      convolution();
      m_count = 0;
      std::copy(m_in.begin() + m_psize, m_in.end(), m_in.begin());
    }
  }
  return vector();
}
