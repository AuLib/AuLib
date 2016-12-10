////////////////////////////////////////////////////////////////////
// SoundIn class: audio input
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SOUNDIN_H
#define _SOUNDIN_H

#include "AudioBase.h"

enum {SOUNDIN_RT=1,
      SOUNDIN_STDIN,
      SOUNDIN_SNDFILE};

/** Audio input class
 */
class SoundIn : AudioBase {

protected:
  const char *m_src;
  uint32_t m_mode;
  uint32_t m_cnt;
  uint32_t m_framecnt;
  void *m_buffer;
  uint32_t m_bsize;
  void *m_handle;

public:
  /** SoundIn constructor \n\n
      src - input source ("adc", "stdin", or file path) \n
      vsize - vector size \n
      bsize - buffer size \n
  */
  SoundIn(const char *src, uint32_t nchnls = def_nchnls,
	  uint32_t vsize = def_vsize, uint32_t bsize = def_bsize,
	  double sr = def_sr);
  
  /** SoundOut destructor
   */
  ~SoundIn();

  /** Reads audio into output vector
   */
  uint32_t read(); 
  
};

#endif
