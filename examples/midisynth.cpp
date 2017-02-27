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

#include <Note.h>
#include <MidiIn.h>
#include <Adsr.h>
#include <AudioBase.h>
#include <Oscili.h>
#include <SoundOut.h>
#include <atomic>
#include <csignal>

using namespace AuLib;

class SineSyn : public Note {
  // signal processing objects
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
  virtual void off_note() {
    m_env.release();
  }

  // note on processing
  virtual void on_note() {
    m_env.reset(m_amp, 0.01, 0.5, 0.25 * m_amp, 0.01);
  }

public:
  SineSyn() : Note(), m_env(0., 0.01, 0.01, 0., 0.01), m_osc() {
    m_env.release();
  };
};

// handle ctrl-c
static std::atomic_bool running(true);
void signal_handler(int signal) {
  running = false;
  std::cout << "\nexiting...\n";
}


int main() {
  int dev;
  Instrument<SineSyn> synth(8);
  SoundOut out("dac");
  MidiIn midi;
  std::signal(SIGINT, signal_handler);

  std::cout << "Available MIDI inputs:\n";
  for(auto &devs : midi.device_list()) 
    std::cout << devs << std::endl;
  std::cout << "choose a device: ";
  std::cin >> dev;
  
  if (midi.open(dev) == AULIB_NOERROR) {
    std::cout << "running... (use ctrl-c to close)\n";
    while (running) 
      out(midi.listen(synth)); // listen to midi on behalf of synth
    
  } else
    std::cout << "error opening device...\n";
  std::cout << "...finished \n";
  return 0;
}

