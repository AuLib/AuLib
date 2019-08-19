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

#include "Note.h"
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
  std::array<std::array<uint32_t, 17>, 128> m_val;
  std::array<std::array<uint32_t, 17>, 128> m_aval;
  std::array<uint32_t, 17> m_tval;
  std::array<uint32_t, 17> m_pval;
  uint32_t m_cnt;

  void list_devices();

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

  /** returns a MIDI control value for channel chn and control num,
      chn < 0  means all channels. \n Results in the range [0.,1.)
   */
  double ctlval(int32_t chn, uint32_t num) {
    if (chn < 0 || chn > 15)
      chn = 16;
    for (uint32_t i = 0; i < m_cnt; i++)
      if (m_mdata[i].msg == midi::ctrl_msg && m_mdata[i].byte1 == num &&
          (chn == 16 || (int32_t)m_mdata[i].chn == chn)) {
        m_val[chn][num] = m_mdata[i].byte2;
      }
    return m_val[chn][num] / 128.;
  }

  /** returns a MIDI aftertouch value for channel chn and note num,
      chn < 0  means all channels.\n Results in the range [0.,1.)
   */
  double aftval(int32_t chn, uint32_t num) {
    if (chn < 0 || chn > 15)
      chn = 16;
    for (uint32_t i = 0; i < m_cnt; i++)
      if (m_mdata[i].msg == midi::poly_aftouch && m_mdata[i].byte1 == num &&
          (chn == 16 || (int32_t)m_mdata[i].chn == chn)) {
        m_aval[chn][num] = m_mdata[i].byte2;
      }
    return m_aval[chn][num] / 128.;
  }

  /** returns a MIDI aftertouch value for channel chn,
      chn < 0  means all channels.\n Results in the range [0.,1.)
   */
  double aftval(int32_t chn) {
    if (chn < 0 || chn > 15)
      chn = 16;
    for (uint32_t i = 0; i < m_cnt; i++)
      if (m_mdata[i].msg == midi::aftouch &&
          (chn == 16 || (int32_t)m_mdata[i].chn == chn)) {
        m_tval[chn] = m_mdata[i].byte1;
      }
    return m_tval[chn] / 128.;
  }

  /** returns a MIDI program value for channel chn,
      chn < 0  means all channels.
   */
  uint32_t program(int32_t chn) {
    if (chn < 0 || chn > 15)
      chn = 16;
    for (uint32_t i = 0; i < m_cnt; i++)
      if (m_mdata[i].msg == midi::aftouch &&
          (chn == 16 || (int32_t)m_mdata[i].chn == chn)) {
        m_pval[chn] = m_mdata[i].byte1;
      }
    return m_pval[chn];
  }

  /** read MIDI input,
      returning number of MIDI events read. \n
      Only needs to be called if listen() is not used
   */
  uint32_t read();

  /** Returns a list of devices names/numbers as strings
   */
  const std::vector<std::string> &device_list() { return m_devs; }
};
}

#endif
