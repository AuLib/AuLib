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
#include <cmath>

using namespace AuLib;

class SineSyn : public Note {
  // signal processing objects
  double m_bend;
  double m_att;
  double m_dec;
  double m_sus;
  double m_rel;
  Adsr m_env;
  Oscili m_osc;


  // DSP override
  virtual const SineSyn &dsp() {
    if (!m_env.is_finished()) 
      set(m_osc(m_env(), m_cps*m_bend));
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
    m_env.reset(m_amp, m_att, m_dec,m_sus * m_amp, m_rel);
  }

  // msg processing
  virtual void on_msg(uint32_t msg, const std::list<double> &data,
                      uint64_t tstamp){

    // pitchbend;
    if(msg == midi::pitchbend) {
      int32_t bnd = (int32_t) data.back();
      bnd = (bnd << 7) | (int32_t) data.front();
      double amnt = (bnd-8192.)/16384.;
      m_bend =  std::pow(2.,amnt/12.);  
    }
    // ctrls: att, dec, sus, rel
    else if(msg == midi::ctrl_msg) {   
      switch((uint32_t) data.front()){
      case 71:
	m_att = data.back()/128.;
        break;
      case 74:
        m_dec = 2.*data.back()/128.;
        break;
      case 52:
 	m_sus = data.back()/128.;
        break;
      case 83:
       	m_rel = 2.*data.back()/128.;
        break;
      }
    }

  };

public:
  SineSyn() : Note(), m_bend(1.), m_att(.01), m_dec(.1), m_sus(0.25), m_rel(.01),
	      m_env(0., m_att, m_dec, m_sus ,m_rel), m_osc()  {
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

