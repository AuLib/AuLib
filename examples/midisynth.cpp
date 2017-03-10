////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
// midisynth.cpp: simple polyphonic midi instrument
//  demonstrating Note, Instrument and MidiIn classes
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
#include <MidiIn.h>
#include <Note.h>
#include <Oscili.h>
#include <SoundOut.h>
#include <atomic>
#include <cmath>
#include <csignal>
#include <map>

using namespace AuLib;

class SineSyn : public Note {

  // control list
  uint32_t m_atn, m_dcn, m_ssn, m_rln;
  std::map<uint32_t, double> m_ctl;
  double m_bend;

  // signal processing objects
  Adsr m_env;
  Oscili m_osc;

  // DSP override
  virtual const SineSyn &dsp() {
    if (!m_env.is_finished())
      set(m_osc(m_env(), m_cps * m_bend));
    else
      clear();
    return *this;
  }

  // note off processing
  virtual void off_note() { m_env.release(); }

  // note on processing
  virtual void on_note() {
    m_env.reset(m_amp, m_ctl[m_atn] + 0.001, m_ctl[m_dcn] + 0.001,
                m_ctl[m_ssn] * m_amp, m_ctl[m_rln] + 0.001);
  }

  // msg processing
  virtual void on_msg(uint32_t msg, const std::vector<double> &data,
                      uint64_t tstamp) {

    // pitchbend;
    if (msg == midi::pitchbend) {
      int32_t bnd = (int32_t)data[1];
      bnd = (bnd << 7) | (int32_t)data[0];
      double amnt = (bnd - 8192.) / 16384.;
      m_bend = std::pow(2., (4.*amnt) / 12.);
    }
    // ctrls: att, dec, sus, rel
    else if (msg == midi::ctrl_msg) {
      uint32_t num = (uint32_t)data[0];
      m_ctl[num] = data[1] / 128.;
    }
  };

public:
  typedef std::array<int, 4> ctl_list;

  SineSyn(int32_t chn, SineSyn::ctl_list lst, Oscili osc = Oscili())
      : Note(chn), m_atn(lst[0]), m_dcn(lst[1]), m_ssn(lst[2]), m_rln(lst[3]),
        m_ctl({{m_atn, 0.01}, {m_dcn, 0.01}, {m_ssn, 0.25}, {m_rln, 0.01}}),
        m_bend(1.),
        m_env(0., m_ctl[m_atn], m_ctl[m_dcn], m_ctl[m_ssn], m_ctl[m_rln]),
        m_osc(osc) {
    m_env.release();
  };
};

// sawtooth note
struct SawSyn : SineSyn {
  SawSyn(int chn, SineSyn::ctl_list lst)
      : SineSyn(chn, lst, (Oscili)SawOsc()){};
};

// handle ctrl-c
static std::atomic_bool running(true);
void signal_handler(int signal) {
  running = false;
  std::cout << "\nexiting...\n";
}

int main() {
  int dev;

  // Sinewave Synthesizer -
  // 8 voices, channel 0
  // control numbers: 71 - att, 74 - dec, 52 - sus, 83 - rel
  Instrument<SineSyn, SineSyn::ctl_list> sinsynth(8, 1, {{71, 74, 52, 83}});
  // Sawtooh Synthesizer -
  // 8 voices, channel 0
  Instrument<SawSyn, SineSyn::ctl_list> sawsynth(8, 0, {{71, 74, 52, 83}});

  SoundOut out("dac",1,128);
  MidiIn midi;
  std::signal(SIGINT, signal_handler);

  std::cout << "Available MIDI inputs:\n";
  for (auto &devs : midi.device_list())
    std::cout << devs << std::endl;
  std::cout << "choose a device: ";
  std::cin >> dev;

  if (midi.open(dev) == AULIB_NOERROR) {
    std::cout << "running... (use ctrl-c to close)\n";
    while (running) {
      out(midi.listen(sinsynth,
                      sawsynth)); // listen to midi on behalf of sinsynth
    }
  } else
    std::cout << "error opening device...\n";
  std::cout << "...finished \n";
  return 0;
}
