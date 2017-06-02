//
//  ViewController.m
//  midisynth
//
//  Created by Victor Lazzarini on 31/05/2017.
//  Copyright © 2017 Victor Lazzarini. All rights reserved.
//

#import "ViewController.h"

extern int midi_synth(const char*, int, int, int, int, int, int);
extern void stop_synth();
extern int midi_num_devs();
extern const char *midi_dev(int);
extern double get_level();
@implementation ViewController

- (void) update_level {
    double level = 20*log10(fabs(get_level())+0.00000001);
    [self.level setDoubleValue:level];
    [self.level2 setDoubleValue:level];
}

- (void) play {
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString *resource = [[mainBundle resourcePath] stringByAppendingString:@"/church.wav"];
    midi_synth([resource UTF8String], self.dev, self.attn, self.decn, self.susn, self.reln, self.revn);
}

- (void) devs_list {
    [self.mididev removeAllItems];
    int n = midi_num_devs();
    for(int i = 0; i < n; i++){
        [self.mididev addItemWithTitle:[NSString stringWithUTF8String:midi_dev(i)]];
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.attn = 71;
    self.decn = 74;
    self.reln = 7;
    self.susn =  84;
    self.revn = 91;
    self.dev = 0;
    
    NSMutableArray *mmenu = [NSMutableArray arrayWithObjects:@"1",@"2", nil];
    for(int i =0; i < 128; i++)
        [mmenu addObject:[NSString stringWithFormat:@"%d",i]];
    [self.AttNum removeAllItems];
    [self.AttNum addItemsWithTitles:mmenu];
    
    [self.DecNum removeAllItems];
    [self.DecNum addItemsWithTitles:mmenu];
    
    [self.RelNum removeAllItems];
    [self.RelNum addItemsWithTitles:mmenu];
    
    [self.SusNum removeAllItems];
    [self.SusNum addItemsWithTitles:mmenu];
    
    [self.RevNum removeAllItems];
    [self.RevNum addItemsWithTitles:mmenu];
    
    [self.AttNum selectItemAtIndex:self.attn];
    [self.DecNum selectItemAtIndex:self.decn];
    [self.SusNum selectItemAtIndex:self.susn];
    [self.RelNum selectItemAtIndex:self.reln];
    [self.RevNum selectItemAtIndex:self.revn];
    
    [self devs_list];
        // Do any additional setup after loading the view.
    [self.level setDoubleValue:-120.0];
    [self.level2 setDoubleValue:-120.0];
    self.running = false;
    self.sbutton.title = @"start";
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}


- (IBAction)MidiDevVal:(id)sender {
    self.dev = (int)[sender indexOfSelectedItem];
}

- (IBAction)AttNumValue:(id)sender {
    self.attn = (int)[sender indexOfSelectedItem];
}
- (IBAction)DecVal:(id)sender {
    self.decn = (int)[sender indexOfSelectedItem];
}
- (IBAction)SusVal:(id)sender {
    self.susn = (int)[sender indexOfSelectedItem];
}
- (IBAction)RelVal:(id)sender {
    self.reln = (int)[sender indexOfSelectedItem];
}
- (IBAction)RevVal:(id)sender {
    self.revn = (int)[sender indexOfSelectedItem];
}

- (IBAction)handleClick:(id)sender {
    if (self.running == false) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [self play];
        });
        self.running = true;
        self.timer = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self
                      selector:@selector(update_level) userInfo:nil repeats:YES];
        self.sbutton.title = @"stop";
        [self.mididev setEnabled:false];
        [self.AttNum setEnabled:false];
        [self.DecNum setEnabled:false];
        [self.SusNum setEnabled:false];
        [self.RelNum setEnabled:false];
        [self.RevNum setEnabled:false];
    }
    else {
        stop_synth();
        self.running = false;
        [self.timer invalidate];
        self.timer = nil;
        [self.level setDoubleValue:-120.0];
        [self.level2 setDoubleValue:-120.0];
        self.sbutton.title = @"start";
        [self.mididev setEnabled:true];
        [self.AttNum setEnabled:true];
        [self.DecNum setEnabled:true];
        [self.SusNum setEnabled:true];
        [self.RelNum setEnabled:true];
        [self.RevNum setEnabled:true];
    }
}
@end
