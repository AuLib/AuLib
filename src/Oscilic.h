////////////////////////////////////////////
// Oscili class: cubic interpolating
//  oscillator
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _OSCILI_H
#define _OSCILI_H

#include "Oscil.h"

/** Cubic interpolation oscillator
 */
class Oscilic : public Oscil {

 public:
  /** Oscilic constructor \n\n
      amp - amplitude \n
      freq - frequency in Hz \n
      phase - init phase (0-1) \n  
      table - function table \n
      tsize - table size \n
      sr - sampling rate \n
      vsize - vector size \n
  */
  Oscilic(double amp, double freq,
	double phase = .0,
	double *table = NULL,
	uint32_t tsize = def_tsize,
	double sr = def_sr,
	 uint32_t vsize = def_vsize) :
  Oscil(amp,freq,phase,table,
	tsize,sr,vsize) { };


  /** Process one vector of audio
   */
  virtual void process();

  /** Process one vector of audio
      with amplitude amp
  */
  virtual void process(double amp) {
    m_amp = amp;
    process();
  }

  /** Process one vector of audio
      with amplitude amp and
      frequency freq
  */
  virtual void process(double amp, double freq){
    m_amp = amp;
    m_freq = freq;
    m_incr = m_freq*m_tsize/m_sr;
    process();
  }
  
};

#endif
