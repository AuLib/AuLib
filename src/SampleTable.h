////////////////////////////////////////////
// SampleTable class: Sampled-sound tables
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#ifndef _SAMPLETABLE_H
#define _SAMPLETABLE_H

#include "FuncTable.h"

class SampleTable : public FuncTable {

 protected:
  uint32_t m_nchnls;

 public:
  /** SampleTable constructor
      name - filename
      chn - channel to load
  */
  SampleTable(const char *name,
	      uint32_t chn = 1);

  /** Get the number of channels
   */
  uint32_t nchnls(){
    return m_nchnls;
  }
 
};
#endif
