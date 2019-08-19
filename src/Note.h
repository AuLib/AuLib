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

#include "AudioBase.h"
#include <vector>

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

  /** specialise this to handle any incoming msg
  */
  virtual void on_msg(uint32_t msg, const std::vector<double> &data,
                      uint64_t tstamp){};

protected:
  uint64_t m_tstamp;
  double m_num;
  double m_vel;
  int32_t m_chn;
  bool m_on;

  void clear() { set(0.); }

public:
    /** Constructs a note for a given control channel
      (-1 means omni, channels are ignored; this is the default)
    */
    Note(int32_t chn = -1, uint32_t nchnls = def_nchnls, uint32_t vframes = def_vframes,
            double sr = def_sr)
    : AudioBase::AudioBase(nchnls, vframes, sr), m_tstamp(0), m_num(128.),
      m_vel(128.), m_chn(chn), m_on(false) {};

  /** processing method: call it to produce one vector of audio
   */
  const Note &process() { return dsp(); }

  /** checks whether the note is on or off
   */
  bool is_on() const { return m_on; }

  /** onset time for this note
   */
  uint64_t time_stamp() const { return m_tstamp; }

  /** called to turn a note on
   */
  bool note_on(int32_t chn, double num, double vel, uint64_t tstamp);

  /** called to turn a note off for a matching channel chn and
      number num.
   */
  bool note_off(int32_t chn, double num, double vel);

  /** called to turn this note off, regardless of channel or
      note number.
   */
  bool note_off();

  /** called to pass a msg with a list of
      data items
   */
  void ctrl_msg(int32_t chn, uint32_t msg, const std::vector<double> &data,
                uint64_t tstamp) {
    if (m_chn < 0 || m_chn == chn)
      on_msg(msg, data, tstamp);
  }

  void set_chn(uint32_t chn) { m_chn = chn; }
};

/*! \class Note Note.h AuLib/Note.h
 */
}
#endif
