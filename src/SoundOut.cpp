//////////////////////////////////////////////////////////////////////
// SoundOut class implementation
// optional dependencies: libsndfile, portaudio
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////
#include "SoundOut.h"
#include <cstring>
#include <functional>
#include <iostream>
#ifdef HAVE_PORTAUDIO
#include <portaudio.h>
#endif
#ifdef HAVE_LIBSNDFILE
#include <sndfile.h>
#endif

int AuLib::rt_audio(const float *input, float *output, unsigned long frameCount,
                    const void *timeInfo, unsigned long statusFlags,
                    AuLib::SoundOut *userData) {
  AuLib::SoundOut &obj = *userData;
  const double *s = obj.m_cbuf.reads();

  if (s)
    for (uint32_t i = 0; i < frameCount * obj.nchnls(); i++)
      output[i] = (float)s[i];
  else
    std::fill(output, output + frameCount * obj.nchnls(), 0.);
  obj.m_framecnt += frameCount;
  return 0;
}

void AuLib::audio(AuLib::SoundOut &obj) {
  if (obj.m_mode == SOUNDOUT_STDOUT) {
    while (obj.m_run) {
      const AuLib::AudioBase &sig = obj.m_cbuf();
      for (auto s = sig.cbegin(); s != sig.cend(); s++)
        std::cout << *s << "\n";
      obj.m_framecnt += obj.m_vframes;
    }
  }
#ifdef HAVE_LIBSNDFILE
  else if (obj.m_mode == SOUNDOUT_SNDFILE) {
    SNDFILE *p = (SNDFILE *)obj.m_handle;
    if (p != NULL) {
      const double *s;
      while (obj.m_run) {
        s = obj.m_cbuf.reads();
        if (s) {
          obj.m_framecnt += sf_writef_double(p, s, obj.m_vframes);
        }
      }
    }
  }
#endif
}

namespace AuLib {
typedef int (*pa_callback_t)(const void *, void *, unsigned long,
                             const PaStreamCallbackTimeInfo *, unsigned long,
                             void *);
}

AuLib::SoundOut::SoundOut(const char *dest, uint32_t nchnls, uint32_t vframes,
                          double sr)
    : AudioBase(nchnls, vframes, sr), m_dest(dest), m_mode(0),
      m_cnt(m_vector.begin()), m_framecnt(0), m_run(false), m_handle(NULL),
      m_cbuf(m_vframes * 4, m_nchnls, m_vframes), thread() {
#ifdef HAVE_PORTAUDIO
  if (m_dest == "dac") {
    // RT audio
    PaError err;
    err = Pa_Initialize();
    if (err == paNoError) {
      PaStreamParameters outparam{0};
      PaStream *stream;
      outparam.device = (PaDeviceIndex)Pa_GetDefaultOutputDevice();
      outparam.channelCount = m_nchnls;
      outparam.sampleFormat = paFloat32;
      outparam.suggestedLatency = (PaTime)(m_vframes / m_sr);
      err = Pa_OpenStream(&stream, NULL, &outparam, m_sr, m_vframes, paNoFlag,
                          (AuLib::pa_callback_t)rt_audio, this);
      if (err == paNoError) {
        err = Pa_StartStream(stream);
        if (err == paNoError) {
          m_handle = (void *)stream;
          m_mode = SOUNDOUT_RT;
        } else {
          m_error = AULIB_RTSTREAM_ERROR;
          m_vframes = 0;
        }
      } else {
        m_error = AULIB_RTOPEN_ERROR;
        m_vframes = 0;
      }
    } else {
      m_error = AULIB_RTINIT_ERROR;
      m_vframes = 0;
    }
  } else
#endif
      if (m_dest == "stdout") {
    // stdout
    m_mode = SOUNDOUT_STDOUT;
    m_handle = nullptr;
  }
#ifdef HAVE_LIBSNDFILE
  else {
    // sndfile
    SF_INFO info{0};
    uint32_t fformat = SF_FORMAT_RAW;
    info.samplerate = (int)m_sr;
    info.channels = m_nchnls;
    if (m_dest.find(".wav") != std::string::npos)
      fformat = SF_FORMAT_WAV;
    else if (m_dest.find(".aif") != std::string::npos)
      fformat = SF_FORMAT_AIFF;
    else if (m_dest.find(".au") != std::string::npos)
      fformat = SF_FORMAT_AU;
    info.format = fformat | SF_FORMAT_PCM_16;
    SNDFILE *sf = sf_open(m_dest.c_str(), SFM_WRITE, &info);
    if (sf != NULL) {
      m_handle = (void *)sf;
      m_mode = SOUNDOUT_SNDFILE;
      m_run = true;
      thread = std::thread(audio, std::ref(*this));
    } else {
      m_error = AULIB_FOPEN_ERROR;
      m_vframes = 0;
    }
  }
#else
  else {
    m_error = AULIB_NOIO_ERROR;
    m_dest = "none";
  }
#endif
}

AuLib::SoundOut::~SoundOut() {
  // wait for cbuf to be consumed 
  while (!m_cbuf.is_empty()) 
    ;
#ifdef HAVE_PORTAUDIO
  if (m_mode == SOUNDOUT_RT && m_handle != NULL) {
    Pa_StopStream((PaStream *)m_handle);
    Pa_CloseStream((PaStream *)m_handle);
    Pa_Terminate();
    return;
  }
#endif
  m_run = false;
  thread.join();
#ifdef HAVE_LIBSNDFILE
  if (m_mode == SOUNDOUT_SNDFILE && m_handle != NULL) {
    sf_close((SNDFILE *)m_handle);
  }
#endif
}

uint64_t AuLib::SoundOut::write(const double *sig, uint32_t frames) {
  uint32_t samples = frames * m_nchnls;
  std::copy(sig, sig + samples, m_cnt);
  m_cnt += samples;
  if (m_cnt == m_vector.end()) {
    m_cbuf.write(*this);
    set(0.);
    m_cnt = m_vector.begin();
  }
  return m_framecnt;
}
