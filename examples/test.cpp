#include "SoundOut.h"
#include "SampleTable.h"
#include "SamplePlayer.h"

int main(){
  SampleTable wave("/Users/victor/audio/pianoc2.wav");
  SamplePlayer sig(0.5, 1, 0., wave.table(), wave.size());
  SoundOut output("dac");
 
  for(int i=0; i < def_sr*10; i+=def_vsize){
    sig.process();
    output.write(sig.output());
  }
  return 0;
}
