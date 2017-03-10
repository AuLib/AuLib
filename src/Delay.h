////////////////////////////////////////////////////////////////////////////////
// Delay class: Delay line
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _DELAY_H_
#define _DELAY_H_
#include "AudioBase.h"

namespace AuLib {

/** Fixed or variable delay line with optional feedback
    (Delay, Comb filter, Flanger)
*/
class Delay : public AudioBase {

  virtual const double *dsp(const double *sig, double dt);
  virtual const double *dsp(const double *sig, const double *dt);
  virtual const double *dsp(const double *sig);

protected:
  double m_fdb;
  uint64_t m_pos;
  AudioBase m_delay;

public:
  /** Delay constructor \n\n
      dtime - delay time \n
      vframes - vector size \n
      sr - sampling rate
  */
  Delay(double dtime, double fdb, uint32_t vframes = def_vframes,
        double sr = def_sr)
      : AudioBase(1, vframes, sr), m_fdb(fdb), m_pos(0), m_delay(1, 1, sr) {
    m_delay.resize_exact(std::floor(dtime >= 0. ? dtime * sr : 1));
  };

  /** delay a signal sig for a fixed time
   */
  const double *process(const double *sig) { return dsp(sig); }

  /** delay a signal for dt seconds
   */
  const double *process(const double *sig, double dt) { return dsp(sig, dt); }

  /** delay a signal for dt seconds and with optional feedback fdb
   */
  const double *process(const double *sig, double dt, double fdb) {
    m_fdb = fdb;
    return dsp(sig, dt);
    ;
  }

  /** delay a signal for delay time taken from the signal dt
  */
  const double *process(const double *sig, const double *dt) {
    return dsp(sig, dt);
  }

  /** delay a signal for delay time taken from the signal dt and
      with optional feedback fdb
  */
  const double *process(const double *sig, const double *dt, double fdb) {
    m_fdb = fdb;
    return dsp(sig, dt);
  }

  /** delay a signal in obj for a fixed time
   */
  const Delay &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      process(obj.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** delay a signal in obj, optionally for dt seconds.
  */
  const Delay &process(const AudioBase &obj, double dt) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls) {
      if (dt < 0)
        process(obj.vector());
      else
        process(obj.vector(), dt);
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** delay a signal in obj, optionally for dt seconds
      and with feedback fdb.
  */
  const Delay &process(const AudioBase &obj, double dt, double fdb) {
    m_fdb = fdb;
    return process(obj, dt);
  }

  /** delay a signal in obj for dt sec with variable delaytime sig
  */
  const Delay &process(const AudioBase &obj, const AudioBase &dt) {
    if (obj.vframes() == m_vframes && obj.nchnls() == m_nchnls &&
        dt.vframes() == m_vframes && dt.nchnls() == m_nchnls) {
      process(obj.vector(), dt.vector());
    } else
      m_error = AULIB_ERROR;
    return *this;
  }

  /** delay a signal in obj for dt sec with variable delaytime sig
      and with optional feedback fdb.
  */
  const Delay &process(const AudioBase &obj, const AudioBase &dt, double fdb) {
    m_fdb = fdb;
    return process(obj, dt);
  }

  /** operator(a,b,c) convenience method
   */
  const Delay &operator()(const AudioBase &a, const AudioBase &b, double c) {
    return process(a, b, c);
  }

  /** operator(a,b) convenience method
   */
  const Delay &operator()(const AudioBase &a, const AudioBase &b) {
    return process(a, b);
  }

  /** operator(a,b,c) convenience method
 */
  const Delay &operator()(const AudioBase &a, const double b, double c) {
    return process(a, b, c);
  }

  /** operator(a,b) convenience method
   */
  const Delay &operator()(const AudioBase &a, double b) {
    return process(a, b);
  }

  /** operator(a) convenience method
   */
  const Delay &operator()(const AudioBase &a) { return process(a); }

  /** get the current write position
   */
  uint32_t pos() const { return m_pos; }

  /** get a reference to the delay
      line.
  */
  const AudioBase &delayline() const { return m_delay; }
};

/*! \class Delay Delay.h AuLib/Delay.h
 */
}
#endif
