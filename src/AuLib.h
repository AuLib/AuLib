////////////////////////////////////////////////////////////////////////////////
// AuLib.h: Library-wide definitions
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _AU_LIB_H
#define _AU_LIB_H

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>

#ifndef AULIB_MAJOR_V
#define AULIB_MAJOR_V 0
#endif

#ifndef AULIB_MINOR_V
#define AULIB_MINOR_V 0
#endif

#ifndef BETA
#define BETA 1
#endif

#define NONCOPYABLE(A)                                                         \
  \
public:                                                                        \
  A(const A &) = delete;                                                       \
  A &operator=(A) = delete

namespace AuLib {

namespace Info {
/** Aulib major version */
const uint32_t major_version = AULIB_MAJOR_V;

/** Aulib minor version */
const uint32_t minor_version = AULIB_MINOR_V;

/** returns the version string */
static inline const std::string version() {
  std::ostringstream stm;
  stm << "Aulib version " << major_version << '.' << minor_version
      << (BETA ? " beta\n" : "\n");
  return stm.str();
}

/** returns the copyright string */
static inline const std::string copyright() {
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
const uint32_t def_bframes = 1024;

/** default sampling rate
 */
const double def_sr = 44100.;

/** default control rate
 */
const double def_kr = def_sr / def_vframes;

/** default audio channels.
 */
const uint32_t def_nchnls = 1;

/** default function table length
 */
const uint32_t def_tframes = 8192;

/** default fftsize
 */
const uint32_t def_fftsize = 1024;

/** default decimation
 */
const uint32_t def_decim = 4;

/** the pi definition.
 */
const double pi = 4. * atan(1.);

/** the two pi definition.
 */
const double twopi = 8. * atan(1.);

/** the min. pos. double
 */
const double db_min = std::numeric_limits<double>::min();

/** the max uint64_t
 */
const uint64_t ui64_max = std::numeric_limits<uint64_t>::max();

/** -120dBfs
 */
const double m120dBfs = 0.000001;

namespace midi {
/** MIDI note on
 */
const uint32_t note_on = 0x90;

/** MIDI note off
*/
const uint32_t note_off = 0x80;

/** MIDI control change
*/
const uint32_t ctrl_msg = 0xB0;

/** MIDI aftertouch
*/
const uint32_t aftouch = 0xD0;

/** MIDI poly aftertouch
*/
const uint32_t poly_aftouch = 0xA0;

/** MIDI program change
*/
const uint32_t prg_msg = 0xC0;

/** MIDI pitchbend
*/
const uint32_t pitchbend = 0xE0;
}

/** General error xodes
 */
enum error_codes { AULIB_NOERROR = 0, AULIB_ERROR };

/** Standard Error messages
 */
const std::string aulib_error[] = {"Aulib: no error",
                                   "Aulib: general object error"};
};
#endif // ifndef _AU_LIB_H
