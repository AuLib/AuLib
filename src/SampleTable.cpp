////////////////////////////////////////////
// Implementation of the SampleTable class
// (c) V Lazzarini, 2016-7
////////////////////////////////////////////
#include "SampleTable.h"
#include "sndfile.h"

SampleTable::SampleTable(const char *name,
			 uint32_t chn) :
  FuncTable(0) {
  SF_INFO info;
  SNDFILE *sf = sf_open(name,SFM_READ,&info);
  if(chn == 0) {
    m_tsize = info.frames*info.channels;
    m_nchnls = info.channels;
  }
  else {
    m_tsize = info.frames;
    m_nchnls = 1;
  }
  m_table = new double[m_tsize];
  sf_count_t read = 0;
  while(read != m_tsize) 
    read +=
      sf_read_double(sf,m_table,m_tsize);
  sf_close(sf);
}
