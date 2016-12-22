////////////////////////////////////////////////////////////////////
// FourierTable class: Fourier series tables
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _FOURIERTABLE_H
#define _FOURIERTABLE_H

#include "FuncTable.h"

namespace AuLib {

  /** Table type constants
   */
  enum wave_types {
    SAW=1,
    SQUARE,
    TRIANGLE};

  /** Function tables based on
      Fourier Series
  */
  class FourierTable : public FuncTable {

  protected:
    /** Create the table
     */ 
    void create(uint32_t harms,
		double *amps,
		double phase);

  public:
    /** FourierTable constructor \n\n
	harms - number of harmonics \n
	amps - array of harmonic amplitudes \n
	phase - phase offset \n
	tframes - table size \n
    */
    FourierTable(uint32_t harms=0,
		 double *amps=NULL,
		 double phase=0.,
		 uint64_t tframes = def_tframes);
  
    /** FourierTable constructor \n\n
	harms - number of harmonics \n
	type - wave type \n
	tframes - table size \n
    */
    FourierTable(uint32_t harms,
		 uint32_t type,
		 uint64_t tframes = def_tframes);

  };

  /*! \class FourierTable FourierTable.h AuLib/FourierTable.h
   */

}
#endif
