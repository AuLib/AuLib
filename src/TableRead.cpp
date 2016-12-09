////////////////////////////////////////////
// Implementation of the TableRead class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "TableRead.h"

void
TableRead::process(double *phs){
  uint32_t pos;
  for(int i=0; i < m_vsize; i++){   
    pos = (uint32_t) mod((m_norm ?
	       phs[i]*m_tsize : phs[i])
	      + m_phs);
    m_output[i] = m_table[pos];
  } 
}
