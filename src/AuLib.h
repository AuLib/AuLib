////////////////////////////////////////////////////////////////////
// AuLib.h: Library-wide definitions
// Copyright (C) 2016-7 V Lazzarini
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
/////////////////////////////////////////////////////////////////////
#ifndef _AU_LIB_H
#define _AU_LIB_H

/*! \mainpage
 *
 *  AuLib is a simple lightweight C++ (ISO C++11) class library 
 *  for audio DSP.
 *
 *  <a href="https://github.com/vlazzarini/AuLib">
 *          github.com/vlazzarini/AuLib</a> 
 *
 * \section build_the_package Building
 *
 * The library and example programs are built with CMake. The only
 * dependencies are libsndfile and portaudio and they are only required
 * to build the example programs that use the audio IO classes.
 * The code can be used within any audio processing context and it can
 * be incorporated in plugins and other programs with their own IO.
 *
 * \code
 * 
 * $ mkdir build
 * $ cd build
 * $ cmake .. 
 * $ make
 * $ make install
 * 
 * \endcode
 *
 * \section using_the_library Using
 *
 * A number of minimal examples are supplied to demonstrate how the
 * library can be used. However, these are only very simple programs
 * that only hint at the possible applications. Generally speaking,
 * objects of the different processing classes are instantiated and 
 * then used to generate or process vectors of audio data, which are 
 * kept the library objects.
 * 
 * It is possible to access the output samples of each object directly,
 * and processing functions can take both objects (const AudioObj&) and 
 * audio vectors (const double*) as inputs (as well as other 
 * parameters, depending on the object type). A number of operations
 * can be applied to objects (such as scaling, offsetting, mixing to,
 * etc.).
 *
 * A minimal example is shown below where an echo effect is applied
 * to an arbitrary input, using SoundIn, SoundOut, Chn, Pan, SigBus 
 * and Delay objects.
 *
 * \code
 * 
 * #include <SoundOut.h>
 * #include <SoundIn.h>
 * #include <Delay.h>
 * #include <Chn.h>
 * #include <SigBus.h>
 * #include <Pan.h>
 * #include <iostream>
 * #include <vector>
 *
 * using namespace AuLib;
 * using namespace std;

 * int main(int argc, const char **argv){

 *   if(argc > 2){
    
 *     SoundIn input(argv[1]);
 *     std::vector<Chn>  chn(input.nchnls());
 *     std::vector<Delay> echo(input.nchnls(),
 * 			    Delay(0.5, 0.75,
 * 				  def_vframes,input.sr()));
 *     std::vector<Pan>   pan(input.nchnls());
 *     SigBus  mix(1./input.nchnls(),0.,false,2);
 *     SoundOut output(argv[2],2,def_vframes,input.sr());
 *    
 *     cout << Info::version();
 * 
 *    for(uint64_t i=0; i < input.dur(); i+=def_vframes){
 *       input.read();
 *       for(uint32_t j = 0; j < input.nchnls(); j++){
 * 	chn[j].process(input,j+1);
 * 	echo[j].process(chn[j]);
 * 	pan[j].process(echo[j]+=chn[j],
 * 		       (1+j)*input.nchnls()/2.);
 * 	mix.process(pan[j]);
 *       }
 *       output.write(mix);
 *       mix.clear();
 *     }
 *     
 *     return 0;
 *     
 *   } else
 *     std::cout << "usage: "
 * 	      << argv[0] << " <source> <dest>\n";
 *   return 1;
 * }
 * 
 * \endcode
 *
 * \section extending_lib Extending
 *
 * The library classes can be easily extended. There is quite a
 * bit freedom to do this, as there are not strict prescriptions
 * on the form or signature of processing methods. Usually we
 * would inherit from the AudioBase class to allow easy 
 * integration with existing objects, and to avail of basic audio 
 * processing facilities provided there. Supplying a processing
 * method that consumes a vector of samples and another that reads
 * from an AudioObj& is the minimum necessary to allow full 
 * integration. 
 *
 * The following example is a skeleton of an AudioBase-derived 
 * class that demonstrates these ideas:
 *
 * \code
 * namespace AuLib {
 *
 * class NewClass : public AudioBase {
 * 
 *   protected:
 *     double m_par;
 *   
 *   public:
 *     
 *     NewClass(double param = .5, uint32_t nchnls=def_nchnls
 * 	   uint32_t vframes = def_vframes, double sr = def_sr) :
 *         m_param(param),
 *         AudioBase(nchnls,vframes,sr) { };
 * 
 *     virtual const double* process(const double* sig);
 *    
 *     virtual const double* process(const double* sig, double par){
 *       m_par = par;
 *       return process(sig);
 *     }
 * 
 *     virtual const NewClass& process(const AudioBase& obj){
 *       if(obj.vframes() == m_vframes &&
 * 	 obj.nchnls() == m_nchnls) {
 * 	   process(obj.vector());
 *       } else m_error = AULIB_ERROR;
 *       return *this;
 *     }
 * 
 *     virtual const NewClass& process(const AudioBase& obj, 
 *                                      double par){
 *       m_par = par;
 *       process(obj);
 *       return *this;
 *     }    
 * 
 *   };
 * 
 * }
 *
 * \endcode
 *
 * \section gpl_license License
 *
 * Copyright (C) 2016-7 V Lazzarini
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <limits>

#define NONCOPYABLE(A)\
  public:\
  A(const A&) = delete;	\
  A& operator=(A) = delete \

namespace AuLib {

  namespace Info {
    /** Aulib major version */
    const uint32_t major_version = AULIB_MAJOR_V;

    /** Aulib minor version */
    const uint32_t minor_version = AULIB_MINOR_V;

    /** returns the version string */
    static const std::string version() {
      std::ostringstream stm;
      stm << "Aulib version " << major_version << '.'
	  << minor_version  << (BETA ? " beta\n" : "\n");
      return stm.str();
    }

    /** returns the copyright string */
    static const std::string copyright() {
      std::ostringstream stm;
      stm << "Aulib version " << major_version << '.' << minor_version;
      stm << "\n(c) 2016-7 V.Lazzarini, licensed by the LGPL\n";
      return stm.str();
    }    
    
  }
  
  /** default signal vectorsize.
   */
  const uint32_t def_vframes = 64;

  /** default IO buffersize.
   */
  const uint32_t def_bframes = 1024;

  /** default sampling rate
   */
  const double def_sr = 44100.;

  /** default control rate
   */
  const double def_kr = def_sr/def_vframes;

  /** default audio channels.
   */
  const uint32_t  def_nchnls = 1;

  /** default function table length
   */
  const uint32_t def_tframes = 8192;

  /** the pi definition. 
   */ 
  const double pi = 4*atan(1.); 

  /** the two pi definition. 
   */ 
  const double twopi = 8*atan(1.);

  /** the min. pos. double
   */ 
  const double db_min = std::numeric_limits<double>::min();

  /** -120dBfs
   */ 
  const double m120dBfs = 0.000001;
  

  /** General error xodes
   */
  enum error_codes {
    AULIB_NOERROR = 0,
    AULIB_ERROR
  };

  /** Standard Error messages
   */
  static const char* aulib_error[] = {
    "Aulib: no error",
    "Aulib: general object error"
  };
  
};
#endif  // ifndef _AU_LIB_H
