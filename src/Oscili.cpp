////////////////////////////////////////////
// Implementation of the Oscili class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "Oscili.h"

void Oscili::process(){
  uint32_t phi;
  double frac,a,b;
  for(int i=0; i < m_vsize; i++){
    phi = (uint32_t) m_phs;
    frac = m_phs - phi;
    a = m_table[phi];
    b = m_table[phi+1];
    m_output[i] = 
      m_amp*(a + frac*(b - a));
    m_phs += m_incr;
    mod();
  }
}
