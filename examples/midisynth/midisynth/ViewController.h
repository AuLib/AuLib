//
//  ViewController.h
//  midisynth
//
//  Created by Victor Lazzarini on 31/05/2017.
//  Copyright © 2017 Victor Lazzarini. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController
@property (weak) IBOutlet NSButton *sbutton;
@property (atomic) bool running;
@property (weak) IBOutlet NSPopUpButton *AttNum;
@property (weak) IBOutlet NSPopUpButton *DecNum;
@property (weak) IBOutlet NSPopUpButton *SusNum;
@property (weak) IBOutlet NSPopUpButton *RelNum;
@property (weak) IBOutlet NSPopUpButton *RevNum;
@property (weak) IBOutlet NSPopUpButton *mididev;
@property (weak) IBOutlet NSLevelIndicator *level2;
@property (weak) IBOutlet NSLevelIndicator *level;
@property (readwrite) int attn;
@property (readwrite) int decn;
@property (readwrite) int susn;
@property (readwrite) int reln;
@property (readwrite) int revn;
@property (readwrite) int dev;
@property (strong) NSTimer *timer;
@end

