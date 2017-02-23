////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// Note.h: note and instrument modelling classes
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __NOTE_H__
#define __NOTE_H__

#include <AudioBase.h>

namespace AuLib {

class Note : public AudioBase {

  virtual const Note &dsp() { return *this; }
  virtual void on_note() {};
  virtual void off_note() {};

protected:
  uint64_t m_tstamp;
  uint32_t m_num;
  uint32_t m_vel;
  int32_t m_chn;
  bool m_on;
  double m_cps;
  double m_amp;

  void clear() { set(0.); }

public:
  Note(int32_t chn = -1)
      : m_tstamp(0), m_num(128), m_vel(128), m_chn(chn), m_on(false), m_cps(0.),
        m_amp(0.){};

  const Note &process() {
    return dsp();
    return *this;
  }

  bool is_on() const { return m_on; }
  uint64_t time_stamp() const { return m_tstamp; }
  bool note_on(uint32_t chn, uint32_t num, uint32_t vel, uint64_t tstamp);
  bool note_off(uint32_t chn, uint32_t num, uint32_t vel);
  bool note_off();
};

template <typename T> class Instrument : public AudioBase {
  static constexpr int note_on = 0x90;
  static constexpr int note_off = 0x80;

  std::vector<T> m_voices;
  virtual void poly(uint32_t msg, uint32_t chn, uint32_t num, uint32_t vel,
                    uint64_t stamp);

  virtual const Instrument &dsp() {
    set(0.);
    for(auto &note: m_voices) {
      note.process();
      *this += note;
    }
    return *this;
  }

public:
  Instrument(uint32_t nvoices) : m_voices(nvoices) {}
  void dispatch(uint32_t msg, uint32_t chn, uint32_t note, uint32_t vel,
                uint64_t stamp) {
    poly(msg, chn, note, vel, stamp);
  }

  virtual const Instrument &process() { return dsp(); }
};

template <typename T>
void Instrument<T>::poly(uint32_t msg, uint32_t chn, uint32_t num, uint32_t vel,
                         uint64_t stamp) {
  if (vel == 0)
    msg = note_off;
  if (msg == note_on) {
    for (auto &note : m_voices) {
      if (!note.is_on()) {
        note.note_on(chn, num, vel, stamp);
        return;
      }
    }
    auto ostamp = m_voices[0].time_stamp();
    auto oldest = &m_voices[0];
    for (auto &note : m_voices) {
      if (note.time_stamp() < ostamp) {
        oldest = &note;
        ostamp = note.time_stamp();
      }
    }
    oldest->note_off();
    oldest->note_on(chn, num, vel, stamp);
  } else if (msg == note_off) {
    for (auto &note : m_voices) {
      note.note_off(chn, num, vel);
    }
  }
}
}
#endif
