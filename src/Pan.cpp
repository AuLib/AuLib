#include "Pan.h"

void
AuLib::Pan::process(const double* sig){
  double lg,rg;
  lg = sin(m_pos*pi/2.);
  rg = cos(m_pos*pi/2.);
  for(int i = 0, j = 0; i < m_vsize; i++, j+=2){
    m_output[j] = lg*sig[i];
    m_output[j+1] = rg*sig[i];
  }
}
