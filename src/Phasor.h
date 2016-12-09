////////////////////////////////////////////
// Phasor class: phase signal generator
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _PHASOR_H
#define _PHASOR_H

#include "AudioBase.h"

class Phasor : public AudioBase {

 protected:
  double m_phs;  
  double m_freq;
  double m_incr;
  
  /** phase modulo
   */
  void mod(){
    while(m_phs >= 1.0) m_phs -= 1.;
    while(m_phs < 0.0) m_phs += 1.;
  }

 public:
  /** Phasor constructor
      freq - frequency in Hz
      phase - init phase (0-1)  
      sr - sampling rate
      vsize - vector size
  */
  Phasor(double freq,
	double phase = .0,
	double sr = def_sr,
	uint32_t vsize = def_vsize);

  /** Process one vector of audio
   */
  virtual void process();
  
  /** Process one vector of audio with
      frequency freq
  */
  virtual void process(double freq){
    m_freq = freq;
    m_incr = m_freq/m_sr;
    process();
  }

};
#endif
