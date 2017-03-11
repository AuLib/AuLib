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

/** This class implements a MIDI input listener
*/
class MidiIn final : public AudioBase {

  struct MidiData {
    uint32_t msg, chn, byte1, byte2;
    uint64_t stamp;
  };

  void *m_mstream;
  std::vector<std::string> m_devs;
  std::array<MidiData, 1024> m_mdata;
  void list_devices();
  uint32_t read();

  void dispatch(uint32_t msg, int32_t chn, uint32_t byte1, uint32_t byte2,
                uint64_t stamp){};

  template <typename T, typename... Targs>
  void dispatch(uint32_t msg, int32_t chn, uint32_t byte1, uint32_t byte2,
                uint64_t stamp, T &inst, Targs &... args) {
    inst.dispatch(msg, chn, (double)byte1, (double)byte2, stamp);
    dispatch(msg, chn, byte1, byte2, stamp, args...);
  };

  void dsp(){};

  template <typename T, typename... Targs> void dsp(T &inst, Targs &... args) {
    *this += inst.process();
    dsp(args...);
  }

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
      the dispatch & process methods are valid types
      for arguments here.
   */
  template <typename... Targs> const MidiIn &listen(Targs &... args) {
    uint32_t cnt = read();
    set(0.);
    for (uint32_t i = 0; i < cnt; i++)
      dispatch(m_mdata[i].msg, m_mdata[i].chn, m_mdata[i].byte1,
               m_mdata[i].byte2, m_mdata[i].stamp, args...);
    dsp(args...);
    return *this;
  }

  /** Returns a list of devices names/numbers as strings
   */
  const std::vector<std::string> &device_list() { return m_devs; }
};
}

#endif
