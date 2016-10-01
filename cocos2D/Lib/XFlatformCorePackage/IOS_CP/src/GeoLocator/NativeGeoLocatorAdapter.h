#ifndef __NATIVEGEOLOCATORADAPTER_H__
#define __NATIVEGEOLOCATORADAPTER_H__

#include "iOSCorePackage_config.h"

#ifdef USE_GEOLOCATION

@class NativeGeoLocator;

namespace platform
{
	class GeoLocator;

	class NativeGeoLocatorAdapter
	{
	public:
		NativeGeoLocatorAdapter(GeoLocator* parent);
		~NativeGeoLocatorAdapter() {}
		
		void Start();
		void Stop();

		void OnLocationChanged(double latitude, double longitude, double altitude, double accuracy);
		void OnAccessDenied();
		void OnServiceDisabled();
		void OnExpiredCoordinates();
		
	private:
		platform::GeoLocator* m_parent;
		NativeGeoLocator* m_nativeGeoLocator;
	};
} // platform

#endif

#endif