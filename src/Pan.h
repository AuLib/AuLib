/////////////////////////////////////////////////////////////////////
// Pan class: stereo panning
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _PAN_H_
#define _PAN_H_
#include "AudioBase.h"

namespace AuLib {

  /** Pan a mono input signal using 
      an equal-power algorithm
  */
  class Pan : public AudioBase {

  protected:
    double m_pos;
  
  public:
    /** Pan constructor \n\n
	pos - pan position (0 - 1) \n
	vsize - vector size \n
        sr - sampling rate
    */  
  Pan(double pos = .5,
      uint32_t vsize = def_vsize) :
    m_pos(pos),
      AudioBase(2,vsize) { };

    /** Pan a signal sig 
     */
    virtual const double* process(const double* sig);

    /** Pan a signal sig according to position pos (0-1)
     */
    virtual const double* process(const double* sig, double pos) {
      m_pos = pos;
      return process(sig);
    }

    /** Pan a signal in obj
     */
    virtual const Pan& process(const AudioBase& obj) {
      if(obj.vsize() == m_vsize &&
	 obj.nchnls() == 1) {
	process(obj.vector());
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** Pan a signal in obj according to position pos (0-1)
     */
    virtual const Pan& process(const AudioBase& obj, double pos) {
      m_pos = pos;
      process(obj);
      return *this;
    }    

  };

  /*! \class Pan Pan.h AuLib/Pan.h
   */

}
#endif
