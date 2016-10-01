//
//  ViewController.h
//  AdColonyInstantFeed
//
//  Created by Owain Moss on 2/4/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AdColony/AdColony.h>
#import <AdColony/AdColonyNativeAdView.h>

@interface ViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, AdColonyDelegate, AdColonyNativeAdDelegate>
@end
