////////////////////////////////////////////////////////////////////
// Implementation of the SampleTable class
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#include "SampleTable.h"
#include "sndfile.h"

SampleTable::SampleTable(const char *name,
			 uint32_t chn) :
  FuncTable(0) {
  SF_INFO info;
  SNDFILE *sf = sf_open(name,SFM_READ,&info);
  if(sf != NULL) {
    if(chn == 0) {
      m_tsize = info.frames*info.channels;
      m_nchnls = info.channels;
    }
    else {
      m_tsize = info.frames;
      m_nchnls = 1;
    }
    m_table = new double[m_tsize];
    if(m_table == NULL) {
      m_tsize = 0;
      m_error = AULIB_MEM_ERROR;
      sf_close(sf);
      return;
    }
    sf_count_t read = 0, items;
    while(read != m_tsize) { 
      items = sf_read_double(sf,m_table,m_tsize);
      if(items == 0 ||
	 sf_error(sf) != SF_ERR_NO_ERROR){
        m_error = AULIB_FILE_ERROR;
	break;
      }
      read += items;
    }
    sf_close(sf);
  } else
    m_error = AULIB_FILE_ERROR;
}
