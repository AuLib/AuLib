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

AuLib::SampleTable::SampleTable(const char *name, uint32_t chn) : FuncTable() {
  SF_INFO info;
  SNDFILE *sf = sf_open(name, SFM_READ, &info);
  if (sf != NULL) {
    if (chn == 0) {
      m_tframes = info.frames;
      m_vframes = info.frames + 2;
      m_nchnls = info.channels;
      m_sr = (double)info.samplerate;
    } else {
      m_tframes = info.frames;
      m_vframes = m_tframes + 2;
      m_nchnls = 1;
      m_sr = (double)info.samplerate;
    }
    m_vector.resize(m_vframes * m_nchnls);
    sf_count_t read = 0, frms;
    std::vector<double> tmp(def_vframes * info.channels);
    while (read != (int32_t)m_tframes) {
      frms = sf_readf_double(sf, tmp.data(), def_vframes);
      if (frms == 0 || sf_error(sf) != SF_ERR_NO_ERROR) {
        m_error = AULIB_FILE_ERROR;
        break;
      }
      if (chn && (uint32_t)info.channels != m_nchnls)
        for (uint32_t i = 0, j = chn <= (uint32_t)info.channels
                                     ? chn - 1
                                     : info.channels - 1;
             i < frms; i++, j += info.channels) {
          m_vector[i + read] = tmp[j];
        }
      else
        std::copy(tmp.begin(), tmp.end(), m_vector.data() + read * m_nchnls);
      read += frms;
    }
    for (uint32_t i = 0; i < m_nchnls; i++) {
      m_vector[m_tframes * m_nchnls + i] = m_vector[i];
      m_vector[(m_tframes + 1) * m_nchnls + i] = m_vector[m_nchnls + i];
    }
    sf_close(sf);
  } else
    m_error = AULIB_FILE_ERROR;
}
