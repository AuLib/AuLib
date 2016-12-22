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

  protected:
    const double *m_ir;
    uint64_t m_irsize;

  public:
    /** Fir constructor \n\n
        ir - impulse response
	vframes - vector size \n
        sr - sampling rate
    */  
  Fir(const FuncTable& ir, uint32_t vframes = def_vframes,
      double sr = def_sr) :
    Delay(ir.tframes()*sr,0.,vframes,sr),
    m_ir(ir.table()), m_irsize(ir.tframes()) { };

    /** apply convolution to a signal sig 
     */
    virtual const double* process(const double* sig);

    /** apply convolution to a signal in obj
     */
    virtual const Fir& process(const AudioBase& obj) {
      if(obj.vframes() == m_vframes &&
	 obj.nchnls() == m_nchnls) {
	process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }

  };

  /*! \class Fir Fir.h AuLib/Fir.h
   */
}
#endif
