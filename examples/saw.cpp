#include "SoundOut.h"
#include "WaveTables.h"
#include "Oscilic.h"

int main(){
  SawTable wave(50);
  Oscilic sig(0.5, 440., 0., wave.table(), wave.size());
  SoundOut output("dac");
 
  for(int i=0; i < def_sr*10; i+=def_vsize){
    sig.process(0.5, 440.);
    output.write(sig.output());
  }
  return 0;
}
