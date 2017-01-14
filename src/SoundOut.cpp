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
#include <iostream>
#ifdef HAVE_PORTAUDIO
#include <portaudio.h>
#endif
#ifdef HAVE_LIBSNDFILE
#include <sndfile.h>
#endif

AuLib::SoundOut::SoundOut(const char *dest, uint32_t nchnls, uint32_t vframes,
                          double sr)
    : AudioBase(nchnls, vframes, sr), m_dest(dest), m_mode(0), m_cnt(0),
      m_framecnt(0), m_handle(NULL) {

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
                          NULL, NULL);
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
    m_cnt = 0;
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
#ifdef HAVE_PORTAUDIO
  if (m_mode == SOUNDOUT_RT && m_handle != NULL) {
    Pa_StopStream((PaStream *)m_handle);
    Pa_CloseStream((PaStream *)m_handle);
    Pa_Terminate();
  }
#endif
#ifdef HAVE_LIBSNDFILE
  if (m_mode == SOUNDOUT_SNDFILE && m_handle != NULL) {
    sf_close((SNDFILE *)m_handle);
  }
#endif
}

uint64_t AuLib::SoundOut::write(const double *sig, uint32_t frames) {
  uint32_t samples = frames * m_nchnls;
#ifdef HAVE_PORTAUDIO
  if (m_mode == SOUNDOUT_RT && m_handle != NULL) {
    PaError err;
    uint32_t bsamples = m_vframes * m_nchnls;
    float *buffer = (float *)m_vector.data();
    for (uint32_t i = 0; i < samples; i++) {
      buffer[m_cnt++] = sig[i];
      if (m_cnt == bsamples) {
        err = Pa_WriteStream((PaStream *)m_handle, buffer, m_vframes);
        if (err == paNoError) {
          m_framecnt += m_cnt / m_nchnls;
        }
        set(0.);
        m_cnt = 0;
      }
    }
  } else
#endif
      if (m_mode == SOUNDOUT_STDOUT) {
    uint32_t sample_cnt = 0;
    for (uint32_t i = 0; i < samples; i++) {
      std::cout << sig[i] << "\n";
      sample_cnt++;
    }
    m_framecnt += sample_cnt / m_nchnls;
  }
#ifdef HAVE_LIBSNDFILE
  else if (m_mode == SOUNDOUT_SNDFILE && m_handle != NULL) {
    uint32_t bsamples = m_vframes * m_nchnls;
    for (uint32_t i = 0; i < samples; i++) {
      m_vector[m_cnt++] = sig[i];
      if (m_cnt == bsamples) {
        m_framecnt +=
            sf_writef_double((SNDFILE *)m_handle, m_vector.data(), m_vframes);
        set(0.);
        m_cnt = 0;
      }
    }
  }
#endif
  else
    return 0;
  return m_framecnt;
}
