/////////////////////////////////////////////////////////////////////
// Fir class: convolution engine
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _FIR_H_
#define _FIR_H_
#include "Delay.h"
#include "FuncTable.h"

namespace AuLib {

/** This class implements a direct convolution engine
    using an impulse response defined in a function table.
*/
class Fir : public Delay {

  virtual const double *dsp(const double *sig, double dt) { return dsp(sig); }
  virtual const double *dsp(const double *sig, const double *dt) {
    return dsp(sig);
  }
  virtual const double *dsp(const double *sig);

protected:
  const FuncTable &m_ir;

public:
  /** Fir constructor \n\n
      ir - impulse response
      vframes - vector size \n
      sr - sampling rate
  */
  Fir(const FuncTable &ir, uint32_t vframes = def_vframes, double sr = def_sr)
      : Delay(ir.tframes() * sr, 0., vframes, sr), m_ir(ir){};
};

/*! \class Fir Fir.h AuLib/Fir.h
 */
}
#endif
