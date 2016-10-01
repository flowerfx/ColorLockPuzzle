#include "GeoLocator.h"
#include "NativeGeoLocatorAdapter.h"

using namespace platform;

#if defined(USE_GEOLOCATION)
#import <CoreLocation/CoreLocation.h>

GeoLocator::GeoLocator()
{
	m_state = DISABLED;
	m_nativeGeoLocatorAdapter = new NativeGeoLocatorAdapter(this);
}

GeoLocator::~GeoLocator()
{
	Disable();
	delete m_nativeGeoLocatorAdapter;
}

GeoLocator::State GeoLocator::GetState() 
{
	return m_state.load();
}

void GeoLocator::Enable()
{
	State state = m_state.load();
	if ((state != DISABLED)
	 && (state != HAS_LOCATION_SERVICE_DISABLED)
	 && (state != HAS_ACCESS_DENIED)
	 && (state != HAS_ERRORS))
		return;
	
	std::lock_guard<std::mutex> lock(m_guard);
	
	m_state = ENABLED;
	
	m_nativeGeoLocatorAdapter->Start();
}

void GeoLocator::Disable()
{
	if (m_state == DISABLED)
		return;
	
	std::lock_guard<std::mutex> lock(m_guard);

	m_nativeGeoLocatorAdapter->Stop();	
	m_location = Location();
	m_state = DISABLED;
}

GeoLocator::Location GeoLocator::GetLocation()
{
	std::lock_guard<std::mutex> lock(m_guard);
	return m_location;
}

bool GeoLocator::IsEnabled()
{
	State state = m_state.load();
	return state != FEATURE_DISABLED && state != DISABLED;
}

bool GeoLocator::HasValidCoordinates()
{
	return m_state == HAS_VALID_COORDINATES;
}

void GeoLocator::OnLocationChanged(double latitude, double longitude, double altitude, double accuracy)
{
	if(m_state == DISABLED)
		return;
	
	std::lock_guard<std::mutex> lock(m_guard);
	m_location.latitude = latitude;
	m_location.longitude = longitude;
	m_location.altitude = altitude;
	m_location.accuracy = accuracy;
	m_state = HAS_VALID_COORDINATES;
}

void GeoLocator::OnAccessDenied()
{
	if(m_state == DISABLED)
		return;
	
	m_state = HAS_ACCESS_DENIED;
}

void GeoLocator::OnServiceDisabled()
{
	if(m_state == DISABLED)
		return;
	
	m_state = HAS_LOCATION_SERVICE_DISABLED;
}
void GeoLocator::OnExpiredCoordinates()
{
	if(m_state == DISABLED)
		return;
	
	m_state = HAS_EXPIRED_COORDINATES;
}


#else

GeoLocator::GeoLocator()
{
	
}

GeoLocator::~GeoLocator()
{
}

void GeoLocator::Enable()
{
}

GeoLocator::State GeoLocator::GetState()
{
	return FEATURE_DISABLED;
}

void GeoLocator::Disable()
{
}

GeoLocator::Location GeoLocator::GetLocation()
{
	return Location();
}

void GeoLocator::OnAccessDenied()
{
}

void GeoLocator::OnExpiredCoordinates()
{
}

bool GeoLocator::HasValidCoordinates()
{
	return false;
}

bool GeoLocator::IsEnabled()
{
	return false;
}

#endif //USE_GEOLOCATION