//
//  ViewController.m
//  AdColonyInstantFeed
//
//  Created by Owain Moss on 2/4/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

#import "ViewController.h"

#import "FeedCellImage.h"
#import "FeedCellAd.h"

#define kAd            @"ad"
#define kPost          @"post"
#define kAdView        @"adView"
#define kAdZone        @"adZone"
#define kCellType      @"cellType"
#define kPostImage     @"postImage"
#define kPostImageAR   @"aspectRatio"
#define kFeedCellImage @"FeedCellImage"
#define kFeedCellAd    @"FeedCellAd"
#define kAdZone1       @"vz5c96c5bbab0546d783"

#define kAdViewCellIndex  4
#define kAdViewCellHeight 277.0f

@interface ViewController ()
{
    BOOL _active;
    
    NSMutableArray *_posts;
    NSMutableArray *_readyAds;
    NSMutableArray *_finishedAds;
}
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *spinner;
@property (weak, nonatomic) IBOutlet UILabel *loadingLabel;
@end


@implementation ViewController

#pragma mark - UIViewController Overrides

- (void)viewDidLoad {
    [super viewDidLoad];
    
    //Configure AdColony once
    [AdColony configureWithAppID:@"appe5d1ddee83f34954af" zoneIDs:@[kAdZone1] delegate:self logging:YES];
    
    //Hardcoded data source for our feed
    _posts = [@[@{kCellType : kPost, kPostImage : @"Taco-Bell", kPostImageAR : @3.2f},
                @{kCellType : kPost, kPostImage : @"Pacific",   kPostImageAR : @1.78f},
                @{kCellType : kPost, kPostImage : @"MLB",       kPostImageAR : @1.45f},
                @{kCellType : kPost, kPostImage : @"MTV",       kPostImageAR : @1.68f},
                @{kCellType : kPost, kPostImage : @"Fallon",    kPostImageAR : @1.33f},
                @{kCellType : kPost, kPostImage : @"Cashmore",  kPostImageAR : @1.68f},
                @{kCellType : kPost, kPostImage : @"Pugs",      kPostImageAR : @1.41f},
                @{kCellType : kPost, kPostImage : @"Jobs",      kPostImageAR : @1.78f}] mutableCopy];
    
    //Cached ad source
    _readyAds = [@[] mutableCopy];
    
    //List of finished ads that can be replaced with new ones
    _finishedAds = [@[] mutableCopy];
    
    //Register our nibs
    [_tableView registerNib:[UINib nibWithNibName:kFeedCellImage bundle:nil] forCellReuseIdentifier:kFeedCellImage];
    [_tableView registerNib:[UINib nibWithNibName:kFeedCellAd bundle:nil] forCellReuseIdentifier:kFeedCellAd];
    
    //Hide the table view until we have at least one ready ad
    _tableView.hidden = YES;
}

- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return UIInterfaceOrientationMaskAll;
    } else {
        return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
    }
}

- (BOOL)shouldAutorotate {
    return YES;
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [_posts count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSDictionary *cellConfig = _posts[indexPath.row];
    NSString *cellType = cellConfig[kCellType];
    
    //There are only two cases to consider here:
    // 1. The cell cannot be an ad
    // 2. The cell is an ad and we have an AdColonyNativeAdView
    if ([cellType isEqualToString:kPost]) {
        return [self createStandardCell:indexPath]; //The cell is just a regular post, so return a standard FeedCell
    } else {
        AdColonyNativeAdView *adView = (AdColonyNativeAdView *)cellConfig[kAdView]; //The cell is an ad, and we have an ad view already
        return [self createCellWithAdView:adView indexPath:indexPath]; //Return a FeedCellAd object
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSDictionary *cellConfig = _posts[indexPath.row];
    NSString *cellType = cellConfig[kCellType];
    
    //There are only two cases to consider here:
    // 1. The cell cannot be an ad
    // 2. The cell is an ad and we have an AdColonyNativeAdView
    if ([cellType isEqualToString:kPost]) {
        return [self getImageCellHeight:cellConfig]; //The cell is just a regular post, so return the height for a standard FeedCell
    } else {
        return kAdViewCellHeight; //The cell should be an ad view, so return the height for a FeedCellAd object
    }
}

#pragma mark - Feed Cell Creation

- (FeedCellAd *)createCellWithAdView:(AdColonyNativeAdView *)adView indexPath:(NSIndexPath *)indexPath {
    FeedCellAd *adCell = [_tableView dequeueReusableCellWithIdentifier:kFeedCellAd forIndexPath:indexPath];
    
    //Configure the cell's view elements using the properties of the AdColonyNativeAdView
    adCell.advertiserLabel.text = adView.advertiserName;
    adCell.iconView.image  = adView.advertiserIcon;
    adCell.titleLabel.text = adView.adTitle;
    
    //Size the native ad view appropriately
    CGFloat videoWidth  = adCell.adContainer.frame.size.width;
    CGFloat videoHeight = adCell.adContainer.frame.size.height;
    [adView setFrame:CGRectMake(0, 0, videoWidth, videoHeight)];
    
    //Configure the native ad's engagement button
    adView.engagementButton.backgroundColor = [UIColor blackColor];
    
    //Add the video view to the cell
    adCell.adView = adView;
    
    return adCell;
}

- (FeedCellImage *)createStandardCell:(NSIndexPath *)indexPath {
    //Create a standard cell with an image
    NSDictionary *cellConfig = _posts[indexPath.row];
    FeedCellImage *cell = [_tableView dequeueReusableCellWithIdentifier:kFeedCellImage forIndexPath:indexPath];
    cell.background.image  = [UIImage imageNamed:cellConfig[kPostImage]];
    
    return cell;
}

- (CGFloat)getImageCellHeight:(NSDictionary *)cellConfig {
    CGFloat aspectRatio = [cellConfig[kPostImageAR] floatValue];
    CGFloat tableWidth  = _tableView.frame.size.width;
    return tableWidth / aspectRatio;
}

#pragma mark - Updating Feed and Data Source

- (BOOL)updateFeedWithAdView:(AdColonyNativeAdView *)adView forZone:(NSString *)zoneID {
    NSDictionary *cellConfig = _posts[kAdViewCellIndex];
    NSString *cellType = cellConfig[kCellType];
    
    //We want to insert an ad view in the 5th position of the feed if possible
    //If the cell at that position is currently an image, insert an ad view at that index
    if (![cellType isEqualToString:kAd]) {
        [self updateDataSourceWithAdView:adView forZone:zoneID atIndex:kAdViewCellIndex];
        return YES;
    }
    
    //If the current ad view is finished, replace it with the new one
    //*** NOTE: Replacing finished ads with new ones will increase publisher revenue
    AdColonyNativeAdView *currentAd = cellConfig[kAdView];
    if (currentAd && [_finishedAds containsObject:currentAd]) {
        [_posts removeObjectAtIndex:kAdViewCellIndex];
        [self updateDataSourceWithAdView:adView forZone:zoneID atIndex:kAdViewCellIndex];
        [_finishedAds removeObject:currentAd];
        return YES;
    }
    
    return NO;
}

- (void)updateDataSourceWithAdView:(AdColonyNativeAdView *)adView forZone:(NSString*)zoneID atIndex:(NSUInteger)index {
    //Update our data source with the new ad view and reload the table view
    NSLog(@"Prepping ad view %@ for display", adView);
    [_posts insertObject:@{kCellType : kAd, kAdZone : zoneID, kAdView : adView} atIndex:index];
    [_tableView reloadData];
}

- (void)removeCurrentAdViewFromDataSource {
    //Remove the old ad view from our data source
    NSDictionary *cellConfig = _posts[kAdViewCellIndex];
    AdColonyNativeAdView *currentAd = cellConfig[kAdView];
    if (currentAd) {
        [_posts removeObjectAtIndex:kAdViewCellIndex];
    }
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    //Resume the ad if our table view is going to display it
    if ([cell isKindOfClass:[FeedCellAd class]]) {
        FeedCellAd* adCell = (FeedCellAd*)cell;
        NSLog(@"Resuming ad view: %@", adCell.adView);
        [adCell resume];
    }
}

- (void)tableView:(UITableView *)tableView didEndDisplayingCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    //Pause the ad if our table view has finished displaying it
    if ([cell isKindOfClass:[FeedCellAd class]]) {
        FeedCellAd* adCell = (FeedCellAd*)cell;
        NSLog(@"Pausing ad view: %@", adCell.adView);
        [adCell pause];
    }
}

#pragma mark - AdColonyDelegate

- (void)onAdColonyAdAvailabilityChange:(BOOL)available inZone:(NSString *)zoneID {
    if (!available) return;
    
    //The returned ad view can be nil in some cases so we need to check that
    //we have a valid ad view before we update our data source and table view
    AdColonyNativeAdView *adView = [AdColony getNativeAdForZone:zoneID presentingViewController:self];
    if (!adView) {
        NSLog(@"AdColony returned an invalid ad view for zone: %@", zoneID);
        return;
    } else {
        NSLog(@"AdColony returned a valid ad view: %@ for zone: %@.", adView, zoneID);
        adView.delegate = self;
    }
    
    //Try to insert the new ad view into our feed
    //If it is already full, queue the new ad for later use
    if (![self updateFeedWithAdView:adView forZone:zoneID]) {
        NSLog(@"Feed is full of ads right now. Queuing ad: %@ for later use", adView);
        [_readyAds addObject:@{kAdView : adView, kAdZone : zoneID}]; //Queue the new ad for display
    }
    
    //Unhide the table view if this callback indicates at least one ad is ready
    if (!_active) {
        [_spinner stopAnimating];
        _loadingLabel.hidden = YES;
        _tableView.hidden = NO;
        _active = YES;
    }
}

#pragma mark - AdColonyNativeAdDelegate

- (void)onAdColonyNativeAdStarted:(AdColonyNativeAdView *)ad {
    NSLog(@"AdColonyNativeAdView: %@ has started", ad);
}

- (void)onAdColonyNativeAdFinished:(AdColonyNativeAdView *)ad expanded:(BOOL)expanded {
    NSLog(@"AdColonyNativeAdView: %@ has finished", ad);
    
    //Mark the ad view as finished
    [_finishedAds addObject:ad];
    
    //If we have no more ads ready, just leave the finished one in the feed
    if ([_readyAds count] == 0) return;
    
    //Remove the ad that finished
    [self removeCurrentAdViewFromDataSource];
    
    //Try to get an ad that's ready to be viewed and then try to insert it into our feed
    //*** NOTE: Replacing finished ads with new ones will increase publisher revenue
    NSDictionary *newAdConfig = [_readyAds firstObject];
    AdColonyNativeAdView *newAdView = newAdConfig[kAdView];
    NSString *zoneID = newAdConfig[kAdZone];
    
    //If we successfully update our feed with the new ad, remove it from our queue of waiting ad views
    if ([self updateFeedWithAdView:newAdView forZone:zoneID]) {
        [_readyAds removeObjectAtIndex:0];
    }
}
@end
