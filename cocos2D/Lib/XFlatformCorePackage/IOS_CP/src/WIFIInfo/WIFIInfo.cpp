#include "Platform/WIFIInfo.h"
#include "../../../iOSCorePackage_config/iOSCorePackage_config.h" 

#import <Foundation/Foundation.h>
#import <SystemConfiguration/CaptiveNetwork.h>

using namespace platform;

#if defined(USE_WIFIINFO)

const std::vector<std::string> WIFIInfo::wifi_properties ({ "SSID","BSSID"});

WIFIInfo::WIFIInfo()
{
	m_state = DISABLED;
    m_data = "{}";
}

WIFIInfo::~WIFIInfo()
{
	Disable();
}

WIFIInfo::State WIFIInfo::GetState() 
{ 
	return m_state; 
}

void WIFIInfo::Enable()
{
	State expectedState = DISABLED;
	State newState = ENABLED;

	/// If the State is already enabled -> return. 
	if (m_state.compare_exchange_strong(expectedState, newState) == false)
		return;

	/// initialize other things.
	//m_data = "{}";
}

void WIFIInfo::Disable()
{
	State expectedState = ENABLED;
	State newState = DISABLED;

	/// If the State is already disabled -> return. 
	if (m_state.compare_exchange_strong(expectedState, newState) == false)
		return;

	/// disable other things.

}


bool WIFIInfo::IsEnabled()
{
	///retain the value because, m_state is atomic!
	State st = m_state; 
    
	return st != FEATURE_DISABLED && st != DISABLED;
}

std::string WIFIInfo::GetInfoAsJson()
{
    
    // Does not work on the simulator.
    @autoreleasepool
    {
        //syncronous call for retrieving current wifi information, no need to cache the result.
        CFArrayRef supportedWifiInterfaces = CNCopySupportedInterfaces();
        NSDictionary* currentWifiInfo = (__bridge_transfer id)CNCopyCurrentNetworkInfo((CFStringRef)CFArrayGetValueAtIndex(supportedWifiInterfaces, 0));
        CFRelease(supportedWifiInterfaces);
        //NSLog(@"current wifi info =\n%@\n", currentWifiInfo);
        
        if (currentWifiInfo)
        {
            id wifiProperties = [NSMutableArray new];
            std::for_each(wifi_properties.begin(), wifi_properties.end(), ^(std::string wifi_property)
                          {
                              [wifiProperties addObject:[NSString stringWithUTF8String:wifi_property.c_str()]];
                          });
            
            // NSDictionary* currentWifiObject = [(NSDictionary*)currentWifiInfo dictionaryWithValuesForKeys:wifiProperties];
            NSDictionary* currentWifiObject = [NSDictionary dictionaryWithObjects:[currentWifiInfo objectsForKeys:wifiProperties notFoundMarker:@""] forKeys:wifiProperties];
            
            NSDictionary* wifiInfoJson = @{ @"connected_wifi" : currentWifiObject };
            //serialize json to string
            NSError* convertError;
            NSData* jsonData = [NSJSONSerialization dataWithJSONObject:wifiInfoJson
                                                               options:0//NSJSONWritingPrettyPrinted
                                                                 error:&convertError];
            if (jsonData)
            {
                m_data = [[[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding] UTF8String];
            }
            //else { NSLog(@"Encountered an error during serialization: %@", convertError); }
        }
       
    }
    
    //NSLog(@"m_data = %s", m_data.c_str());
    return m_data;

}

#else

WIFIInfo::WIFIInfo()
{
	
}

WIFIInfo::~WIFIInfo()
{
}

WIFIInfo::State WIFIInfo::GetState()
{ 
	return FEATURE_DISABLED; 
}

void WIFIInfo::Enable()
{
}

void WIFIInfo::Disable()
{
}


bool WIFIInfo::IsEnabled()
{
	return false;
}

std::string WIFIInfo::GetInfoAsJson()
{
    return "{}";
}

#endif //USE_WIFIINFO