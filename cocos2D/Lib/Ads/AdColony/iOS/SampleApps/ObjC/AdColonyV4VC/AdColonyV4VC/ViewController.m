//
//  ViewController.m
//  AdColonyV4VC
//
//  Created by Owain Moss on 1/14/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

#import "ViewController.h"

#import "Constants.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *background;
@property (weak, nonatomic) IBOutlet UIButton *button;
@property (weak, nonatomic) IBOutlet UILabel  *currencyLabel;
@property (weak, nonatomic) IBOutlet UILabel  *statusLabel;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *spinner;
@end


@implementation ViewController

#pragma mark - UIViewController overrides

- (void)viewDidLoad {
    [super viewDidLoad];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        [_spinner setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhiteLarge];
    } else {
        [_spinner setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhite];
    }
    
    [self updateCurrencyBalance];
    [self zoneLoading];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(removeObservers) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(addObservers) name:UIApplicationWillEnterForegroundNotification object:nil];
    [self addObservers];
}

- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

- (BOOL)shouldAutorotate {
    return YES;
}

#pragma mark - UI Updates

- (void)addObservers {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateCurrencyBalance) name:kCurrencyBalanceChange object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(zoneReady) name:kZoneReady object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(zoneOff) name:kZoneOff object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(zoneLoading) name:kZoneLoading object:nil];
}

- (void)removeObservers {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kZoneLoading object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kZoneOff object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kZoneReady object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kCurrencyBalanceChange object:nil];
}

- (void)zoneLoading {
    [_spinner setHidden:NO];
    [_spinner startAnimating];
    [_button  setAlpha:0.];
    [UIView animateWithDuration:.5 animations:^{
        _statusLabel.alpha = 1.;
    } completion:nil];
}

- (void)zoneReady {
    [_spinner stopAnimating];
    [_spinner setHidden:YES];
    [_statusLabel setAlpha:0.];
    [UIView animateWithDuration:1.0 animations:^{
        _button.alpha = 1.;
    } completion:nil];
}

- (void)zoneOff {
    [_spinner stopAnimating];
    [_button  setAlpha:0.];
    [_statusLabel setText:@"V4VC disabled!"];
}

- (void)updateCurrencyBalance {
    //Get currency balance from persistent storage and display it
    NSNumber* wrappedBalance = [[NSUserDefaults standardUserDefaults] objectForKey:kCurrencyBalance];
    NSUInteger balance = wrappedBalance && [wrappedBalance isKindOfClass:[NSNumber class]] ? [wrappedBalance unsignedIntValue] : 0;
    [_currencyLabel setText:[NSString stringWithFormat:@"%lu", (unsigned long)balance]];
}

#pragma mark - AdColony

- (IBAction)triggerVideo {
    [AdColony playVideoAdForZone:kAdColonyZoneID withDelegate:self withV4VCPrePopup:YES andV4VCPostPopup:YES];
}

- (void)onAdColonyAdAttemptFinished:(BOOL)shown inZone:(NSString *)zoneID {
    if (!shown && [AdColony zoneStatusForZone:kAdColonyZoneID] != ADCOLONY_ZONE_STATUS_ACTIVE) {
        [self zoneLoading];
    } else if (!shown) {
        [self zoneReady];
    }
}
@end
