////////////////////////////////////////////
// SoundIn class: audio input
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _SOUNDIN_H
#define _SOUNDIN_H

#include "AudioBase.h"

enum {SOUNDIN_RT=1,
     SOUNDIN_STDIN,
     SOUNDIN_SNDFILE};

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
  /** SoundIn constructor
    src - input source
    vsize - vector size
    bsize - buffer size
  */
  SoundIn(const char *src,
	   uint32_t vsize = def_vsize,
	   uint32_t bsize = def_bsize);
  
  /** SoundOut destructor
   */
  ~SoundIn();

  /** Writes sig to the output
    destination.
  */
  uint32_t read(double *sig); 
  
};

#endif
