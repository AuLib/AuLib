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
 *  AuLib is a simple lightweight C++ (ISO C++11) class library 
 *  for audio DSP.
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
#include <iostream>
#include <sstream>
#include <limits>

#define NONCOPYABLE(A)				\
  private:					\
  A(const A&);					\
  A& operator=(A)				\

namespace AuLib {

  namespace Info {
    /** Aulib major version */
    const uint32_t major_version = 0;

    /** Aulib minor version */
    const uint32_t minor_version = 0;

    /** returns the version string */
    static const std::string version() {
      std::ostringstream stm;
      stm << "Aulib version " << major_version << '.' << minor_version
	  << '\n';
      return stm.str();
    }

    /** returns the copyright string */
    static const std::string copyright() {
      std::ostringstream stm;
      stm << "Aulib version " << major_version << '.' << minor_version;
      stm << "\n(c) 2016-7 V.Lazzarini, licensed by the LGPL\n";
      return stm.str();
    }    
    
  }
  
  /** default signal vectorsize.
   */
  const uint32_t def_vframes = 64;

  /** default IO buffersize.
   */
  const uint32_t def_bsize = 1024;

  /** default sampling rate
   */
  const double def_sr = 44100.;

  /** default control rate
   */
  const double def_kr = def_sr/def_vframes;

  /** default audio channels.
   */
  const uint32_t  def_nchnls = 1;

  /** default function table length
   */
  const uint32_t def_tsize = 8192;

  /** the pi definition. 
   */ 
  const double pi = 4*atan(1.); 

  /** the two pi definition. 
   */ 
  const double twopi = 8*atan(1.);

  /** the min. pos. double
   */ 
  const double db_min = std::numeric_limits<double>::min();

  /** -120dBfs
   */ 
  const double m120dBfs = 0.000001;
  

  /** General error xodes
   */
  enum error_codes {
    AULIB_NOERROR = 0,
    AULIB_ERROR
  };

  /** Standard Error messages
   */
  static const char* aulib_error[] = {
    "Aulib: no error",
    "Aulib: general object error"
  };
  
};
#endif  // ifndef _AU_LIB_H
