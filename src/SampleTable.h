////////////////////////////////////////////////////////////////////
// SampleTable class: Sampled-sound tables
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SAMPLETABLE_H
#define _SAMPLETABLE_H

#include "FuncTable.h"

namespace AuLib {

  /** Error codes
   */
  enum sampletable_error_codes {
    AULIB_FILE_ERROR
    = AULIB_ERROR + 1,
    AULIB_READ_ERROR
  };

  /** Sampled-sound table from a soundfile
   */
  class SampleTable : public FuncTable {

  protected:
    uint32_t m_nchnls;

  public:
    /** SampleTable constructor \n\n
	name - filename \n
	chn - channel to load (0 = all channels) \n
    */
    SampleTable(const char *name,
		uint32_t chn = 1);

    /** Get the number of channels
     */
    uint32_t nchnls(){
      return m_nchnls;
    }
 
  };

}
#endif
