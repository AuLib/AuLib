////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  Score.h: Basic score-building class
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SCORE_H__
#define __SCORE_H__

#include <fstream>
#include <list>
#include <sstream>

namespace AuLib {

/** single score event
 */
struct Event {
  uint32_t chn;
  uint32_t msg;
  std::vector<double> data;
  double time;
};

/** Basic score model
 */
class Score {
public:
  /** score command
   */
  struct Cmd {
    std::string cmd;
    bool mode;
    uint32_t msg;
    uint32_t len;
    static constexpr uint32_t omni = true;
    static constexpr uint32_t chn = false;
  };

private:
  std::list<Event> m_score;
  std::list<Cmd> m_cmds;

  void parse_stream(std::istream &input, double offset = 0.) {
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
          ev.time = (time + offset);
          for (auto &d : ev.data)
            input >> d;
          add_event(ev);
        }
      }
    }
  }

  void add_event(const Event &e) { m_score.push_back(e); }

public:
  /** end command message
   */
  static constexpr uint32_t end = 0;

  /** create a score
   */
  Score() : m_score(), m_cmds(){};

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

  /** clear the stored score
   */
  void clear() { m_score.clear(); }

  /** get the score
   */
  const std::list<Event> &score() const { return m_score; }
};

/*! \class Score Score.h AuLib/Score.h
 */
}

#endif
