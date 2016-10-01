//
//  AppDelegate.m
//  AdColonyBasic
//
//  Created by Owain Moss on 1/13/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

#import "AppDelegate.h"

#import <AdColony/AdColony.h>

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    //Configure AdColony only once, on initial launch
    [AdColony configureWithAppID:@"appe5d1ddee83f34954af" zoneIDs:@[@"vz908f23a0cc6a49e6b1"] delegate:nil logging:YES];
    
    return YES;
}
@end
