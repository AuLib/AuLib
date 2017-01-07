////////////////////////////////////////////////////////////////////
// Function table reader
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _TABLEREAD_H
#define _TABLEREAD_H
#include "AudioBase.h"
#include "FourierTable.h"
#include "FuncTable.h"

namespace AuLib {

/** Table reader with truncated lookup
 */
class TableRead : public AudioBase {

protected:
  std::unique_ptr<FourierTable> m_sine;
  const double *m_table;
  double m_phs;
  bool m_norm;
  bool m_wrap;
  uint64_t m_tframes;

  /** truncated table lookup
   */
  virtual void lookup(const double *phs);

  /** modulo function
   */
  double mod(double pos) {
    if (m_wrap) {
      while (pos >= m_tframes)
        pos -= m_tframes;
      while (pos < 0)
        pos += m_tframes;
    } else {
      pos = pos < 0 ? 0. : (pos >= m_tframes ? m_tframes : pos);
    }
    return pos;
  }

public:
  /** TableRead constructor \n\n
      table - function table \n
      phase - initial phase \n
      norm - normalisation switch \n
      wrap - wraparound switch \n
      vframes - vector size \n
      sr - sampling rate
  */
  TableRead(const FuncTable &ftable, double phase = 0., bool norm = true,
            bool wrap = true, uint32_t vframes = def_vframes,
            double sr = def_sr)
      : AudioBase(1, vframes, sr), m_sine(nullptr), m_table(ftable.table()),
        m_phs(phase), m_norm(norm), m_wrap(wrap), m_tframes(ftable.tframes()) {
    if (m_table == nullptr) {
      m_vframes = 0;
      m_error = AULIB_ERROR;
    }
  };

  /** TableRead constructor \n\n
      phase - initial phase \n
      norm - normalisation switch \n
      wrap - wraparound switch \n
      vframes - vector size \n
      sr - sampling rate \n
  */
  TableRead(double phase = 0., bool norm = true, bool wrap = true,
            uint32_t vframes = def_vframes, double sr = def_sr)
      : AudioBase(1, vframes), m_sine(new FourierTable),
        m_table(m_sine->table()), m_phs(phase), m_norm(norm), m_wrap(wrap),
        m_tframes(m_sine->tframes()){};

  /** takes in a frame of phase values
      and lookups up the table values
  */
  virtual const double *process(const double *phs) {
    lookup(phs);
    return vector();
  }

  /** takes in a frame of phase values
      and lookups up the table values
  */
  virtual const TableRead &process(const AudioBase &obj) {
    if (obj.vframes() == m_vframes && obj.nchnls() == 1)
      lookup(obj.vector());
    else
      m_error = AULIB_ERROR;
    return *this;
  }
};

/*! \class TableRead TableRead.h AuLib/TableRead.h
 */
}
#endif
