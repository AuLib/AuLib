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

/*! \mainpage
 *
 *  AuLib is a simple lightweight C++ class library for audio DSP.
 *
 * \section build_the_package Building
 *
 * The library and example programs are built with CMake. The only
 * dependencies are libsndfile and portaudio and they are only required
 * to build the example programs that use the audio IO classes.
 * The code can be used within any audio processing context and it can
 * be incorporated in plugins and other programs with their own IO.
 *
 * \code
 * 
 * $ mkdir build
 * $ cd build
 * $ cmake .. 
 * $ make
 * $ make install
 * 
 * \endcode
 *
 * \section gpl_license License
 *
 * Copyright (C) 2016-7 V Lazzarini
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <cmath>
#include <cstdint>
#include <cstring>

namespace AuLib {
  
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

  /** filter modes 
   */
  enum filter_types { BP = 0,BR,LP,HP };

  /** the pi definition. 
   */ 
  const double pi = 4*atan(1.); 

  /** the two pi definition. 
   */ 
  const double twopi = 8*atan(1.);

  /** General error xodes
   */
  enum error_codes {
    AULIB_NOERROR = 0,
    AULIB_MEM_ERROR,
    AULIB_ERROR
  };

  /** Standard Error messages
   */
  static const char* aulib_error[] = {
    "Aulib: no error",
    "Aulib: memory allocation error",
    "Aulib: general object error"
  };
  
};
#endif  // ifndef _AU_LIB_H
