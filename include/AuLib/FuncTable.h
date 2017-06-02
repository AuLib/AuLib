////////////////////////////////////////////////////////////////////
// FuncTable class: function tables
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _FUNCTABLE_H
#define _FUNCTABLE_H

#include "AudioBase.h"

namespace AuLib {
/** Function table base class
 */
class FuncTable : public AudioBase {

protected:
  uint64_t m_tframes;

  /** Normalise the table
   */
  void normalise_table(double s = 1.0) {
    double max = 0.;
    for (uint64_t n = 0; n < tframes() + 2; n++)
      max = m_vector[n] > max ? m_vector[n] : max;
    if (max)
      for (uint64_t n = 0; n < vsamps(); n++)
        m_vector[n] *= s / max;
  }

public:
  /** define normalisation (rescaling) */
  static constexpr bool normalised = true;

  /** FuncTable constructor \n\n
      tframes - table size \n
      nchnls - number of channels \n
      sr - sampling rate \n
  */
  FuncTable(uint32_t tframes = def_tframes, uint32_t nchnls = def_nchnls,
            uint32_t sr = def_sr)
      : AudioBase(nchnls, 0, sr), m_tframes(tframes) {
    resize_exact(tframes + 2);
  };

  /** FuncTable constructor from vector \n\n
      src - source vector \n
      norm - normalise table
      tframes - table size \n
      nchnls - number of channels \n
      sr - sampling rate \n
  */
  FuncTable(const double *src, bool norm = false,
            uint32_t tframes = def_tframes, uint32_t nchnls = def_nchnls,
            uint32_t sr = def_sr)
      : AudioBase(nchnls, tframes + 2, sr), m_tframes(tframes) {
    set(src);
    if (norm)
      normalise_table();
  }

  /** return the nominal table size, ie. the total vectorsize minus the
      two guard points.
   */
  uint64_t tframes() const { return m_tframes; }

  /** rescale the table for an absolute max
   */
  void rescale(double max) { normalise_table(max); }
};

/*! \class FuncTable FuncTable.h AuLib/FuncTable.h
 */
}
#endif
