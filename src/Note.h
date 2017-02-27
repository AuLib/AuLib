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

/** Note class: \n
    Base class for modelling synthesiser notes
*/
class Note : public AudioBase {

  /** specialise this to contain your
      signal processing objects
  */
  virtual const Note &dsp() { return *this; }

  /** specialise this to handle any
      note onset processing (e.g. envelope resets etc)
  */
  virtual void on_note(){};

  /** specialise this to handle any
      note termination processing (e.g. envelope releases etc)
  */
  virtual void off_note(){};

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
  /** Constructs a note for a given channel
      (-1 means omni, channels are ignored; this is the default)
  */
  Note(int32_t chn = -1)
      : m_tstamp(0), m_num(128), m_vel(128), m_chn(chn), m_on(false), m_cps(0.),
        m_amp(0.){};

  /** processing method: call it to produce one vector of audio
   */
  const Note &process() {
    return dsp();
    return *this;
  }

  /** checks whether the note is on or off
   */
  bool is_on() const { return m_on; }

  /** onset time for this note
   */
  uint64_t time_stamp() const { return m_tstamp; }

  /** called to turn a note on
   */
  bool note_on(int32_t chn, uint32_t num, uint32_t vel, uint64_t tstamp);

  /** called to turn a note off for a matching channel chn and
      number num.
   */
  bool note_off(int32_t chn, uint32_t num, uint32_t vel);

  /** called to turn this note off, regardless of channel or
      note number.
   */
  bool note_off();
};

template <typename T> class Instrument : public AudioBase {
  static constexpr int note_on = 0x90;
  static constexpr int note_off = 0x80;

  std::vector<T> m_voices;

  /** specialise this to handle poliphony and
      dispatched messages. The base implementation uses
      last-note priority, parsing MIDI note on and note off
      messages only.
  */
  virtual void msg_handler(uint32_t msg, uint32_t chn, uint32_t num,
                           uint32_t vel, uint64_t stamp);

  /** basic processing method, can be specialised.
   */
  virtual const Instrument &dsp() {
    set(0.);
    for (auto &note : m_voices) {
      note.process();
      *this += note;
    }
    return *this;
  }

public:
  /** Construct an instrument with nvoices polyphony
   */
  Instrument(uint32_t nvoices) : m_voices(nvoices) {}

  /** Dispatch a channel message using a MIDI-like format. \n
     msg - message type \n
     chn - channel \n
     data1 - message data 1 \n
     data2 - message data 2 \n
     stamp - time stamp \n\n
     Note that message types are not constrained to MIDI ones,
     specialisations of this class might implement their own.
     Channels and data are not limited to four and seven bits,
     but can use the full unsigned 32-bit range.
   */
  void dispatch(uint32_t msg, uint32_t chn, uint32_t data1, uint32_t data2,
                uint64_t stamp) {
    msg_handler(msg, chn, data1, data2, stamp);
  }

  /** processing interface
   */
  virtual const Instrument &process() { return dsp(); }
};

/** @private
    msg_handler implementation.
*/
template <typename T>
void Instrument<T>::msg_handler(uint32_t msg, uint32_t chn, uint32_t num,
                                uint32_t vel, uint64_t stamp) {
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
