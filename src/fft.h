////////////////////////////////////////////////////////////////////////////////
// fft.h: radix-2 (Cooley-Tukey) fft definitions
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _AULIB_FFT_H
#define _AULIB_FFT_H

#include "AuLib.h"
#include <cmath>
#include <complex>
#include <vector>

namespace AuLib {

namespace fft {

/** definition of forward
 */
const bool forward = true;

/** definition of inverse
 */
const bool inverse = false;

/** definition of polar
 */
const bool polar = true;

/** definition of rectang
 */
const bool rectang = false;

/** definition of packed
 */
const bool packed = true;
  

/** In-place complex-to-complex FFT \n
    data - data to be transformed  \n
    dir - true for forward operation, false for inverse \n
    pckd - true for packed data (first point is DC,Nyq)
    Size of data is expected to be a power-of-two.
 */
  void transform(std::vector<std::complex<double>> &data, bool dir);

/** Real-to-complex forward FFT (size N) \n
    in - pointer to real array of size N or N + 1 \n
    out - complex vector of size N/2 or N/2 + 1 \n
    pckd - true for packed data. 
    Size of complex out array is expected to be a power-of-two (pckd = fft::packed) 
    or power-of-two + 1 (pckd = !fft::packed)
    In packed form im(0) contains the Nyquist coefficient \n
    The in and out data may reside in the same memory location 
    for an in-place transform (but should have enough memory to hold N/2 or N/2 + 1
     complex numbers).
*/
  void transform(std::vector<std::complex<double>> &out, double *in, bool pckd = true);

/** Complex-to-real inverse FFT (size N) \n
    in - complex vector of size N/2 or N/2 + 1 \n
    with im(0) containing the Nyquist coefficient \n
    out - real array of size N or N + 1 \n
    Size of complex in array is expected to be a power-of-two (pckd = fft::packed) 
    or power-of-two + 1 (pckd = !fft::packed)
    In packed form im(0) contains the Nyquist coefficient \n
    The in and out data may reside in the same memory location 
    for an in-place transform (but should have enough memory to hold N/2 or N/2 + 1
     complex numbers).
*/
  void transform(double *out, std::vector<std::complex<double>> &in, bool pckd = true);
}
}

#endif
