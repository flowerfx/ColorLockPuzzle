//
//  AppDelegate.swift
//  AdColonyBasic
//
//  Created by Owain Moss on 1/12/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate
{
    var window: UIWindow?
    
    func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool
    {
        //Configure AdColony once on app launch
        AdColony.configureWithAppID("appbdee68ae27024084bb334a", zoneIDs: ["vzf8fb4670a60e4a139d01b5"], delegate: nil, logging: true)
        
        return true
    }
}
