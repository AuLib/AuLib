////////////////////////////////////////////////////////////////////
// Implementation of radix-2 (Cooley-Tukey) fft
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "fft.h"

void AuLib::fft::reorder(std::vector<std::complex<double>> &s) {
  size_t N = s.size();
  uint32_t j = 0, m;
  for (int i = 0; i < N; i++) {
    if (j > i) {
      std::swap(s[i], s[j]);
    }
    m = N / 2;
    while (m >= 2 && j >= m) {
      j -= m;
      m /= 2;
    }
    j += m;
  }
}

void AuLib::fft::transform(std::vector<std::complex<double>> &s, bool fwd) {
  size_t N = s.size();
  std::complex<double> wp, w, even, odd;
  double o;
  uint32_t i;
  reorder(s);
  for (uint32_t n = 1; n < N; n *= 2) {
    o = fwd ? pi / n : -pi / n;
    wp.real(cos(o)), wp.imag(sin(o));
    w = 1.;
    for (uint32_t m = 0; m < n; m++) {
      for (uint32_t k = m; k < N; k += n * 2) {
        i = k + n;
        even = s[k];
        odd = w * s[i];
        s[k] = even + odd;
        s[i] = even - odd;
      }
      w *= wp;
    }
  }
  if (fwd)
    for (uint32_t n = 0; n < N; n++)
      s[n] /= N;
}

void AuLib::fft::transform(std::vector<std::complex<double>> &c, double *r) {
  size_t N = c.size();
  std::complex<double> wp, w = 1., even, odd;
  double o, zro, nyq;
  double *s = reinterpret_cast<double *>(c.data());
  if (s != r)
    std::copy(r, r + 2 * N, s);
  transform(c, true);
  zro = c[0].real() + c[0].imag();
  nyq = c[0].real() - c[0].imag();
  c[0].real(zro * .5), c[0].imag(nyq * .5);
  o = pi / N;
  wp.real(cos(o)), wp.imag(sin(o));
  w *= wp;
  for (uint32_t i = 1, j = 0; i < N / 2; i++) {
    j = N - i;
    even = .5 * (c[i] + conj(c[j]));
    odd = .5 * _j * (conj(c[j]) - c[i]);
    c[i] = even + w * odd;
    c[j] = conj(even - w * odd);
    w *= wp;
  }
}

void AuLib::fft::transform(double *r, std::vector<std::complex<double>> &c) {
  size_t N = c.size();
  std::complex<double> wp, w = 1., even, odd;
  double o, zro, nyq;
  double *s = reinterpret_cast<double *>(c.data());
  zro = c[0].real(), nyq = c[0].imag();
  c[0].real(zro + nyq), c[0].imag(zro - nyq);
  o = -pi / N;
  wp.real(cos(o)), wp.imag(cos(o));
  w *= wp;
  for (uint32_t i = 1, j = 0; i < N / 2 + 1; i++) {
    j = N - i;
    even = .5 * (c[i] + conj(c[j]));
    odd = .5 * _j * (c[i] - conj(c[j]));
    c[i] = even + w * odd;
    c[j] = conj(even - w * odd);
    w *= wp;
  }
  transform(c, false);
  if (s != r)
    std::copy(s, s + 2 * N, r);
}
