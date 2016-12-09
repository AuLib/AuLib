////////////////////////////////////////////
// Implementation of the TableReadi class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "TableReadi.h"

void
TableReadi::process(double *phs){
  uint32_t posi;
  double   pos, frac, a, b;
  for(int i=0; i < m_vsize; i++){   
    pos = mod((m_norm ?
	       phs[i]*m_tsize : phs[i])
	      + m_phs);
    posi = (uint32_t) pos;
    frac = pos - posi;
    a = m_table[posi];
    b = m_table[posi+1];
    m_output[i] = a + frac*(b - a);
  } 
}
