////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// PConv.h: partitioned convolution engine
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#include "AudioBase.h"
#include "FuncTable.h"
#include "fft.h"

namespace AuLib {

class PConv : public AudioBase {

  static constexpr uint32_t OLA = 1;
  static constexpr uint32_t OLS = 2;
  uint32_t m_count;
  uint32_t m_p;
  uint32_t m_psize;
  uint32_t m_nparts;
  uint32_t m_algo;
  std::vector<double> m_in;
  std::vector<double> m_out;
  std::vector<double> m_saved;
  std::vector<std::vector<std::complex<double>>> m_part;
  std::vector<std::vector<std::complex<double>>> m_del;
  std::vector<std::complex<double>> m_mix;

  const double *ola(const double *sig);
  const double *ols(const double *sig);

  void convolution();

  const double *dsp(const double *sig) {
    switch (m_algo) {
    case OLA:
      return ola(sig);
    case OLS:
      return ols(sig);
    default:
      return ols(sig);
    }
  }

public:
  /** PConv constructor, takes a function table containing an
     impulse response, the partition size, a channel to read from the
     table, a begin and end position in frames, and the usual standard
     parameters
  */
  PConv(const FuncTable &ir, uint32_t psize = 256, uint32_t chn = 0,
        uint32_t begin = 0, uint32_t end = 0, uint32_t vframes = def_vframes,
        double sr = def_sr);

  /** PConv constructor, selects a convolution algorithm (PConv::OLA,
     PConv::OLS),
     takes a function table containing an
     impulse response, the partition size, a channel to read from the
     table, a begin and end position in frames, and the usual standard
     parameters
  */
  PConv(uint32_t algo, const FuncTable &ir, uint32_t psize = 256,
        uint32_t chn = 0, uint32_t begin = 0, uint32_t end = 0,
        uint32_t vframes = def_vframes, double sr = def_sr);

  /** Update the impulse response, replacing it with new data from the
      ir function table.
   */
  void update(const FuncTable &ir, uint32_t chn = 0, uint32_t begin = 0,
              uint32_t end = 0);

  /** Apply partitioned convolution to an input signal sig
   */
  const double *process(const double *sig) { return dsp(sig); }

  /** Apply partitioned convolution to a signal in obj
   */
  const PConv &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == 1 && obj.sr() == m_sr)
      process(obj.vector());
    return *this;
  }

  /** function operator convenience method, same as process()
   */
  const PConv &operator()(const AudioBase &obj) { return process(obj); }
};

/*! \class PConv PConv.h AuLib/PConv.h
*/
}
