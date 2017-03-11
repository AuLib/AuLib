//////////////////////////////////////////////////////////////////////
// SoundIn class implementation
// optional dependencies: libsndfile, portaudio
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////
#include "SoundIn.h"
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
                    AuLib::SoundIn *userData) {
  AuLib::SoundIn &obj = *userData;
  for (auto &v : obj.m_buffer)
    v = *input++;
  while (!obj.m_cbuf.writes(obj.m_buffer.data()))
    ;
  return 0;
}

void AuLib::audio(AuLib::SoundIn &obj) {
  bool written = true;
  while (obj.m_run) {
    if (obj.m_mode == SOUNDIN_STDIN) {
      for (auto &v : obj.m_buffer)
        std::cin >> v;
      written = obj.m_cbuf.writes(obj.m_buffer.data());
    }
#ifdef HAVE_LIBSNDFILE
    else if (obj.m_mode == SOUNDIN_SNDFILE) {
      SNDFILE *p = (SNDFILE *)obj.m_handle;
      if (p != NULL) {
        uint32_t n = obj.m_buffer.size() / obj.m_nchnls;
        if (written) {
          sf_readf_double(p, obj.m_buffer.data(), n);
        }
        written = obj.m_cbuf.writes(obj.m_buffer.data());
      }
    }
#endif
  }
}

namespace AuLib {
typedef int (*pa_callback_t)(const void *, void *, unsigned long,
                             const PaStreamCallbackTimeInfo *, unsigned long,
                             void *);
}

AuLib::SoundIn::SoundIn(const char *src, uint32_t nchnls, uint32_t vframes,
                        uint64_t bframes, double sr)
    : AudioBase(nchnls, vframes, sr), m_src(src), m_mode(0),
      m_dur(std::numeric_limits<int64_t>::max()), m_framecnt(0), m_run(false),
      m_cbuf(4 * bframes, m_nchnls, bframes),
      m_buffer(npow2(bframes) * m_nchnls), m_cnt(m_cbuf.cend()), m_handle(NULL),
      thread() {
#ifdef HAVE_PORTAUDIO
  if (m_src == "adc") {
    // RT audio
    PaError err;
    err = Pa_Initialize();
    if (err == paNoError) {
      PaStreamParameters inparam{0};
      PaStream *stream;
      inparam.device = (PaDeviceIndex)Pa_GetDefaultInputDevice();
      inparam.channelCount = m_nchnls;
      inparam.sampleFormat = paFloat32;
      inparam.suggestedLatency = (PaTime)(m_vframes / m_sr);
      err = Pa_OpenStream(&stream, &inparam, NULL, m_sr, bframes, paNoFlag,
                          (pa_callback_t)rt_audio, this);
      if (err == paNoError) {
        err = Pa_StartStream(stream);
        if (err == paNoError) {
          m_handle = (void *)stream;
          m_mode = SOUNDIN_RT;
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
      if (m_src == "stdin") {
    // stdout
    m_mode = SOUNDIN_STDIN;
    m_handle = nullptr;
    m_run = true;
    thread = std::thread(audio, std::ref(*this));
  }
#ifdef HAVE_LIBSNDFILE
  else {
    // sndfile
    SF_INFO info{0};
    SNDFILE *sf = sf_open(m_src.c_str(), SFM_READ, &info);
    if (sf != NULL) {
      m_handle = (void *)sf;
      m_mode = SOUNDIN_SNDFILE;
      m_sr = info.samplerate;
      if (m_nchnls != (uint32_t)info.channels) {
        m_nchnls = info.channels;
        m_vector.resize(m_nchnls * m_vframes);
      }
      m_dur = (uint64_t)info.frames;
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
    m_src = "none";
  }
#endif
}

AuLib::SoundIn::~SoundIn() {
#ifdef HAVE_PORTAUDIO
  if (m_mode == SOUNDIN_RT && m_handle != NULL) {
    Pa_StopStream((PaStream *)m_handle);
    Pa_CloseStream((PaStream *)m_handle);
    Pa_Terminate();
    return;
  }
#endif
  m_run = false;
  thread.join();
#ifdef HAVE_LIBSNDFILE
  if (m_mode == SOUNDIN_SNDFILE && m_handle != NULL) {
    sf_close((SNDFILE *)m_handle);
  }
#endif
}

const double *AuLib::SoundIn::read(uint32_t frames) {
  uint32_t samples = frames * m_nchnls;
  if (m_cnt == m_cbuf.cend()) {
    m_cbuf.read();
    m_cnt = m_cbuf.cbegin();
  }
  std::copy(m_cnt, m_cnt + samples, m_vector.begin());
  m_framecnt += frames;
  m_cnt += samples;
  return vector();
}
