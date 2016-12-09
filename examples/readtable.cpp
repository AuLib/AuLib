#include "TableRead.h"
#include "SoundOut.h"
#include "WaveTables.h"
#include "Phasor.h"


int main(){
  TriangleTable wave(50);
  Phasor   phase(440.);
  TableRead sig(wave.table());
  SoundOut output("dac");
 
  for(int i=0; i < def_sr*10; i+=def_vsize){
    phase.process();
    sig.process(phase.output());
    output.write(sig.output());
  }
  return 0;
}
