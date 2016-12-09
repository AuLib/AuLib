////////////////////////////////////////////
// Implementation of the Phasor class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "Phasor.h"

Phasor::Phasor(double freq, double phase,
	       double sr,uint32_t vsize) :
  m_freq(freq),m_phs(phase),
  AudioBase(1,sr,vsize)
{
  m_incr = m_freq/m_sr;
  mod();
}

void Phasor::process(){
  for(int i=0; i < m_vsize; i++){
    m_output[i] = m_phs;
    m_phs += m_incr;
    mod();
  }
}
