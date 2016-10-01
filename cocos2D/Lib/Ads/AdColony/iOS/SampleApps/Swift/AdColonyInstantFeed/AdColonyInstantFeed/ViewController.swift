//
//  ViewController.swift
//  AdColonyInstantFeed
//
//  Created by Owain Moss on 2/11/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

import UIKit

class ViewController: UIViewController, AdColonyDelegate, AdColonyNativeAdDelegate, UITableViewDataSource, UITableViewDelegate
{
    let kAd = "ad"
    let kPost = "post"
    let kAdView = "adView"
    let kAdZone = "zone"
    let kCellType = "cellType"
    let kPostImage = "postImage"
    let kPostImageAR = "aspectRatio"
    let kFeedCellImage = "FeedCellImage"
    let kFeedCellAd = "FeedCellAd"
    let kAdZone1 = "vz7c0765ee52af4d67b9"
    
    let kAdViewCellIndex = 4
    let kAdViewWidth:CGFloat = 300.0
    let kAdViewHeight:CGFloat = 169.0
    let kAdViewCellHeight:CGFloat = 277.0
    
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var loadingLabel: UILabel!
    
    var posts = [[String : Any?]]()
    var readyAds = [[String : Any?]]()
    var finishedAds = [AdColonyNativeAdView]()
    
    var active: Bool = false
    
    //===================================================
    // MARK:- UIViewController Overrides
    //===================================================
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        //Configure AdColony once
        AdColony.configureWithAppID("app2086517932ad4b608a", zoneIDs: [kAdZone1], delegate: self, logging: true)
        
        //Hardcoded data source for our feed
        posts = [[kCellType : kPost, kPostImage : "Taco-Bell", kPostImageAR : 3.2 as CGFloat],
                 [kCellType : kPost, kPostImage : "Pacific",   kPostImageAR : 1.78 as CGFloat],
                 [kCellType : kPost, kPostImage : "MLB",       kPostImageAR : 1.45 as CGFloat],
                 [kCellType : kPost, kPostImage : "MTV",       kPostImageAR : 1.68 as CGFloat],
                 [kCellType : kPost, kPostImage : "Fallon",    kPostImageAR : 1.33 as CGFloat],
                 [kCellType : kPost, kPostImage : "Jobs",      kPostImageAR : 1.7 as CGFloat],
                 [kCellType : kPost, kPostImage : "Pugs",      kPostImageAR : 1.41 as CGFloat],
                 [kCellType : kPost, kPostImage : "Cashmore",  kPostImageAR : 1.68 as CGFloat]]
        
        //Register our nibs
        tableView.registerNib(UINib(nibName: kFeedCellImage,   bundle: nil), forCellReuseIdentifier: kFeedCellImage)
        tableView.registerNib(UINib(nibName: kFeedCellAd, bundle: nil), forCellReuseIdentifier: kFeedCellAd)
        
        //Hide the table view until we have at least one ready ad
        tableView.hidden = true
    }
    
    override func preferredStatusBarStyle() -> UIStatusBarStyle
    {
        return UIStatusBarStyle.LightContent
    }
    
    override func supportedInterfaceOrientations() -> UIInterfaceOrientationMask
    {
        if UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiom.Pad {
            return UIInterfaceOrientationMask.All
        } else {
            return UIInterfaceOrientationMask.Portrait
        }
    }
    
    override func shouldAutorotate() -> Bool
    {
        return true
    }
    
    //===================================================
    // MARK:- UITableViewDataSource
    //===================================================
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int
    {
        return posts.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell
    {
        let cellConfig = posts[indexPath.row]
        let cellType = cellConfig[kCellType] as! String
        
        //There are only two cases to consider here:
        // 1. The cell cannot be an ad
        // 2. The cell is an ad and we have an AdColonyNativeAdView
        if (cellType == kPost) {
            return self.createStandardCell(indexPath)
        } else {
            let adView = cellConfig[kAdView] as! AdColonyNativeAdView
            return self.createCellWithAdView(adView, indexPath: indexPath)
        }
    }
    
    func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat
    {
        let cellConfig = posts[indexPath.row]
        let cellType = cellConfig[kCellType] as! String
        
        //There are only two cases to consider here:
        // 1. The cell cannot be an ad
        // 2. The cell is an ad and we have an AdColonyNativeAdView
        if (cellType == kPost) {
            return self.getStandardCellHeight(cellConfig)
        } else {
            return kAdViewCellHeight
        }
    }
    
    //===================================================
    // MARK:- Feed Cell Creation
    //===================================================
    
    func createCellWithAdView(adView: AdColonyNativeAdView, indexPath: NSIndexPath) -> FeedCellAd
    {
        let adCell = tableView.dequeueReusableCellWithIdentifier(kFeedCellAd, forIndexPath: indexPath) as! FeedCellAd
        
        //Configure the cell's view elements using the properties of the AdColonyNativeAdView
        adCell.advertiserLabel.text = adView.advertiserName
        adCell.iconView.image  = adView.advertiserIcon
        adCell.titleLabel.text = adView.adTitle

        //Size the native ad view appropriately
        adView.frame = CGRectMake(0, 0, kAdViewWidth, kAdViewHeight)
        
        //Configure the native ad's engagement button
        adView.engagementButton?.backgroundColor = UIColor.blackColor()
        
        //Add the video view to the cell
        adCell.adView = adView
        
        return adCell
    }
    
    func createStandardCell(indexPath: NSIndexPath) -> FeedCellImage
    {
        //Create a standard cell with an image
        let cellConfig = posts[indexPath.row]
        let cell = tableView.dequeueReusableCellWithIdentifier(kFeedCellImage, forIndexPath: indexPath) as! FeedCellImage
        if let image = UIImage(named: cellConfig[kPostImage] as! String) {
            cell.background.image = image
        }
        
        return cell
    }
    
    func getStandardCellHeight(cellConfig: Dictionary<String, Any?>) -> CGFloat
    {
        let aspectRatio = cellConfig[kPostImageAR] as! CGFloat
        let tableWidth  = tableView.frame.size.width
        return tableWidth / aspectRatio
    }
    
    //===================================================
    // MARK:- Updating Feed and Data Source
    //===================================================
    
    func updateFeedWithAdView(adView: AdColonyNativeAdView, zoneID: String) -> Bool {
        let cellConfig = posts[kAdViewCellIndex]
        let cellType = cellConfig[kCellType] as! String
        
        //We want to insert an ad view in the 5th position of the feed if possible
        //If the cell at that position is currently an image, insert an ad view at that index
        if cellType != kAd {
            self.updateDataSourceWithAdView(adView, zoneID: zoneID, index: kAdViewCellIndex)
            return true
        }
        
        //If the current ad view is finished, replace it with the new one
        //*** NOTE: Replacing finished ads with new ones will increase publisher revenue
        if let oldAdView = cellConfig[kAdView] as? AdColonyNativeAdView {
            if let index = finishedAds.indexOf(oldAdView) {
                posts.removeAtIndex(kAdViewCellIndex)
                self.updateDataSourceWithAdView(adView, zoneID: zoneID, index: kAdViewCellIndex)
                finishedAds.removeAtIndex(index)
                return true
            }
        }
        
        return false
    }
    
    func updateDataSourceWithAdView(adView: AdColonyNativeAdView, zoneID: String, index: Int) {
        //Update our data source with the new ad view and reload the table view
        NSLog("Prepping ad view %@ for display", adView)
        posts.removeAtIndex(index)
        posts.insert([kCellType : kAd, kAdZone : zoneID, kAdView : adView], atIndex: index)
        tableView.reloadData()
    }
    
    func removeCurrentAdViewFromDataSource() {
        //Remove the old ad view from our data source
        let cellConfig = posts[kAdViewCellIndex]
        if let _ = cellConfig[kAdView] {
            posts.removeAtIndex(kAdViewCellIndex)
        }
    }
    
    //===================================================
    // MARK:- UITableViewDelegate
    //===================================================
    
    func tableView(tableView: UITableView, willDisplayCell cell: UITableViewCell, forRowAtIndexPath indexPath: NSIndexPath)
    {
        guard let adCell = cell as? FeedCellAd else { return }
        
        //Resume the ad if our table view is going to display it
        NSLog("Resuming ad view: %@", adCell.adView!)
        adCell.adView?.resume()
    }
    
    func tableView(tableView: UITableView, didEndDisplayingCell cell: UITableViewCell, forRowAtIndexPath indexPath: NSIndexPath)
    {
        guard let adCell = cell as? FeedCellAd else { return }
        
        //Pause the ad if our table view is going to display it
        NSLog("Pausing ad view: %@", adCell.adView!)
        adCell.adView?.pause()
    }
    
    //===================================================
    // MARK:- AdColonyDelegate
    //===================================================
    
    func onAdColonyAdAvailabilityChange(available: Bool, inZone zoneID: String)
    {
        guard available else { return }
        
        //The returned ad view can be nil in some cases so we need to check that
        //we have a valid ad view before we update our data source and table view
        guard let adView = AdColony.getNativeAdForZone(zoneID, presentingViewController: self) else {
            NSLog("AdColony returned an invalid ad view for zone: %@", zoneID)
            return
        }
        
        //Set ourself as the delegate of the valid ad view
        NSLog("AdColony returned a valid ad view: %@ for zone: %@.", adView, zoneID)
        adView.delegate = self
        
        //Try to insert the new ad view into our feed
        //If it is already full, queue the new ad for later use
        if self.updateFeedWithAdView(adView, zoneID: zoneID) == false {
            NSLog("Feed is full of ads right now. Queuing ad: %@ for later use", adView)
            readyAds.append([kAdView : adView, kAdZone : zoneID])
        }
        
        //Unhide the table view if this callback indicates at least one ad is ready
        if (available && !active) {
            spinner.stopAnimating()
            loadingLabel.hidden = true
            tableView.hidden = false
            active = true
        }
    }
    
    //===================================================
    // MARK:- AdColonyNativeAdDelegate
    //===================================================
    
    func onAdColonyNativeAdStarted(ad: AdColonyNativeAdView) {
        NSLog("AdColonyNativeAdView: %@ has started", ad)
    }
    
    func onAdColonyNativeAdFinished(ad: AdColonyNativeAdView, expanded: Bool) {
        NSLog("AdColonyNativeAdView: %@ has finished", ad)
        
        //Mark the ad view as complete
        finishedAds.append(ad)
        
        //Try to get an ad that's ready to be viewed and then try to insert it into our feed
        //*** NOTE: Replacing finished ads with new ones will increase publisher revenue
        if let newAdConfig = readyAds.first {
            self.removeCurrentAdViewFromDataSource() //Remove the ad that finished
            
            //If we successfully update our feed with the new ad view, remove it from our queue of waiting ads
            let newAdView = newAdConfig[kAdView] as! AdColonyNativeAdView
            let zoneID = newAdConfig[kAdZone] as! String
            if self.updateFeedWithAdView(newAdView, zoneID: zoneID) {
                readyAds.removeFirst()
            }
        }
    }
}
