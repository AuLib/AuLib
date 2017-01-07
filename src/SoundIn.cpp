//////////////////////////////////////////////////////////////////////
// SoundIn class implementation
// dependencies: libsndfile, portaudio
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
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>

AuLib::SoundIn::SoundIn(const char *src, uint32_t nchnls, uint32_t vframes,
                        uint64_t bframes, double sr)
    : AudioBase(nchnls, vframes, sr), m_src(src), m_mode(0),
      m_cnt(bframes * nchnls), m_dur(std::numeric_limits<uint64_t>::max()),
      m_framecnt(0), m_inbuf(bframes * nchnls), m_handle(NULL) {
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
      inparam.suggestedLatency = (PaTime)(bframes / m_sr);
      err = Pa_OpenStream(&stream, &inparam, NULL, m_sr, bframes, paNoFlag,
                          NULL, NULL);
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
  } else if (m_src == "stdin") {
    // stdout
    m_mode = SOUNDIN_STDIN;
  } else {
    // sndfile
    SF_INFO info{0};
    SNDFILE *sf = sf_open(m_src.c_str(), SFM_READ, &info);
    if (sf != NULL) {
      m_handle = (void *)sf;
      m_mode = SOUNDIN_SNDFILE;
      m_sr = info.samplerate;
      if (m_nchnls != info.channels) {
        m_nchnls = info.channels;
        m_vector.resize(m_nchnls * m_vframes);
      }
      m_dur = (uint64_t)info.frames;
    } else {
      m_error = AULIB_FOPEN_ERROR;
      m_vframes = 0;
    }
  }
}

AuLib::SoundIn::~SoundIn() {
  if (m_mode == SOUNDIN_RT && m_handle != NULL) {
    Pa_StopStream((PaStream *)m_handle);
    Pa_CloseStream((PaStream *)m_handle);
    Pa_Terminate();
  } else if (m_mode == SOUNDIN_SNDFILE && m_handle != NULL) {
    sf_close((SNDFILE *)m_handle);
  }
}

const double *AuLib::SoundIn::read(uint32_t frames) {
  uint32_t samples = frames * m_nchnls;
  if (m_mode == SOUNDIN_RT && m_handle != NULL) {
    PaError err;
    uint32_t bsamples = m_inbuf.size();
    float *buffer = (float *)m_inbuf.data();
    for (uint32_t i = 0; i < samples; i++) {
      if (m_cnt == bsamples) {
        err = Pa_ReadStream((PaStream *)m_handle, buffer, bsamples / m_nchnls);
        if (err == paNoError) {
          m_framecnt += m_cnt / m_nchnls;
        }
        m_cnt = 0;
      }
      m_vector[i] = (double)buffer[m_cnt++];
    }
  } else if (m_mode == SOUNDIN_STDIN) {
    uint32_t sample_cnt = 0;
    for (uint32_t i = 0; i < samples; i++) {
      std::cin >> m_vector[i];
      sample_cnt++;
    }
    m_framecnt += sample_cnt / m_nchnls;
  } else if (m_mode == SOUNDIN_SNDFILE && m_handle != NULL) {
    uint32_t bsamples = m_inbuf.size();
    for (uint32_t i = 0; i < samples; i++) {
      if (m_cnt == bsamples) {
        m_framecnt += sf_readf_double((SNDFILE *)m_handle, m_inbuf.data(),
                                      bsamples / m_nchnls);
        m_cnt = 0;
      }
      m_vector[i] = m_inbuf[m_cnt++];
    }
  } else
    return NULL;
  return vector();
}
