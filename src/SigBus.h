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
    virtual const double* process(const double* sig){
      for(int i=0; i < m_vsize*m_nchnls; i++)
	m_vector[i] = m_ovw ? m_scal*sig[i] + m_offs :
	  m_scal*sig[i] + m_offs + m_vector[i];
      return m_vector;
    }

    /** Applies a gain scaling and optional
	offset to a signal vector. If
	overwrite is true, the vector vector is
	overwritten and a call to 
	clear() is not required.
    */
    virtual const SigBus& process(const double* sig,
				  double scal, double offs = 0.,
				  bool overwrite = true){
      m_scal = scal;
      m_offs = offs;
      m_ovw = overwrite;
      process(sig);
      return *this;
    }


    /** Adds a signal vector to the bus from obj.
	Requires an explicit call to clear()
	once the bus data has been consumed
    */
    virtual const SigBus& process(const AudioBase& obj) {
      if(obj.vsize() == m_vsize &&
	 obj.nchnls() == m_nchnls){
	process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** Applies a gain scaling and optional
	offset to a signal vector from obj. If
	overwrite is true, the vector vector is
	overwritten and a call to 
	clear() is not required.
    */
    virtual const SigBus& process(const AudioBase& obj,
				  double scal, double offs = 0.,
				  bool overwrite = true) {
      m_scal = scal;
      m_offs = offs;
      m_ovw = overwrite;
      process(obj.vector());
      return *this;
    }     

    /** Clears the vector vector
     */
    void clear(){
      memset(m_vector,0,
	     sizeof(double)*m_vsize*m_nchnls);
    }

  };

  /*! \class SigBus SigBus.h AuLib/SigBus.h
   */

}
#endif
