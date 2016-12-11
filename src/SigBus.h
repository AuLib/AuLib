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
    bool  m_ovw;
  
  public:
    /** SigBus constructor \n\n
	scal - amplitude scaling \n
	offs - amplitude offset \n
        overwrite - overwrite bus \n
	nchnls - number of channels \n
	vsize - vector size \n
    */  
    SigBus(double scal = 1., double offs = 0.,
	   bool overwrite = false,
	   int nchnls = def_nchnls,
	   uint32_t vsize = def_vsize) :
      m_scal(scal), m_offs(offs), m_ovw(overwrite),
      AudioBase(nchnls,vsize) { };

    /** Adds a signal vector to the bus.
	Requires an explicit call to clear()
        if overwrite is switched off
	once the bus data has been consumed
    */
    virtual void process(const double* sig);

    /** Applies a gain scaling and optional
	offset to a signal vector. If
	overwrite is true, the output vector is
	overwritten and a call to 
	clear() is not required.
    */
    virtual void process(const double* sig,
			 double scal, double offs = 0.,
			 bool overwrite = true){
      m_scal = scal;
      m_offs = offs;
      m_ovw = overwrite;
      process(sig);
    }


    /** Adds a signal vector to the bus from obj.
	Requires an explicit call to clear()
	once the bus data has been consumed
    */
    virtual void process(const AudioBase& obj) {
      if(obj.vsize() == m_vsize &&
	 obj.nchnls() == m_nchnls){
	process(obj.output());
      } else m_error = AULIB_ERROR;
    }

    /** Applies a gain scaling and optional
	offset to a signal vector from obj. If
	overwrite is true, the output vector is
	overwritten and a call to 
	clear() is not required.
    */
    virtual void process(const AudioBase& obj,
			 double scal, double offs = 0.,
			 bool overwrite = true) {
      m_scal = scal;
      m_offs = offs;
      m_ovw = overwrite;
      process(obj.output());
    }     

    /** Clears the output vector
     */
    void clear(){
      memset(m_output,0,
	     sizeof(double)*m_vsize*m_nchnls);
    }

  };

  /*! \class SigBus SigBus.h AuLib/SigBus.h
   */

}
#endif
