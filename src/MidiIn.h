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

/** MIDI data structure
 */
struct MidiData {
  uint32_t msg, chn, byte1, byte2;
  uint64_t stamp;
};

/** This class implements a MIDI input listener
*/  
class MidiIn {
  void *m_mstream;
  std::vector<std::string> m_devs;
  std::array<MidiData, 1024> m_mdata;
  void list_devices();
  uint32_t read();

public:
  /** Constructs an empty object */
  MidiIn();

  /** Destructor */
  ~MidiIn();

  /** Open MIDI input device dev
   */
  uint32_t open(int dev);

  /** Close MIDI connection
   */
  void close();

  /** Implements a listener for an obj inst
      dispatching any received data. Classes implementing
      the dispatch & process methods are valid types here.
   */
  template <typename T> T &listen(T &inst) {
    uint32_t cnt = read();
    for (uint32_t i = 0; i < cnt; i++)
      inst.dispatch(m_mdata[i].msg, m_mdata[i].chn, m_mdata[i].byte1,
                    m_mdata[i].byte2, m_mdata[i].stamp);
    inst.process();
    return inst;
  }

  /** Returns a list of devices names/numbers as strings
   */
  const std::vector<std::string> &device_list() { return m_devs; }
};
}

#endif
