//
//  ViewController.m
//  AdColonyBasic
//
//  Created by Owain Moss on 1/13/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

#import "ViewController.h"

#import <AdColony/AdColony.h>

@implementation ViewController

#pragma mark - UIViewController Overrides

- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

- (BOOL)shouldAutorotate {
    return YES;
}

#pragma mark - Video Playback

- (IBAction)triggerVideo {
    [AdColony playVideoAdForZone:@"vz908f23a0cc6a49e6b1" withDelegate:nil];
}
@end
