//
//  AppDelegate.m
//  midisynth
//
//  Created by Victor Lazzarini on 31/05/2017.
//  Copyright © 2017 Victor Lazzarini. All rights reserved.
//

#import "AppDelegate.h"
#import "midisynth.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    stop_synth();
}


@end
