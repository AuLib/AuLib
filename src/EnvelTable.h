////////////////////////////////////////////////////////////////////
// EnvelTable class: envelope tables
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _ENVELTABLE_H
#define _ENVELTABLE_H

#include "FuncTable.h"
#include "Envel.h"

namespace AuLib {
  
  /** Multi-segment Envelope Tables 
   */
  class EnvelTable : public FuncTable {
   
  public:
    
    /** EnvelTable constructor \n\n
        segs - envelope segments
        norm - normalise table
    */ 
    EnvelTable(const Segments& segs, bool norm = true);
  
  };
  
  /*! \class FuncTable EnvelTable.h AuLib/EnvelTable.h
   */
}
#endif
