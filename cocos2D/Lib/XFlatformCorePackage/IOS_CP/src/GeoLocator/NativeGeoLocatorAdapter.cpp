#include "NativeGeoLocatorAdapter.h"

#ifdef USE_GEOLOCATION

#include "GeoLocator.h"
#include "NativeGeoLocator.h"

using namespace platform;

NativeGeoLocatorAdapter::NativeGeoLocatorAdapter(GeoLocator* parent)
:m_parent(parent), m_nativeGeoLocator(nil)
{
}

void NativeGeoLocatorAdapter::Start()
{
	m_nativeGeoLocator = [[NativeGeoLocator alloc] initWithParent:this];
	[m_nativeGeoLocator Start];
}

void NativeGeoLocatorAdapter::Stop()
{
	if (m_nativeGeoLocator == nil)
		return;

	[m_nativeGeoLocator Stop];
	[m_nativeGeoLocator release];
	m_nativeGeoLocator = nil;
}

void NativeGeoLocatorAdapter::OnLocationChanged(double latitude, double longitude, double altitude, double accuracy)
{
	m_parent->OnLocationChanged(latitude, longitude, altitude, accuracy);
}

void NativeGeoLocatorAdapter::OnExpiredCoordinates()
{
	m_parent->OnExpiredCoordinates();
}

void NativeGeoLocatorAdapter::OnAccessDenied()
{
	m_parent->OnAccessDenied();
}

void NativeGeoLocatorAdapter::OnServiceDisabled()
{
	m_parent->OnServiceDisabled();
}


#endif