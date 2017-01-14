////////////////////////////////////////////////////////////////////////////////
// Classic window functions based on FourierTable
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// -*- mode: c++;-*-
////////////////////////////////////////////////////////////////////////////////

#ifndef _AULIB_WINDOWS_H
#define _AULIB_WINDOWS_H

#include "FourierTable.h"

namespace AuLib {

/** von Hann window
 */
struct Hann : FourierTable {

  /** Hann constructor \n\n
      tframes - table size \n
  */
  Hann(uint64_t tframes = def_fftsize)
      : FourierTable(1, std::vector<double>(1, {1.}).data(), 0.5, tframes) {
    *this *= 0.5;
    *this += 0.5;
  }
};

/** Hamming window
 */
struct Hamming : FourierTable {

  /** Hamming constructor \n\n
      tframes - table size \n
  */
  Hamming(uint64_t tframes = def_fftsize)
      : FourierTable(1, std::vector<double>(1, {1.}).data(), 0.5, tframes) {
    *this *= 0.48;
    *this += 0.52;
  }
};
}
#endif
