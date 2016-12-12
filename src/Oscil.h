////////////////////////////////////////////////////////////////////
// Oscil class: truncating oscillator
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _OSCIL_H
#define _OSCIL_H

#include "AudioBase.h"
#include "FuncTable.h"

namespace AuLib {
  
  /** Truncating oscillator
   */
  class Oscil : public AudioBase {

  protected:
  
    double m_phs;  
    double m_freq;
    double m_amp;
    double m_incr;
    uint32_t m_tsize;
    double *m_sine;
    const double *m_table;
    const double *m_am;
    const double *m_fm;

    /** truncating oscillator process
     */
    virtual void oscillator();

    /** AM/FM processing
     */
    void am_fm(uint32_t ndx){
      if(m_am != NULL) m_amp = m_am[ndx];
      if(m_fm != NULL) {
	m_freq = m_fm[ndx];
	m_incr = m_freq * m_tsize/m_sr;
      }
    }
  
    /** phase modulo
     */
    void mod(){
      while(m_phs >= m_tsize) m_phs -= m_tsize;
      while(m_phs < 0) m_phs += m_tsize;
    }

  public:
    /** swap function for copy assignment 
     */
    friend void swap(Oscil& obja,
		     Oscil& objb) 
    {
      using std::swap;
      swap(obja.m_phs,objb.m_phs);
      swap(obja.m_freq,objb.m_freq);
      swap(obja.m_amp,objb.m_amp);
      swap(obja.m_incr,objb.m_incr);
      swap(obja.m_tsize,objb.m_tsize);
      swap(obja.m_sine,objb.m_sine);
      swap(obja.m_table,objb.m_table);
      swap(obja.m_am,objb.m_am);
      swap(obja.m_fm,objb.m_fm);
    }
    /** Oscil copy assignment operator 
     */
    const Oscil& operator=(Oscil obj){
      swap(*this, obj);
      return *this;
    }
    
    /** Oscil copy constructor */
    Oscil(const Oscil& obj);
    
    /** Oscil constructor \n\n
	amp - amplitude   \n
	freq - frequency in Hz \n
	phase - init phase (0-1) \n 
	vsize - vector size \n
	sr - sampling rate \n\n
        Uses internal sine wave table
    */
    Oscil(double amp = 0., double freq = 0.,
	  double phase = 0., uint32_t vsize = def_vsize,
	  double sr = def_sr);
    
    /** Oscil constructor \n\n
	amp - amplitude   \n
	freq - frequency in Hz \n
	ftable - function table \n
	phase - init phase (0-1) \n 
	vsize - vector size \n
	sr - sampling rate \n
    */
    Oscil(double amp, double freq,
	  const FuncTable& ftable, double phase = 0.,
	  uint32_t vsize = def_vsize,
	  double sr = def_sr);

    /** Oscil destructor
     */
    virtual ~Oscil();

    /** Process one vector of audio
     */
    virtual const Oscil& process() {
      oscillator();
      return *this;
    }
  
    /** Process one vector of audio
	with amplitude amp
    */
    virtual const Oscil& process(double amp) {
      m_amp = amp;
      return process();
    }

    /** Process one vector of audio
	with amplitude amp and
	frequency freq
    */
    virtual const Oscil& process(double amp, double freq){
      m_amp = amp;
      m_freq = freq;
      m_incr = m_freq*m_tsize/m_sr;
      return process();
    }

    /** Process one vector of audio
	with amplitude and (optional) freq modulation
    */
    virtual const double*
    process(const double* amp, const double* freq = NULL){
      m_am = amp;
      m_fm = freq;
      process();
      return m_output;
    }

    /** Process one vector of audio
	with amplitude amp 
	and freq modulation
    */
    virtual const double* process(double amp, const double* freq){
      m_amp = amp;
      m_fm = freq;
      process();
      return m_output;
    }

    /** Process one vector of audio
	with amplitude modulation 
	and frequency freq
    */ 
    virtual const double* process(const double* amp, double freq){
      m_am = amp;
      m_freq = freq;
      m_incr = m_freq*m_tsize/m_sr;
      process();
      return m_output;
    }

    /** Process one vector of audio
	with amplitude modulation from obja
    */ 
    virtual const Oscil& process(const AudioBase& obja){
      if(obja.vsize() == m_vsize) {
	m_am = obja.output();
        process();
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** Process one vector of audio
	with amplitude modulation from obja
	and frequency freq
    */ 
    virtual Oscil& process(const AudioBase& obja, double freq){
      m_freq = freq;
      m_incr = m_freq*m_tsize/m_sr;
      process(obja);
      return *this;
    }

    /** Process one vector of audio
	with amplitude amp 
	and freq modulation from objf
    */
    virtual Oscil& process(double amp, const AudioBase& objf){
      if(objf.vsize() == m_vsize &&
	 objf.nchnls() == 1) {
	m_amp = amp;
	m_fm = objf.output();
        process();
      } else m_error = AULIB_ERROR;
      return *this;
    }

    /** Process one vector of audio
	with amplitude from obja and freq modulation from objf
    */
    virtual Oscil& process(const AudioBase& obja, const AudioBase& objf){
      if(obja.vsize() == m_vsize &&
	 objf.vsize() == m_vsize &&
	 obja.nchnls() == 1 &&
	 objf.nchnls() == 1) {
	m_am = obja.output();
	m_fm = objf.output();
        process();
      } else m_error = AULIB_ERROR;
     return *this;
    }
 
  };

  /*! \class Oscil Oscil.h AuLib/Oscil.h
   */
}
#endif
