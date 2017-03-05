////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  ScorePlayer.h: Basic score playback class
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SCOREPLAYER_H__
#define __SCOREPLAYER_H__

#include <Note.h>
#include <Score.h>
#include <list>

namespace AuLib {

class ScorePlayer : public AudioBase {

  void sort() {
    auto cmp = [](const Event &a, const Event &b) -> bool {
      if (a.time < b.time)
        return true;
      else
        return false;
    };
    m_events.sort(cmp);
  }

  void dispatch(Event &ev){};

  template <typename T, typename... Targs>
  void dispatch(Event &ev, T &obj, Targs &... args) {
    obj.dispatch(ev.msg, ev.chn, ev.data, ev.time);
    dispatch(ev, args...);
  }

  void dsp(){};

  template <typename T, typename... Targs> void dsp(T &obj, Targs &... args) {
    *this += obj.process();
    dsp(args...);
  }

  void strip() {
    auto it = m_events.begin();
    for (auto &ev : m_events) {
      if (m_time > ev.time * m_sr) {
        it = m_events.erase(it);
      } else
        std::advance(it, 1);
    }
  }

protected:
  const Score &m_score;
  std::list<Event> m_events;
  uint64_t m_time;
  bool m_done;

public:
  /** end command message
   */
  static constexpr uint32_t end = 0;

  /** create a score
   */
  ScorePlayer(const Score &sc, uint32_t nchnls = def_nchnls,
              uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(nchnls, vframes, sr), m_score(sc), m_events(), m_time(0),
        m_done(true){};

  /** process instruments obj ... dispatching score commands, for
      one vector of audio, returning the mixed score audio
   */
  template <typename... Targs> const ScorePlayer &process(Targs &... args);

  /** insert score at the current running time
      with optional offset.
  */
  void insert(const Score &input, double offset = 0.) {
    Event ev;
    for (auto &e : input.score()) {
      ev = e;
      ev.time += m_time / m_sr + offset;
      m_events.push_back(ev);
    }
    sort();
    m_done = false;
  }

  /** get current score time in frames
   */
  uint64_t score_time() const { return m_time; }

  /** reset playback time
     counter
   */
  void reset() { m_time = 0; }

  /** set current score time in frames
   */
  void score_time(uint64_t t) {
    if (m_time > t)
      m_events = m_score.score();
    m_time = t;
    m_done = false;
    strip();
    sort();
  }

  /** set current score time in secs
   */
  void score_time(double t) {
    if (t  <  0.) return; 
    if (m_time > t * m_sr)
      m_events = m_score.score();
    m_time = t * m_sr;
    m_done = false;
    strip();
    sort();
  }

  /** prepare for playback of score object.
      Also resets playback time.
   */
  void prepare() {
    m_time = 0;
    m_events = m_score.score();
    sort();
    m_done = false;
  }

  /** play loaded score from start position to end
      into dest object.
   */
  template <typename T, typename... Targs> bool play(T &dest, Targs... args) {
    if (check_score()) {
      while (!m_done)
        dest(process(args...));
      return true;
    } else
      return false;
  }

  /** check score for termination msg
   */
  bool check_score() const {
    for (auto &ev : m_events) {
      if (ev.msg == 0)
        return true;
    }
    return false;
  }
};

template <typename... Targs>
const ScorePlayer &ScorePlayer::process(Targs &... args) {
  if (!m_done) {
    auto it = m_events.begin();
    for (auto &ev : m_events) {
      uint64_t stime = (ev.time >= 0. ? ev.time : 0.) * m_sr;
      if (stime >= m_time)
        break;
      if (m_time > stime) {
        if (ev.msg == 0) {
          m_done = true;
          set(0.);
          return *this;
        }
        dispatch(ev, args...);
        it = m_events.erase(it);
      } else
        std::advance(it, 1);
    }
  }
  set(0.);
  dsp(args...);
  m_time += vframes();
  return *this;
}
}

/*! \class ScorePlayer ScorePlayer.h AuLib/ScorePlayer.h
 */

#endif
