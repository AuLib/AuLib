////////////////////////////////////////////////////////////////////
// fft.h: radix-2 (Cooley-Tukey) fft definitions
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _AULIB_FFT_H
#define _AULIB_FFT_H

#include "AuLib.h"
#include <cmath>
#include <complex>
#include <vector>

namespace AuLib {

namespace fft {

/** In-place complex-to-complex FFT \n
    data - data to be transformed  \n
    fwd - true for forward operation, false for inverse \n
    Size of data is expected to be a power-of-two.
 */
void transform(std::vector<std::complex<double>> &data, bool fwd = true);

/** Real-to-complex forward FFT (size N) \n
    in - pointer to real array of size N \n
    out - complex vector of size N/2 \n
    im(0) contains the Nyquist coefficient \n
    N is expected to be a power-of-two and the in and out data
    may reside in the same memory location for an in-place transform
*/
void transform(std::vector<std::complex<double>> &out, double *in);

/** Complex-to-real inverse FFT \n
    in - complex vector of size N/2 \n
    with im(0) containing the Nyquist coefficient \n
    out - real array of  \n
    N is expected to be a power-of-two and the in and out data
    may reside in the same memory location for an in-place transform
*/
void transform(double *out, std::vector<std::complex<double>> &in);

/** imaginary constant
 */
const std::complex<double> _j(0., 1.);
}
}

#endif
