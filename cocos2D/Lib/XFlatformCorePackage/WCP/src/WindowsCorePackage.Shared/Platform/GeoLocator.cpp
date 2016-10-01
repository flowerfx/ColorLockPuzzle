#include "pch.h"

#include "Platform/GeoLocator.h"

#include "WinShared.h"


namespace platform
{
#if defined(OS_W8)
	GeoLocator::GeoLocator()
	{
		std::lock_guard<std::mutex> lock(m_guard);
		m_state = DISABLED;
	}

	GeoLocator::~GeoLocator()
	{
		Disable();
	}

	void GeoLocator::Enable()
	{
		std::lock_guard<std::mutex> lock(m_guard);
		// Start with a required update frequency of 5 minutes
		WCPToolkit::Sensors::Geolocator::Start(300000);
		m_state = ENABLED;
	}

	void GeoLocator::Disable()
	{
		std::lock_guard<std::mutex> lock(m_guard);
		// Disable the geolocator
		WCPToolkit::Sensors::Geolocator::Stop();
		m_state = DISABLED;
	}
	
	GeoLocator::State GeoLocator::GetState()
	{
		std::lock_guard<std::mutex> lock(m_guard);
		if(m_state != State::DISABLED)
		{
			// Match windows geolocator states to platformbase states
			switch (WCPToolkit::Sensors::Geolocator::GetStatus())
			{
				case WCPToolkit::Sensors::Geolocator::Ready:
					m_state = GeoLocator::State::HAS_VALID_COORDINATES;
					break;
					
				case WCPToolkit::Sensors::Geolocator::Initializing:
					m_state = GeoLocator::State::ENABLED;
					break;
					
				case WCPToolkit::Sensors::Geolocator::NoData:
					m_state = GeoLocator::State::HAS_EXPIRED_COORDINATES;
					break;
					
				case WCPToolkit::Sensors::Geolocator::Disabled:
					m_state = GeoLocator::State::DISABLED;
					break;
					
				case WCPToolkit::Sensors::Geolocator::NotInitialized:
					m_state = GeoLocator::State::HAS_EXPIRED_COORDINATES;
					break;
					
				case WCPToolkit::Sensors::Geolocator::NotAvailable:
					m_state = GeoLocator::State::HAS_LOCATION_SERVICE_DISABLED;
					break;
				break;
			}
		}

		return m_state; 
	}

	bool GeoLocator::IsEnabled()
	{	
		// Check if it was enabled and no error was thrown
		GeoLocator::State state = GetState();
		if (state != state == GeoLocator::State::DISABLED)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool GeoLocator::HasValidCoordinates()
	{
		// On windows, the WCPToolkit::Sensors::Geolocator::Ready state, there should be data available
		return WCPToolkit::Sensors::Geolocator::HasValidValues();
	}

	GeoLocator::Location GeoLocator::GetLocation()
	{
		// Get location & accuracy and return it
		float latitude = 0.0;
		float longitude = 0.0;
		float altitude = 0.0;
		float accuracy = 0.0;
		WCPToolkit::Sensors::Geolocator::GetLocation(latitude, longitude, altitude, accuracy);

		GeoLocator::Location location;
		location.latitude = latitude;
		location.longitude = longitude;
		location.altitude = altitude;
		location.accuracy = accuracy;

		return location;
	}
#else
	GeoLocator::GeoLocator()
	{
	}

	GeoLocator::~GeoLocator()
	{
	}
#endif

} //namespace platform
