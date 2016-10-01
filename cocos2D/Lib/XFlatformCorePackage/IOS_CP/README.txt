iOSCorePackage is a collection of platform dependent functions. 

Usage
=====
1) Add "iOSCorePackage/include" to your include path

2) Create the platform (The game will be the owner):
	#include "Platform/Platform.h"

	std::shared_ptr<platform::PlatformImpl> thePlatform;
	platform::Settings settings;
	thePlatform = platform::PlatformImpl::CreateSingleInstance(settings);

CreateSingleInstance must be called only once (the next calls will return nullptr).
	
3) Pass the platform to Gaia, GLOT and GLAds (the libraries will store it internally as a weak pointer):
	Gaia::GetInstance()->Initialize(thePlatform, Gaia::ProductID(GL_PRODUCT_ID), CLIENTID, true, GaiaCallbackRequestCompleted, this);
	glotv3::TrackingManager::getInstance()->OnLaunch(11111, std::string("0.0.1"), glotv3::EventsJson::GetEventsJson(), std::string(CLIENTID), thePlatform);
	GetGLAdsManager()->SetPlatform(thePlatform); 

GeoLocator specific
===================
4) Enable the GeoLocator based on the player's age:
	unsigned int age = 13;
	if ((gaia::Gaia::GetInstance()->RetrieveAge(age) == (int)gaia::ErrorsCodes::OPERATION_SUCCESSFULL) && (age > 13))
	{
		thePlatform->GetGeoLocator()->Enable();
	}
	else
	{
		thePlatform->GetGeoLocator()->Disable();
	}

5) Fetch the location:
	if (thePlatform->GetGeoLocator()->IsEnabled() && thePlatform->GetGeoLocator()->HasValidCoordinates())
	{
		platform::GeoLocator::Location location = thePlatform->GetGeoLocator()->GetLocation();
		double latitude = location.latitude;
		double longitude = location.longitude;
		double accuracy = location.accuracy;
		double altitude = location.altitude;
	}

GetLocation returns immediately (the location is cached) so you can call it any time with minimal performance costs.
However, you should test for valid coordinates, otherwise you'll get 0.0.

6) On iOS 8.0, you'll have to add the following two lines in info.plist:
	<key>NSLocationWhenInUseUsageDescription</key>
	<string>Reason for accessing location</string>