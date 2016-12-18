////////////////////////////////////////////////////////////////////
// Oscilic class: cubic interpolating oscillator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _OSCILIC_H
#define _OSCILIC_H

#include "Oscil.h"

namespace AuLib {

  /** Cubic interpolation oscillator
   */
  class Oscilic : public Oscil {

  protected:
    /** cubic oscillator process
     */
    virtual void oscillator();
    
  public:
    /** Sinusoidal Oscilic constructor \n\n
	amp - amplitude   \n
	freq - frequency in Hz \n
	phase - init phase (0-1) \n 
	vframes - vector size \n
	sr - sampling rate \n\n
	Uses internal sine wave table
    */
    Oscilic(double amp = 0., double freq = 0.,
	    double phase = 0., uint32_t vframes = def_vframes,
	    double sr = def_sr) :
      Oscil(amp,freq,phase,vframes,sr) { };
      
    /** Oscilic constructor \n\n
	amp - amplitude \n
	freq - frequency in Hz \n
	ftable - function table \n
	phase - init phase (0-1) \n 
	vframes - vector size \n
	sr - sampling rate \n
    */
    Oscilic(double amp, double freq,
	    const FuncTable& ftable,
	    double phase = 0.,
	    uint32_t vframes = def_vframes,
	    double sr = def_sr) :
      Oscil(amp,freq,ftable,phase,vframes,sr) { };

  };
  
  /*! \class Oscilic Oscilic.h AuLib/Oscil.h
   */
}
#endif
