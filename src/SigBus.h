/////////////////////////////////////////////////////////////////////
// Signal Bus 
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SIGBUS_H_
#define _SIGBUS_H_
#include "AudioBase.h"

namespace AuLib {

  /** A signal bus
      with optional scaling, offset and overwriting
  */
  class SigBus : public AudioBase {

  protected:
    double m_scal;
    double m_offs;
  
  public:
    /** SigBus constructor \n\n
	scal - amplitude scaling \n
	offs - amplitude offset \n
	nchnls - number of channels \n
	vsize - vector size \n
    */  
    SigBus(double scal = 1., double offs = 0.,
	   int nchnls = def_nchnls,
	   uint32_t vsize = def_vsize) :
      m_scal(scal), m_offs(offs),
      AudioBase(nchnls,vsize) { };

    /** Adds a signal vector to the bus
	requires an explicit call to clear()
	once the bus data has been consumed
    */
    void process(const double *sig);

    /** Applies a gain scaling and optional
	offset to a signal vector. If
	overwrite is true, the output vector is
	overwritten and a call to 
	clear() is not required.
    */
    void process(const double *sig,
		 double scal, double offs = 0.,
		 bool overwrite = true);

    /** Clears the output vector
     */
    void clear(){
      memset(m_output,0,
	     sizeof(double)*m_vsize*m_nchnls);
    }

  };

}
#endif
