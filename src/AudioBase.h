////////////////////////////////////////////
// AudioBase class: dsp base class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _AUDIOBASE_H
#define _AUDIOBASE_H

#include "AuLib.h"

class AudioBase {

 protected:
  uint32_t m_nchnls;
  double m_sr;
  uint32_t m_vsize;
  double *m_output;

 public:
  /** AudioBase constructor
      nchnls - number of channels
      sr - sampling rate
      vsize - vector size (frames)
  */
  AudioBase(uint32_t nchnls = def_nchnls,
	   double sr = def_sr,
	   double vsize = def_vsize) :
    m_nchnls(nchnls), m_sr(sr),
    m_vsize(vsize) {
    m_output = new double[m_vsize*m_nchnls];
  }
  
  /** AudioBase destructor
   */
  virtual ~AudioBase(){
    delete[] m_output;
  }

  /** Get the audio output vector
   */ 
  double *output(){
    return m_output;
  }
  
  /** Get a single sample at ndx 
      off the output audio vector
  */  
  double output(uint32_t ndx){
    return m_output[ndx];
  }
  
};
#endif
