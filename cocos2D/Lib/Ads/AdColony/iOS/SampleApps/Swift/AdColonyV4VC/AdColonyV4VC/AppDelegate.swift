//
//  AppDelegate.swift
//  AdColonyV4VC
//
//  Created by Owain Moss on 1/15/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, AdColonyDelegate
{
    var window: UIWindow?

    //=============================================
    // MARK:- App Launch
    //=============================================
    
    func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool
    {
        //Initialize AdColony only once, on initial launch
        AdColony.configureWithAppID(Constants.adcolonyAppID, zoneIDs: [Constants.adcolonyZoneID], delegate: self, logging: true)
        
        return true
    }
    
    //=============================================
    // MARK:- AdColony V4VC
    //=============================================
    
    /** 
        Callback activated when a V4VC currency reward succeeds or fails
        This implementation is designed for client-side virtual currency without a server
        It uses NSUserDefaults for persistent client-side storage of the currency balance
        For applications with a server, contact the server to retrieve an updated currency balance
        On success, posts an NSNotification so the rest of the app can update the UI
        On failure, posts an NSNotification so the rest of the app can disable V4VC UI elements
    */
    func onAdColonyV4VCReward(success: Bool, currencyName: String, currencyAmount amount: Int32, inZone zoneID: String)
    {
        NSLog("AdColony zone: %@ reward: %@ amount: %i", zoneID, success ? "YES" : "NO", amount)
        
        if success {
            let storage = NSUserDefaults.standardUserDefaults()
            
            //Get currency balance from persistent storage and update it
            var balance:UInt32
            if let wrappedBalance = storage.objectForKey(Constants.currencyBalance) as! NSNumber? {
                balance = wrappedBalance.unsignedIntValue
            } else {
                balance = 0
            }
            balance += UInt32(amount)
            
            //Persist the currency balance
            storage.setValue(NSNumber(unsignedInt: balance), forKey: Constants.currencyBalance)
            storage.synchronize()
            
            //Post a notification so the rest of the app knows the balance changed
            NSNotificationCenter.defaultCenter().postNotificationName(Constants.currencyBalanceChange, object: nil)
        }
    }
    
    //=============================================
    // MARK:- AdColony Ad Fill
    //=============================================
    
    /** 
        Callback triggered when the state of ad readiness changes
        If the AdColony SDK tells us our zone either has, or doesn't have, ads, we
        need to tell our view controller to update its UI elements appropriately
    */
    func onAdColonyAdAvailabilityChange(available: Bool, inZone zoneID: String)
    {
        if available {
            NSNotificationCenter.defaultCenter().postNotificationName(Constants.zoneReady, object: nil)
        } else {
            NSNotificationCenter.defaultCenter().postNotificationName(Constants.zoneLoading, object: nil)
        }
    }
}
