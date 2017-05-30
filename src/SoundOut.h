////////////////////////////////////////////////////////////////////
// SoundOut class: audio output
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _SOUNDOUT_H
#define _SOUNDOUT_H

#include "AudioBase.h"
#include "Circular.h"
#include <thread>

namespace AuLib {

/** destinations
 */
enum dest_types { SOUNDOUT_RT = 1, SOUNDOUT_STDOUT, SOUNDOUT_SNDFILE };

/** Error codes
 */
enum soundout_error_codes {
  AULIB_FOPEN_ERROR = AULIB_ERROR + 1,
  AULIB_RTINIT_ERROR,
  AULIB_RTOPEN_ERROR,
  AULIB_RTSTREAM_ERROR,
  AULIB_NOIO_ERROR,
  AULIB_SOUNDIO_ERROR
};

/** Standard Error messages
 */
const std::string soundout_error[] = {"SoundOut: file open error",
                                      "SoundOut: RT initialisation error",
                                      "SoundOut: RT open error",
                                      "SoundOut: RT stream start error",
                                      "SoundOut: destination not available",
                                      "SoundOut: general error"};
  
/** Audio output class
 */
class SoundOut final : public AudioBase {

  std::string m_dest;
  uint32_t m_mode;
  std::vector<double>::iterator m_cnt;
  std::vector<double>::iterator m_pos;
  std::atomic<uint64_t> m_framecnt;
  std::atomic<bool> m_run;
  void *m_handle;
  Circular m_cbuf;
  std::thread thread;

  NONCOPYABLE(SoundOut);

  friend int rt_audio(const float *input, float *output,
                      unsigned long frameCount, const void *timeInfo,
                      unsigned long statusFlags, SoundOut *userData);

  friend void audio(SoundOut &obj);

  uint64_t write(uint32_t frames, uint32_t chn, uint32_t nchnls,
                 const double *sig);

public:
  /** SoundOut constructor \n\n
      dest - vector destination
      ("dac", "stdout", or file path) \n
      nchnls - number of channels \n
      sr - sampling rate \n
      vframes - vector size \n
  */
  SoundOut(const char *dest, uint32_t nchnls = def_nchnls,
           uint32_t vframes = def_bframes, double sr = def_sr);

  /** SoundOut destructor
   */
  ~SoundOut();

  /** Writes sig containing samples to the vector
      destination, returning the vector current
      framecount.
  */
  uint64_t write(const double *sig, uint32_t samples) {
    return write(samples, 0, 1, sig);
  }

  /** Writes the audio vector in obj to the vector
      destination, returning the vector current
      framecount.
  */
  uint64_t write(const AudioBase &obj) {
    if (obj.nchnls() == m_nchnls)
      return write(obj.vector(), (uint32_t)obj.vsamps());
    else
      m_error = AULIB_ERROR;
    return 0;
  }

  /** Writes the audio vector in obj to chn of
      destination, returning the vector current
      framecount.
  */
  uint64_t write(uint32_t chn, const AudioBase &obj) {
    if (chn < m_nchnls)
      return write((uint32_t)obj.vframes(), chn, m_nchnls, obj.vector());
    else
      m_error = AULIB_ERROR;
    return 0;
  }

  /** Get the current output stream time
   */
  double time() const { return m_framecnt / m_sr; }

  /** Get the current output stream time in frames
   */
  uint64_t timestamp() const { return m_framecnt; }

  /** Get the destination name
   */
  const char *dest() const { return m_dest.c_str(); }

  /** Get error message
   */
  virtual const char *error_message() const {
    if (m_error > AULIB_ERROR)
      return soundout_error[m_error - AULIB_ERROR - 1].c_str();
    else
      return aulib_error[m_error].c_str();
  }

  /** operator() overload convenience method
   */
  uint64_t operator()(const AudioBase &obj) { return write(obj); }

  /** operator() overload convenience method
  */
  uint64_t operator()(uint32_t chn, const AudioBase &obj) {
    return write(chn, obj);
  }
};

/*! \class SoundOut SoundOut.h AuLib/SoundOut.h
 */
}
#endif
