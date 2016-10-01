//
//  ViewController.swift
//  AdColonyV4VC
//
//  Created by Owain Moss on 1/15/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

import UIKit

class ViewController: UIViewController, AdColonyAdDelegate
{
    @IBOutlet weak var background:    UIImageView!
    @IBOutlet weak var button:        UIButton!
    @IBOutlet weak var text:          UIImageView!
    @IBOutlet weak var currencyLabel: UILabel!
    @IBOutlet weak var statusLabel:   UILabel!
    @IBOutlet weak var spinner:       UIActivityIndicatorView!
    
    //=============================================
    // MARK:- UIViewController Overrides
    //=============================================
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        if UIDevice.currentDevice().userInterfaceIdiom == .Pad {
            self.spinner.activityIndicatorViewStyle = UIActivityIndicatorViewStyle.WhiteLarge
        } else {
            self.spinner.activityIndicatorViewStyle = UIActivityIndicatorViewStyle.White
        }
        
        self.updateCurrencyBalance()
        
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "addObservers", name: UIApplicationWillEnterForegroundNotification, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "removeObservers", name: UIApplicationDidEnterBackgroundNotification, object: nil)
        self.addObservers()
    }
    
    override func preferredStatusBarStyle() -> UIStatusBarStyle
    {
        return UIStatusBarStyle.LightContent
    }
    
    override func supportedInterfaceOrientations() -> UIInterfaceOrientationMask
    {
        return UIInterfaceOrientationMask.All
    }
    
    override func shouldAutorotate() -> Bool
    {
        return true
    }
    
    //=============================================
    // MARK:- UI Updates
    //=============================================
    
    func addObservers()
    {
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "updateCurrencyBalance", name: Constants.currencyBalanceChange, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "zoneReady", name: Constants.zoneReady, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "zoneOff", name: Constants.zoneOff, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "zoneLoading", name: Constants.zoneLoading, object: nil)
    }
    
    func removeObservers()
    {
        NSNotificationCenter.defaultCenter().removeObserver(self, name: Constants.zoneLoading, object: nil)
        NSNotificationCenter.defaultCenter().removeObserver(self, name: Constants.zoneOff, object: nil)
        NSNotificationCenter.defaultCenter().removeObserver(self, name: Constants.zoneReady, object: nil)
        NSNotificationCenter.defaultCenter().removeObserver(self, name: Constants.currencyBalanceChange, object: nil)
    }
    
    func zoneReady()
    {
        self.spinner.stopAnimating()
        self.spinner.hidden = true
        self.statusLabel.alpha = 0;
        UIView.animateWithDuration(1.0, animations: {
            self.button.alpha  = 1
        })
    }
    
    func zoneOff()
    {
        self.spinner.stopAnimating()
        self.button.alpha = 0;
        self.statusLabel.text = "V4VC disabled!"
    }
    
    func zoneLoading()
    {
        self.spinner.hidden = false
        self.spinner.startAnimating()
        self.button.alpha = 0
        UIView.animateWithDuration(0.5, animations: {
            self.statusLabel.alpha = 1.0;
        })
    }
    
    func updateCurrencyBalance()
    {
        //Get currency balance from persistent storage and display it
        if let wrappedBalance = NSUserDefaults.standardUserDefaults().objectForKey(Constants.currencyBalance) as! NSNumber? {
            self.currencyLabel.text = wrappedBalance.stringValue
        } else {
            self.currencyLabel.text = "0"
        }
    }
   
    //=============================================
    // MARK:- AdColony
    //=============================================    
    
    @IBAction func triggerVideo(sender: AnyObject)
    {
        AdColony.playVideoAdForZone(Constants.adcolonyZoneID, withDelegate: self, withV4VCPrePopup: true, andV4VCPostPopup: true)
    }
    
    func onAdColonyAdAttemptFinished(shown: Bool, inZone zoneID: String)
    {
        if !shown && AdColony.zoneStatusForZone(Constants.adcolonyZoneID) != ADCOLONY_ZONE_STATUS.ACTIVE {
            self.zoneLoading()
        } else if !shown {
            self.zoneReady()
        }
    }
}
