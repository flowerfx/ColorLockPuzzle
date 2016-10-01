//
//  ViewController.swift
//  AdColonyBasic
//
//  Created by Owain Moss on 1/12/15.
//  Copyright (c) 2015 AdColony. All rights reserved.
//

import UIKit
import Foundation

class ViewController: UIViewController
{
    
    @IBOutlet weak var background: UIImageView!
    
    //=============================================
    // MARK:- UIViewController Overrides
    //=============================================
    
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
    // MARK:- AdColony
    //=============================================
    
    @IBAction func triggerVideo(sender: AnyObject)
    {
        AdColony.playVideoAdForZone("vzf8fb4670a60e4a139d01b5", withDelegate: nil)
    }
}
