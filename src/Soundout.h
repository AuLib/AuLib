////////////////////////////////////////////
// SoundOut class: audio output
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _SOUNDOUT_H
#define _SOUNDOUT_H

#include "AuLib.h"

enum {SOUNDOUT_RT=1,
     SOUNDOUT_STDOUT,
     SOUNDOUT_SNDFILE};

struct SoundOut {

  double m_sr;
  uint32_t m_nchnls;
  double m_vsize;
  const char *m_dest;
  uint32_t m_mode;
  uint32_t m_cnt;
  uint32_t m_framecnt;
  void *m_buffer;
  uint32_t m_bsize;
  void *m_handle;
  
  /** SoundOut constructor
    dest - output destination
    nchnls - number of channels
    sr - sampling rate
    vsize - vector size
  */
  SoundOut(const char *dest,
	   uint32_t nchnls = def_nchnls,
	   double sr = def_sr,
	   uint32_t vsize = def_vsize,
	   uint32_t bsize = def_bsize);
  
  /** SoundOut destructor
   */
  ~SoundOut();

  /** Writes sig to the output
    destination.
  */
  uint32_t write(double *sig); 
  
};

#endif
