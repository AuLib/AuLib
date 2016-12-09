////////////////////////////////////////////
// Oscili class: interpolating oscillator
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _SAMPLEPLAYER_H
#define _SAMPLEPLAYER_H

#include "Oscili.h"

/** Sample playback oscillator 
    with linear interpolation
 */
class SamplePlayer : public Oscili {

 public:
  /** SamplePlayer constructor \n\n
      amp - amplitude \n
      pitch - playback pitch \n
      phase - init phase (0-1) \n 
      table - function table \n
      tsize - table size \n
      sr - sampling rate \n
      vsize - vector size \n
  */
  SamplePlayer(double amp, double pitch,
	double phase = .0,
	double *table = NULL,
	uint32_t tsize = def_tsize,
	double sr = def_sr,
	 uint32_t vsize = def_vsize) :
  Oscili(amp,pitch*sr/tsize,phase,table,
	tsize,sr,vsize) { };


  /** Process one vector of audio
  */
  virtual void process() {
    Oscili::process();
  }

 /** Process one vector of audio
      with amplitude amp
  */
  virtual void process(double amp) {
    Oscili::process(amp);
  }


 /** Process one vector of audio
      with amplitude amp and
      pitch transposition
  */
  virtual void process(double amp, double pitch){
    m_amp = amp;
    m_freq = pitch*m_sr/m_tsize;
    m_incr = pitch;
    Oscili::process();
  }
  
};

#endif
