////////////////////////////////////////////////////////////////////
// SoundOut class: audio output
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SOUNDOUT_H
#define _SOUNDOUT_H

#include "AudioBase.h"

enum {SOUNDOUT_RT=1,
      SOUNDOUT_STDOUT,
      SOUNDOUT_SNDFILE
};

enum{AULIB_FOPEN_ERROR
     = AULIB_ERROR + 1,
     AULIB_RTINIT_ERROR,
     AULIB_RTOPEN_ERROR,
     AULIB_RTSTREAM_ERROR   
};

/** Generic audio output class
 */
class SoundOut : public AudioBase {

protected:
  const char *m_dest;
  uint32_t m_mode;
  uint32_t m_cnt;
  uint32_t m_framecnt;
  void *m_buffer;
  uint32_t m_bsize;
  void *m_handle;

public:
  /** SoundOut constructor \n\n
      dest - output destination 
      ("dac", "stdout", or file path) \n
      nchnls - number of channels \n
      sr - sampling rate \n
      vsize - vector size \n
      bsize - buffer size \n
  */
  SoundOut(const char *dest,
	   uint32_t nchnls = def_nchnls,
	   uint32_t vsize = def_vsize,
	   uint32_t bsize = def_bsize,
	   double sr = def_sr);
  
  /** SoundOut destructor
   */
  ~SoundOut();

  /** Writes sig to the output
      destination.
  */
  uint32_t write(const double *sig); 

};

#endif
