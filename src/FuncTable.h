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
    uint32_t m_tsize;
    
    /** Normalise the table
     */ 
    void normalise_table(){
      uint32_t n;
      double max = 0.;
      for(n=0; n < vsamps(); n++)
	max = m_vector[n] > max ?
	  m_vector[n] : max;
      if(max)
	for(n=0; n < vsamps(); n++)
	  m_vector[n] /= max;
    }

  public:
    
    /** FuncTable constructor \n\n
	tsize - table size \n
        nchnls - number of channels \n
        sr - sampling rate \n
    */ 
    FuncTable(uint32_t tsize = def_tsize,
	      uint32_t nchnls = def_nchnls,
	      uint32_t sr = def_sr)
      : m_tsize(tsize),
	AudioBase(nchnls,tsize+2,sr) {
    }

    uint32_t tsize() const {
      return m_tsize;
    }

    const double* table() const {
      return vector();
    }
      
  };


  
  /*! \class FuncTable FuncTable.h AuLib/FuncTable.h
   */

}
#endif
