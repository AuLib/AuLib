////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016-7 V Lazzarini
//  MidiIn.h: MIDI listener implementation with PortMidi
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#include <MidiIn.h>
#ifdef HAVE_PORTMIDI
#include <portmidi.h>
#include <porttime.h>

AuLib::MidiIn::MidiIn()
    : m_mstream(nullptr), m_devs(), m_mdata({}), m_val({}), m_aval({}),
      m_tval({}), m_pval({}), m_cnt(0) {
  Pm_Initialize();
  list_devices();
};

AuLib::MidiIn::~MidiIn() { Pm_Terminate(); }

uint32_t AuLib::MidiIn::open(int dev) {
  PmError ret = Pm_OpenInput(((PortMidiStream **)&m_mstream), dev, NULL, 512L,
                             NULL, NULL);
  if (ret == pmNoError) {
    Pt_Start(1, NULL, NULL);
    return 0;
  } else
    return AULIB_ERROR;
}

void AuLib::MidiIn::close() { Pm_Close((PortMidiStream *)m_mstream); }

uint32_t AuLib::MidiIn::read() {
  if (Pm_Poll(m_mstream)) {
    uint32_t n = 0;
    uint32_t cnt = 0;
    std::array<PmEvent, 32> msg{};
    do {
      cnt = Pm_Read(m_mstream, msg.data(), 32);
      for (uint32_t i = 0; i < cnt; i++, n++) {
        if (n < 1024) {
          m_mdata[n].msg = Pm_MessageStatus(msg[i].message) & 0xF0;
          m_mdata[n].chn = Pm_MessageStatus(msg[i].message) & 0x0F;
          m_mdata[n].byte1 = Pm_MessageData1(msg[i].message);
          m_mdata[n].byte2 = Pm_MessageData2(msg[i].message);
          m_mdata[n].stamp = msg[i].timestamp;
        } else
          break;
      }
    } while (cnt > 0 && n < 1024);
    m_cnt = n;
    return n;
  }
  return 0;
}

void AuLib::MidiIn::list_devices() {
  uint32_t n = Pm_CountDevices();
  if (n) {
    const PmDeviceInfo *info;
    for (uint32_t i = 0; i < n; i++) {
      info = Pm_GetDeviceInfo(i);
      if (info->input)
        m_devs.push_back(std::string(std::to_string(i) + ": " + info->name));
    }
  } else
    m_devs.push_back(std::string("no devices\n"));
}
#else
// stubs
AuLib::MidiIn::MidiIn() : m_mstream(nullptr), m_devs() { list_devices(); };

AuLib::MidiIn::~MidiIn(){};

uint32_t AuLib::MidiIn::open(int dev) {
  if (m_mstream)
    return 1;
  else
    return 0;
}

void AuLib::MidiIn::close(){};

uint32_t AuLib::MidiIn::read() { return 0; }

void AuLib::MidiIn::list_devices() {
  m_devs.push_back(std::string("no devices\n"));
}
#endif
