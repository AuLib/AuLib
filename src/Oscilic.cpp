////////////////////////////////////////////
// Implementation of the Oscilic class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "Oscilic.h"

void Oscilic::process(){
  uint32_t phi;
  double frac,a,b,c,d;
  double tmp, fracsq, fracb;
  for(int i=0; i < m_vsize; i++){
    phi = (uint32_t) m_phs;
    frac = m_phs - phi;
    a = phi > 0 ? m_table[phi-1] :
      m_table[m_tsize-1];
    b = m_table[phi];
    c = m_table[phi+1];
    d = phi < m_tsize ?
      m_table[phi+2] :
      m_table[1];   
    tmp = d + 3.f*b;
    fracsq = frac*frac;
    fracb = frac*fracsq;
    m_output[i] = m_amp*
      (fracb*(- a - 3.f*c + tmp)/6.f +
       fracsq*((a+c)/2.f - b) +
       frac*(c + (-2.f*a - tmp)/6.f) + b);
    m_phs += m_incr;
    mod();
  }
}

