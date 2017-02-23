////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  MidiIn.h: MIDI listener class
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIDIIN_H__
#define __MIDIIN_H__

#include <Note.h>
#include <array>
#include <string>
#include <vector>

namespace AuLib {

struct MidiData {
  uint32_t msg, chn, byte1, byte2;
  uint64_t stamp;
};

class MidiIn {
  void *m_mstream;
  std::vector<std::string> m_devs;
  std::array<MidiData, 32> m_mdata;
  void list_devices();
  uint32_t read();

public:
  MidiIn();
  ~MidiIn();
  uint32_t open(int dev);
  void close();

  template <typename T> void listen(Instrument<T> &inst) {
    uint32_t cnt = read();
    for (uint32_t i = 0; i < cnt; i++)
      inst.dispatch(m_mdata[i].msg, m_mdata[i].chn, m_mdata[i].byte1,
                    m_mdata[i].byte2, m_mdata[i].stamp);
  }

  const std::vector<std::string> &device_list() { return m_devs; }
};
}

#endif
