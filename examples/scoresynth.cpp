////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  scoresynth.cpp: simple score-base synthesiser
//  demonstrating Note and Instrument classes
//
//  score format:
//    $command chn time(s) [data1 data2 [, data3, data4]
//  where command is either on, off, adsr or end. The optional
//  parameters only apply to the first three commands.
//
//  example:
//  $on 0 0 64 25
//  $off 0 3 64  0
//  $adsr 1 1 0.5 0.5 0.2 0.01
//  $on 1 1 64.5 25
//  $off 1 3 64.5  0
//  $end 4
//
//  comments can be used freely outside commands (but not inside them)
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////

#include <Adsr.h>
#include <AudioBase.h>
#include <BlOsc.h>
#include <Note.h>
#include <Oscili.h>
#include <Score.h>
#include <ScorePlayer.h>
#include <SoundOut.h>

using namespace AuLib;

// sine wave note
class SineSyn : public Note {
  // signal processing objects
  double m_att;
  double m_dec;
  double m_sus;
  double m_rel;
  Adsr m_env;
  Oscili m_osc;

  // DSP override
  virtual const SineSyn &dsp() {
    if (!m_env.is_finished())
      set(m_osc(m_env(), m_cps));
    else
      clear();
    return *this;
  }

  // note off processing
  virtual void off_note() { m_env.release(); }

  // note on processing
  virtual void on_note() {
    m_env.reset(m_amp, m_att, m_dec, m_sus * m_amp, m_rel);
  }

  // msg processing
  virtual void on_msg(uint32_t msg, const std::vector<double> &data,
                      uint64_t tstamp) {
    if (msg == adsr) {
      m_att = data[0];
      m_dec = data[1];
      m_sus = data[2];
      m_rel = data[3];
    }
  }

public:
  // msg definition for adsr
  static constexpr uint32_t adsr = 1;

  SineSyn(int32_t chn = -1, Oscili osc = Oscili())
      : Note(chn), m_att(.01), m_dec(.1), m_sus(0.25), m_rel(.01),
        m_env(0., m_att, m_dec, m_sus, m_rel), m_osc(osc) {
    m_env.release();
  };
};

// sawtooth note
struct SawSyn : SineSyn {
  SawSyn(int chn = -1) : SineSyn(chn, (Oscili)SawOsc()){};
};

int main(int argc, const char **argv) {

  if (argc > 1) {
    Instrument<SineSyn> sinesynth(8, 0); // 8 voices, channel 0
    Instrument<SawSyn> sawsynth(8, 1);   // 8 voices, channel 1
    SoundOut out("dac");
    Score score;
    ScorePlayer player(score);
    std::ifstream input(argv[1]);

    score.add_cmd({"$on", !Score::Cmd::omni, midi::note_on, 2});
    score.add_cmd({"$off", !Score::Cmd::omni, midi::note_off, 2});
    score.add_cmd({"$adsr", !Score::Cmd::omni, SineSyn::adsr, 4});
    score.add_cmd({"$end", Score::Cmd::omni, Score::end, 0});

    score.read(input);
    player.prepare(); // prepare playback
    if (player.play(out, sinesynth, sawsynth) == false) {
      std::cout << "score parse error: "
                << "no termination message found\n";
      return 1;
    }
    player.score_time(1.); // play from 1. sec
    player.play(out, sinesynth, sawsynth);
    player.prepare();           // rewind
    player.insert(score, 0.75); // add another score at 0.75 secs.
    player.play(out, sinesynth, sawsynth);
    return 0;
  } else
    std::cout << "usage:\n" << argv[0] << " score.txt\n";

  return 1;
}
