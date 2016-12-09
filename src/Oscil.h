////////////////////////////////////////////
// Oscil class: truncating oscillator
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _OSCIL_H
#define _OSCIL_H

#include "AudioBase.h"

class Oscil : public AudioBase {

 protected:
  
  double m_phs;  
  double m_freq;
  double m_amp;
  double m_incr;
  uint32_t m_tsize;
  double *m_table;
  bool m_sine;
  
  /** phase modulo
   */
  void mod(){
    while(m_phs >= m_tsize) m_phs -= m_tsize;
    while(m_phs < 0) m_phs += m_tsize;
  }

 public:
  /** Oscil constructor
      amp - amplitude
      freq - frequency in Hz
      phase - init phase (0-1)  
      table - function table
      tsize - table size
      sr - sampling rate
      vsize - vector size
  */
  Oscil(double amp, double freq,
	double phase = .0,
	double *table = NULL,
	uint32_t tsize = def_tsize,
	double sr = def_sr,
	uint32_t vsize = def_vsize);

  /** Oscil destructor
   */
  virtual ~Oscil();

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
