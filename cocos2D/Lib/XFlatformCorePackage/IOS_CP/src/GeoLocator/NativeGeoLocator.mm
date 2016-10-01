#include "NativeGeoLocator.h"
#include "NativeGeoLocatorAdapter.h"

#ifdef USE_GEOLOCATION


static const unsigned int k_locationRefreshTimeInSeconds = 300;



@implementation NativeGeoLocator
{
	/** This is used just to expose only c++ code ( not ObjectiveC )*/
	platform::NativeGeoLocatorAdapter* m_adapter;
	
	/** This is the actual Location Manager SDK object. */
	CLLocationManager* m_locationManager;
	
	/** This is the timer used to refresh the location */
	NSTimer* m_timer;
	
	/** This bool is used to mark the location as expired if no new location is received after
	 * k_locationRefreshTimeInSeconds seconds. */
	BOOL m_waitingForCallback;
}

-(id) initWithParent: (platform::NativeGeoLocatorAdapter*)theParent
{
	self = [super init];
	m_adapter = theParent;
	m_locationManager = nil;
	
	return self;
}
	
-(void) Start
{
	//NSLog(@"start\n");

	[self performSelectorOnMainThread:@selector(HandleStart) withObject:nil waitUntilDone:YES];
}

-(void) Stop
{
	//NSLog(@"stop\n");

	[self performSelectorOnMainThread:@selector(HandleStop) withObject:nil waitUntilDone:YES];
}

-(void) HandleStart
{
	//NSLog(@"handleStart\n");
	
	m_locationManager = [[CLLocationManager alloc] init];
    m_locationManager.delegate = self;
    m_locationManager.desiredAccuracy = kCLLocationAccuracyKilometer;
    m_locationManager.distanceFilter = kCLDistanceFilterNone;
    [m_locationManager setPausesLocationUpdatesAutomatically:NO];
    
	if ([m_locationManager respondsToSelector:@selector(requestWhenInUseAuthorization)])
    {
        //requestWhenInUseAuthorization is present on iOS8 and later and it triggers authorization prompt
        //also triggers Location Services is disabled prompt
		[m_locationManager requestWhenInUseAuthorization];
    }
    else
    {
        //trigger Location Services is disabled prompt for iOS 7 and lower
        //it's ok for this to be called multiple times
        [m_locationManager startUpdatingLocation];
    }

    if ([CLLocationManager locationServicesEnabled] == NO)
    {
        m_adapter->OnServiceDisabled();
        return;
    }
	
	[self RestartTimer];
	
	m_waitingForCallback = YES;
	[m_locationManager startUpdatingLocation];
}

-(void) HandleStop
{
	//NSLog(@"handleStop\n");
	
	[m_locationManager stopUpdatingLocation];
	[m_locationManager release];
	m_locationManager = nil;
}


-(void) UpdateLocation
{
	//NSLog(@"updateLocation\n");
	if(m_locationManager == nil)
	{
		return;
	}
	
	if (m_waitingForCallback == YES)
		m_adapter->OnExpiredCoordinates();
	
	[self RestartTimer];
	
	m_waitingForCallback = YES;
	[m_locationManager startUpdatingLocation];
}

- (void)RestartTimer
{
	[NSTimer scheduledTimerWithTimeInterval:(k_locationRefreshTimeInSeconds) target:self selector:@selector(UpdateLocation) userInfo:nil repeats:NO];
}

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status
{
	//NSLog(@"locationManager didChangeAuthorizationStatus: %d", status);
	
	if(m_locationManager == nil)
	{
		return;
	}
	
	switch (status)
	{
		case kCLAuthorizationStatusNotDetermined:
			//NSLog(@"locationManager didChangeAuthorizationStatus: kCLAuthorizationStatusNotDetermined");
			m_adapter->OnAccessDenied();
	
			if ([m_locationManager respondsToSelector:@selector(requestWhenInUseAuthorization)])
				[m_locationManager requestWhenInUseAuthorization];
			break;
			
		case kCLAuthorizationStatusRestricted:
			//NSLog(@"locationManager didChangeAuthorizationStatus: kCLAuthorizationStatusRestricted");
			m_adapter->OnServiceDisabled();

			break;
			
		case kCLAuthorizationStatusDenied:
			//NSLog(@"locationManager didChangeAuthorizationStatus: kCLAuthorizationStatusDenied");
			
			if ([CLLocationManager locationServicesEnabled] == NO)
			{
				m_adapter->OnServiceDisabled();
			}
			else
			{
				m_adapter->OnAccessDenied();
				
				//Forced to read the location right now. This is actually an workaround because didChangeAuthorizationStatus
				//is not called when the Location Service is disabled.
				[m_locationManager startUpdatingLocation];
			}

			break;
			
		case kCLAuthorizationStatusAuthorizedWhenInUse:
		case kCLAuthorizationStatusAuthorized:
			//NSLog(@"locationManager didChangeAuthorizationStatus: kCLAuthorizationStatusAuthorized || kCLAuthorizationStatusAuthorizedWhenInUse");
			m_adapter->OnExpiredCoordinates();
			
			//Forced to read the location right now. This is actually an workaround because didChangeAuthorizationStatus
			//is not called when the Location Service is disabled.
			[m_locationManager startUpdatingLocation];
			break;

		default:
			break;
	}
}


- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
	//NSLog(@"locationManager didFailWithError: %@", error);
	if ([[error domain] isEqualToString: kCLErrorDomain])
	{
		if ([error code] == kCLErrorDenied)
		{
			if ([CLLocationManager locationServicesEnabled] == NO)
			{
				m_adapter->OnServiceDisabled();
			}
			else
			{
				m_adapter->OnAccessDenied();
			}
		}
		else
		{
			m_adapter->OnExpiredCoordinates();
		}

		m_waitingForCallback = NO;
	}
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
	//NSLog(@"didUpdateToLocation: %@", newLocation);
	[m_locationManager stopUpdatingLocation];
	
	if (newLocation != nil)
	{
		double latitude = newLocation.coordinate.latitude;
		double longitude = newLocation.coordinate.longitude;		
		double altitude = newLocation.altitude;
		double accuracy = newLocation.horizontalAccuracy;		
		m_waitingForCallback = NO;
		m_adapter->OnLocationChanged(latitude, longitude, altitude, accuracy);
	}
}

@end

#endif // USE_GEOLOCATION