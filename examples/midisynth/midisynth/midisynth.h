//
//  midisynth.h
//  midisynth
//
//  Created by Victor Lazzarini on 04/06/2017.
//  Copyright © 2017 Victor Lazzarini. All rights reserved.
//

#ifndef midisynth_h
#define midisynth_h

#ifdef __cplusplus
extern "C" {
#endif

int midi_synth(const char*, int, int, int, int, int, int);
void stop_synth();
int midi_num_devs();
const char *midi_dev(int);
double get_level();

#ifdef __cplusplus
}
#endif
#endif /* midisynth_h */

