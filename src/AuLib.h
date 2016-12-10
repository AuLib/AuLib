////////////////////////////////////////////////////////////////////
// AuLib.h: Library-wide definitions
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////

#ifndef _AU_LIB_H
#define _AU_LIB_H

#include <cmath>
#include <cstdint>
#include <cstring>

/** default signal vectorsize.
*/
const uint32_t def_vsize = 64;

/** default IO buffersize.
*/
const uint32_t def_bsize = 1024;

/** default sampling rate
*/
const double def_sr = 44100.;

/** default control rate
*/
const double def_kr = def_sr/def_vsize;

/** default audio channels.
*/
const uint32_t  def_nchnls = 1;

/** default function table length
*/
const uint32_t   def_tsize = 16384;

/** butterworth modes 
*/
enum { BP,BR,LP,HP };

/** the pi definition. 
*/ 
const double pi = 4*atan(1.); 

/** the two pi definition. 
*/ 
const double twopi = 8*atan(1.);

/** General Error Codes
 */
enum {
      AULIB_NOERROR = 0,
      AULIB_MEM_ERROR,
      AULIB_ERROR
      };

#endif  // ifndef _AU_LIB_H
