#ifndef __NATIVEGEOLOCATOR_H__
#define __NATIVEGEOLOCATOR_H__

#import <CoreLocation/CoreLocation.h>

namespace platform
{
    class NativeGeoLocatorAdapter;
}

@interface NativeGeoLocator : NSObject <CLLocationManagerDelegate>
{

}

-(id) initWithParent: (platform::NativeGeoLocatorAdapter*)parent;
-(void) Start;
-(void) Stop;
-(void) HandleStart;
-(void) HandleStop;
-(void) RestartTimer;

@end

#endif
