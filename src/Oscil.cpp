////////////////////////////////////////////
// Implementation of the Oscil class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "Oscil.h"

Oscil::Oscil(double amp, double freq,
	     double phase, double *table,
	     uint32_t tsize,double sr,
	     uint32_t vsize) :
  m_amp(amp), m_freq(freq),
  m_phs(phase), m_tsize(tsize),
  m_table(table), m_sine(false),
  AudioBase(1,sr,vsize)
{
  m_incr = m_freq*m_tsize/m_sr;
  m_output = new double[m_vsize];
  if(m_table == NULL) {
    m_table = new double[def_tsize+1];
    for(int i=0; i < def_tsize; i++)
      m_table[i] = sin(i*twopi/def_tsize);
    m_table[def_tsize] = m_table[0];
    m_sine = true;
  }
   m_phs *= m_tsize;
   mod();
}

Oscil::~Oscil(){
  if(m_sine)
    delete[] m_table;
}

void Oscil::process(){
  for(int i=0; i < m_vsize; i++){
    m_output[i] = m_amp*m_table[(uint32_t)m_phs];
    m_phs += m_incr;
    mod();
  }
}
