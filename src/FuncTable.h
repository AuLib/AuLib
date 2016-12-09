////////////////////////////////////////////
// FuncTable class: function tables
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _FUNCTABLE_H
#define _FUNCTABLE_H

#include "AuLib.h"

/** Function table base class
 */
class FuncTable {

 protected:
  double *m_table;
  uint32_t m_tsize;
 
  /** Normalise the table
   */ 
  void normalise_table(){
    uint32_t n;
    double max = 0.;
    for(n=0; n < m_tsize+1; n++)
      max = m_table[n] > max ?
	m_table[n] : max;
    if(max)
      for(n=0; n < m_tsize+1; n++)
	m_table[n] /= max;
  }

 public:
  /** FuncTable constructor \n\n
      tsize - table size \n
   */ 
  FuncTable(uint32_t tsize = def_tsize)
  : m_tsize(tsize) {
    if(m_tsize > 0) {
     m_table = new double[m_tsize+1];
     memset(m_table,0,
	 sizeof(double)*(m_tsize+1));
    }
  }
  ~FuncTable(){
    delete[] m_table;
  }

  /** Get the function table
   */ 
  double *table(){
    return m_table;
  }

  /** Get a single point at pos
      from the function table
  */ 
  double table(uint32_t pos){
    return m_table[pos];
  }

  uint32_t size(){
    return m_tsize;
  }
};

#endif
