////////////////////////////////////////////////////////////////////
// SoundIn class: audio input
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SOUNDIN_H
#define _SOUNDIN_H

#include "AudioBase.h"
#include "Circular.h"
#include <thread>

namespace AuLib {

enum { SOUNDIN_RT = 1, SOUNDIN_STDIN, SOUNDIN_SNDFILE };

/** Standard Error messages
*/
const std::string soundin_error[] = {
    "SoundIn: file open error",       "SoundIn: RT initialisation error",
    "SoundIn: RT open error",         "SoundIn: RT stream start error",
    "SoundOut: source not available", "SoundIn: general error"};

/** Audio input class
 */
class SoundIn final : public AudioBase {

  std::string m_src;
  uint32_t m_mode;
  uint64_t m_dur;
  std::atomic<uint64_t> m_framecnt;
  std::atomic<bool> m_run;
  Circular m_cbuf;
  std::vector<double> m_buffer;
  AudioBase::const_iterator m_cnt;
  void *m_handle;
  std::thread thread;

  NONCOPYABLE(SoundIn);

  friend int rt_audio(const float *input, float *output,
                      unsigned long frameCount, const void *timeInfo,
                      unsigned long statusFlags, SoundIn *userData);

  friend void audio(AuLib::SoundIn &obj);

public:
  /** SoundIn constructor \n\n
      src - input source ("adc", "stdin", or file path) \n
      vframes - vector size \n
      bframes - buffer size in frames \n
  */
  SoundIn(const char *src, uint32_t nchnls = def_nchnls,
          uint32_t vframes = def_vframes, uint64_t bframes = def_bframes,
          double sr = def_sr);

  /** SoundOut destructor
   */
  ~SoundIn();

  /** Reads frames of audio to output
   */
  const double *read(uint32_t frames = def_vframes);

  /** Get the current vector stream time
   */
  double time() const { return m_framecnt / m_sr; }

  /** Get the source name
   */
  const char *src() const { return m_src.c_str(); }

  /** Get the file duration in frames
   */
  uint64_t dur() const { return m_dur; }

  /** Get error message
   */
  virtual const char *error_message() const {
    if (m_error > AULIB_ERROR)
      return soundin_error[m_error - AULIB_ERROR - 1].c_str();
    else
      return aulib_error[m_error].c_str();
  }

  /** operator () convenience method
   */
  const double *operator()(uint32_t frames = def_vframes) {
    return read(frames);
  }
};

/*! \class SoundIn SoundIn.h AuLib/SoundIn.h
 */
}

#endif
