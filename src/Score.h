////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  Score.h: Basic score building and playback class
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SCORE_H__
#define __SCORE_H__

#include <Note.h>
#include <fstream>
#include <list>
#include <sstream>

namespace AuLib {

/** Basic score model
 */
class Score : public AudioBase {

  struct Event {
    uint32_t chn;
    uint32_t msg;
    std::vector<double> data;
    uint64_t time;
  };

  void add_event(const Event &e) {
    m_score.push_back(e);
    m_events.push_back(e);
    m_is_sorted = false;
    m_done = false;
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
      if (m_time > ev.time){
	auto d = it;
	std::advance(it, 1);
        m_events.erase(d);
      }else std::advance(it, 1);
    }
  }

public:
  struct Cmd {
    std::string cmd;
    bool mode;
    uint32_t msg;
    uint32_t len;
    static constexpr uint32_t omni = true;
    static constexpr uint32_t chn = false;
  };

protected:
  std::list<Event> m_events, m_score;
  std::list<Cmd> m_cmds;
  uint64_t m_time;
  bool m_is_sorted;
  bool m_done;

  virtual void parse_stream(std::istream &input, uint64_t offset = 0) {
    std::string cmd;
    double time;
    Event ev;
    while (input >> cmd) {
      for (auto &c : m_cmds) {
        if (c.cmd == cmd) {
          ev.msg = c.msg;
          ev.data.resize(c.len);
          if (c.mode != Cmd::omni)
            input >> ev.chn;
          input >> time;
          ev.time = (uint64_t)((time + offset) * m_sr);
          for (auto &d : ev.data)
            input >> d;
          add_event(ev);
        }
      }
    }
  }

public:
  /** end command message
   */
  static constexpr uint32_t end = 0;

  /** create a score
   */
  Score(uint32_t nchnls = def_nchnls, uint32_t vframes = def_vframes,
        double sr = def_sr)
      : AudioBase(nchnls, vframes, sr), m_events(), m_score(), m_cmds(),
        m_time(0), m_is_sorted(false), m_done(true){};

  /** Add command with the following format \n
      { name,  // string \n
        type,  // Score::Cmd::omni or !Score::Cmd::omni \n
        msg,   // uint32_t message code  \n
        len    // uint32_t number of data items (doubles) in cmd \n
       } \n
   **/
  void add_cmd(const Cmd &c) { m_cmds.push_back(c); }

  /** read file stream with commands
   */
  void read(std::ifstream &input) { parse_stream(input); }

  /** read string with commands
   */
  void read(std::string &input) {
    std::istringstream s;
    s.str(input);
    parse_stream(s);
  }

  /** read stream with commands
   */
  void read(std::istream &input) { parse_stream(input); }

  /** insert stream with commands
      at the current running time
  */
  void insert(std::istream &input) { parse_stream(input, m_time); }

  /** insert stream with commands
      with time offset
  */
  void insert(std::istream &input, uint64_t offset) {
    parse_stream(input, offset);
  }

  /** process instruments obj ... dispatching score commands, for
      one vector of audio, returning the mixed score audio
   */
  template <typename... Targs> const Score &process(Targs &... args) {
    if (!m_done) {
      auto it = m_events.begin();
      for (auto &ev : m_events) {
        if (m_is_sorted && ev.time >= m_time)
          break;
        if (m_time > ev.time) {
          if (ev.msg == 0) {
            m_done = true;
            break;
          }
          dispatch(ev, args...);
	  auto d = it;
	  std::advance(it, 1);
          m_events.erase(d);
        } else std::advance(it, 1);
      }
    }
    set(0.);
    dsp(args...);
    m_time += vframes();
    return *this;
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

  /** get current score time in frames
   */
  uint64_t score_time() const { return m_time; }

  /** set current score time in frames
   */
  void score_time(uint64_t t) {
    if (m_time > t)
      m_events = m_score;
    m_time = t;
    m_done = false;
    strip();
  }

  /** set current score time in secs
   */
  void score_time(double t) {
    if (m_time > t * m_sr)
      m_events = m_score;
    m_time = t * m_sr;
    m_done = false;
    strip();
  }

  /** rewind score
   */
  void rewind() {
    m_time = 0;
    m_events = m_score;
    m_done = false;
  }

  /** clear the stored score
   */
  void clear() { m_score.clear(); }

  /** play score from start position to end
      into dest object.
   */
  template <typename T, typename... Targs> bool play(T &dest, Targs... args) {
    sort();
    if (check_score()) {
      while (!m_done)
        dest(process(args...));
      return true;
    } else
      return false;
  }

  /** sorts the score in ascending time order
      for 'sorted' playback.
   */
  void sort() {
    auto cmp = [](const Event &a, const Event &b) -> bool {
      if (a.time < b.time)
        return true;
      else
        return false;
    };
    m_events.sort(cmp);
    m_is_sorted = true;
  }
};
}

#endif
