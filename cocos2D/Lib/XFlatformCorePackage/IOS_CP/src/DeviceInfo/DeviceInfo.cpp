#include "Platform/DeviceInfo.h"
#include "iOSCorePackage_config.h"

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <stdint.h>
#include <mach-o/dyld.h>
#include <ctype.h>
#include "XKeychainItemWrapper.h"

#import <Foundation/Foundation.h>
#import <Foundation/NSString.h>
#import <Foundation/NSFileManager.h>
#import <UIKit/UIKit.h>
#import <Security/Security.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>
#import <CommonCrypto/CommonDigest.h>

#import <AdSupport/AdSupport.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <stdint.h>
#include <mach-o/dyld.h>

#include "Reachability.h"

using namespace platform;

std::string DeviceInfo::GetModel()
{	
	// Device name(iPhone, iPad. iPod touch...)
	//NSString *deviceType = [UIDevice currentDevice].model;
	
	// Device model (iPhone4,1 , iPad2,2, iPod3,1 ...)
	// A list with device models: http://stackoverflow.com/questions/18414032/how-to-identify-a-hw-machine-identifier-reliable
	size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding: NSUTF8StringEncoding];
    free(machine);
	
	return std::string([platform UTF8String]);
}

std::string DeviceInfo::GetCarrier()
{
	// Carrier name, only in iOS 4.0+
	NSString* carrierName = nil;	

	if (NSClassFromString(@"CTCarrier") != nil)
	{
		CTTelephonyNetworkInfo *networkInfo = [[CTTelephonyNetworkInfo alloc] init];
		
		if (networkInfo != nil)
		{
			CTCarrier *carrier = [networkInfo subscriberCellularProvider];
			if (carrier != nil)
			{
				NSString* MCCstring = [carrier mobileCountryCode];
				NSString* MNCstring = [carrier mobileNetworkCode];                        
				if ((MCCstring != nil) && (MNCstring != nil))
					carrierName = [NSString stringWithFormat:@"%@%@", MCCstring, MNCstring];   //attempting to retrieve the carrier name
			}
		}
		
		//[networkInfo release];
	}

	if ((carrierName == nil) || ([carrierName length] == 0))
		carrierName = @"N/A";
		
	return std::string([carrierName UTF8String]);
}

std::string DeviceInfo::GetCountryCode()
{
	NSString *countryCode = nil;

	if (NSClassFromString(@"CTCarrier") != nil)
	{
		CTTelephonyNetworkInfo *networkInfo = [[CTTelephonyNetworkInfo alloc] init];

		if (networkInfo != nil)
		{
			CTCarrier *carrier = [networkInfo subscriberCellularProvider];
			if (carrier != nil)
				countryCode = [carrier isoCountryCode];//attempting to retrieve the country in ISO 3166-1 alpha 2 format from the SIM
		}

		//[networkInfo release];
	}

	if ((countryCode == nil) || ([countryCode length] != 2) || ([countryCode rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"0123456789"]].location != NSNotFound))	
		countryCode = [[NSLocale currentLocale] objectForKey: NSLocaleCountryCode];

	if ((countryCode == nil) || ([countryCode length] != 2) || ([countryCode rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"0123456789"]].location != NSNotFound))
		countryCode = @"XX";

	std::string result = [countryCode UTF8String];
	for (auto &c: result) 
		c = toupper(c);	
	
	return result;	
}

std::string DeviceInfo::GetLanguage()
{
	// Getting the language
	NSString *language  = [[NSLocale preferredLanguages] objectAtIndex:0];
	if ((language == nil) || ([language length] == 0))
		language = @"en";
	
	return std::string([language UTF8String]);
}

DeviceInfo::ConnectivityType DeviceInfo::GetConnectivityType()
{
	// Set
	static Reachability* internetReachability = [Reachability reachabilityForInternetConnection];
	NetworkStatus networkStatus = [internetReachability currentReachabilityStatus];
	
	// Check NetworkStatus
	switch(networkStatus)
	{
		case NotReachable:
			return ConnectivityType::_NO_CONNECTIVITY;
		case ReachableViaWiFi:
			return ConnectivityType::_WIFI_CONNECTIVITY;
		case ReachableViaWWAN:
			// Cellular (data) internet connection
			// Check, we can distinguish this subtype only in iOS7+
			if(NSFoundationVersionNumber > NSFoundationVersionNumber_iOS_6_1)
			{
				// Set
				CTTelephonyNetworkInfo* telephonynNetworkInfo =[CTTelephonyNetworkInfo new];
				NSString* radioAccessTechnology = telephonynNetworkInfo.currentRadioAccessTechnology;
				
				// Check cellular internet connection type
				if([radioAccessTechnology isEqualToString : CTRadioAccessTechnologyLTE])
				{
					return ConnectivityType::_4G_CONNECTIVITY;
				}
				else if([radioAccessTechnology isEqualToString : CTRadioAccessTechnologyCDMA1x]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyCDMAEVDORev0]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyCDMAEVDORevA]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyCDMAEVDORevB]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyeHRPD]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyHSDPA]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyHSUPA]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyWCDMA])
				{
					return ConnectivityType::_3G_CONNECTIVITY;
				}
				else if([radioAccessTechnology isEqualToString : CTRadioAccessTechnologyEdge]
						|| [radioAccessTechnology isEqualToString : CTRadioAccessTechnologyGPRS])
				{
					return ConnectivityType::_2G_CONNECTIVITY;
				}
			}
			else
			{
				// Return for iOS6
				return ConnectivityType::_MOBILE_CONNECTIVITY;
			}
	}
	// Return unknown connectivity (not in the above cases)
	return ConnectivityType::_UNKNOWN_CONNECTIVITY;
	
}

DeviceInfo::ScreenRes DeviceInfo::GetScreenRes()
{
    UIScreen* appScreen     = [UIScreen mainScreen];
	CGRect  appScreenBounds = [appScreen bounds];
	CGSize  appScreenSize   = appScreenBounds.size;
	CGPoint appScreenOrigin = appScreenBounds.origin;
    CGFloat appScreenScale  = 1.0f;
	UIInterfaceOrientation _statusBarOrientation = [[UIApplication sharedApplication] statusBarOrientation];
	
	BOOL appIsPortrait = UIDeviceOrientationIsPortrait((UIDeviceOrientation)_statusBarOrientation);
	if (appIsPortrait && appScreenSize.height < appScreenSize.width)
	{
		//apply correction
		appScreenBounds = CGRectMake(appScreenOrigin.y,
									 appScreenOrigin.x,
									 appScreenSize.height,
									 appScreenSize.width);
	}
	else if ((!appIsPortrait) && appScreenSize.height > appScreenSize.width)
	{
		//apply correction
		appScreenBounds = CGRectMake(appScreenOrigin.y,
									 appScreenOrigin.x,
									 appScreenSize.height,
									 appScreenSize.width);
	}
    //use nativeScale for iphone6 and later. (TODO: #define IPHONE5SCREENSIZE = 568)
    BOOL IsIPhone6orLater = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && MAX(appScreenSize.height, appScreenSize.width) > 568;
    appScreenScale = IsIPhone6orLater ? appScreen.nativeScale : appScreen.scale;
    
	DeviceInfo::ScreenRes sres;
	sres.w = appScreenBounds.size.width * appScreenScale;
	sres.h = appScreenBounds.size.height * appScreenScale;
	return sres;
}

//static NSString* hashStringWithMD5(NSString* i_string)
//{
//	if (i_string == nil || [i_string length] == 0)
//		return nil;

//	const char *value = [i_string UTF8String];

//	unsigned char outputBuffer[CC_MD5_DIGEST_LENGTH];
//	CC_MD5(value, (CC_LONG)strlen(value), outputBuffer);

//	NSMutableString *outputString = [[NSMutableString alloc] initWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
//	for (NSInteger count = 0; count < CC_MD5_DIGEST_LENGTH; count++)
//		[outputString appendFormat:@"%02x",outputBuffer[count]];

//	return [outputString autorelease];
//}

static NSString* GetEtherHwAddress_()
{
	
	int                 mib[6];
	size_t              len;
	char                *buf;
	unsigned char       *ptr;
	struct if_msghdr    *ifm;
	struct sockaddr_dl  *sdl;

	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;

	if ((mib[5] = if_nametoindex("en0")) == 0)
		return NULL;

	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
		return NULL;

	if ((buf = (char*)malloc(len)) == NULL)
		return NULL;

	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) 
	{	
		free(buf);
		return NULL;
	}

	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X", 
		*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	free(buf);

	outstring = [outstring uppercaseString];
	return outstring;
}

std::string DeviceInfo::GetEtherHwAddress()
{
	return std::string([GetEtherHwAddress_() UTF8String]);
}

//	std::string DeviceInfo::GetGLDID()
//	{
//        
//		// If we don't have UDID, let's use a hash of the MAC address
//		NSString *macaddress = GetEtherHwAddress_();
//		if (macaddress != nil && [macaddress length] > 0)
//		{
//			NSString *uniqueIdentifier = hashStringWithMD5(macaddress);
//			return std::string([uniqueIdentifier UTF8String]);
//		}
//
//		// If we no longer have UDID, check if we have a stored GLDID
//		NSString* identifier = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleIdentifier"];
//		XKeychainItemWrapper *keychainItem = [[XKeychainItemWrapper alloc] initWithIdentifier:@"GAMELOFT_GLDID" service:identifier accessGroup:nil];
//
//		//Retrieve the GLDID from the keychain
//		NSString* gldid_str = [NSString stringWithString:[keychainItem objectForKey:(id)kSecValueData]];
//		//If there is no GLDID on the keychain, we should create one and store it in the keychain
//		if (gldid_str == nil || [gldid_str length] == 0) 
//		{
//			// Create a UUID
//			CFUUIDRef uuidRef = CFUUIDCreate(NULL);
//			if (uuidRef) 
//			{
//				gldid_str = (NSString *)CFUUIDCreateString(NULL, uuidRef);
//				CFRelease(uuidRef);
//			}
//
//			//store it in the keychain.
//			[keychainItem setObject:gldid_str forKey:(id)kSecAttrAccount];
//		}
//		
//		[keychainItem release];
//        
//        std::string final_GLDID = std::string([gldid_str UTF8String]);
//        
//#if defined(ACTIVATE_MAC_TRANSITION)
//        std::string mac = final_GLDID;
//        std::string IDFA = GetIDFA();
//        std::string IDFV = GetIDFV();
//        
//        	//this is the composite GLDID
//        if (mac.empty() == false)
//        {
//            char aux_buffer[1024];
//            sprintf(aux_buffer, "md5mac=%s ", mac.c_str());
//            final_GLDID = final_GLDID + aux_buffer;
//        }
//        
//        if (IDFA.empty() == false)
//        {
//            char aux_buffer[1024];
//            sprintf(aux_buffer, "idfa=%s ", IDFA.c_str());
//            final_GLDID = final_GLDID + aux_buffer;
//        }
//        
//        if (IDFV.empty() == false)
//        {
//            char aux_buffer[1024];
//            sprintf(aux_buffer, "idfv=%s ", IDFV.c_str());
//            final_GLDID = final_GLDID + aux_buffer;
//        }
//#endif
//
//		return final_GLDID;
//	}
//    

std::string DeviceInfo::GetOSVersion()
{
	// check if the UIDevice class is loaded
	if (NSClassFromString(@"UIDevice") == nil)
		return "";
	
	if ([[UIDevice currentDevice] respondsToSelector:@selector(systemVersion)] == FALSE)
		return "";
	
	NSString *ios_version = [UIDevice currentDevice].systemVersion;
	return std::string([ios_version UTF8String]);
}

::uint32_t DeviceInfo::GetScreenW()
{
	return 0;//(unsigned int)[[UIScreen mainScreen] bounds].size.width;
}

::uint32_t DeviceInfo::GetScreenH()
{
	return 0;//(unsigned int)[[UIScreen mainScreen] bounds].size.height;
}


std::string DeviceInfo::GetIDFV()
{
	// check if the UIDevice class is loaded
	if (NSClassFromString(@"UIDevice") == nil)
		return "";
	
	// extra check just to be safe - the NSUUID should exist
	if (NSClassFromString(@"NSUUID") == nil)
		return "";
	
	// check if identifierForVendor param exists into UIDevice class
	if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)] == FALSE)
		return "";
	
	NSString *VenID = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
	return std::string([VenID UTF8String]);
}

bool DeviceInfo::IsAdvertisingTrackingEnable()
{
	// check if the ASIdentifierManager class is loaded
	if (NSClassFromString(@"ASIdentifierManager") == nil)
		return false;
	
	if ([[ASIdentifierManager sharedManager] respondsToSelector:@selector(isAdvertisingTrackingEnabled)] == FALSE)
		return false;
	
	return [ASIdentifierManager sharedManager].isAdvertisingTrackingEnabled ? true : false;
}

std::string DeviceInfo::GetIDFA()
{
	// if the tracking is enabled, then we will return a dummy IDFA because this shouldn't be used
	if (IsAdvertisingTrackingEnable() == false)
		return "00000000-0000-0000-0000-000000000000";
	
	// check if the ASIdentifierManager class is loaded
	if (NSClassFromString(@"ASIdentifierManager") == nil)
		return "00000000-0000-0000-0000-000000000000";
	
	// extra check just to be safe - the NSUUID should exist is ASIdentifierManager class exists
	if (NSClassFromString(@"NSUUID") == nil)
		return "00000000-0000-0000-0000-000000000000";
	
	// extra check to see if ASIdentifierManager class has advertisingIdentifier property
	if ([[ASIdentifierManager sharedManager] respondsToSelector:@selector(advertisingIdentifier)] == FALSE)
		return "00000000-0000-0000-0000-000000000000";
	
	// get the advertising identifier from ASIdentifierManager class
	NSString* AdvID = [[ASIdentifierManager sharedManager].advertisingIdentifier UUIDString];
	
	return std::string([AdvID UTF8String]);
}
