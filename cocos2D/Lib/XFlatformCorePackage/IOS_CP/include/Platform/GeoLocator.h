#ifndef __GEOLOCATOR_H__
#define __GEOLOCATOR_H__

/// This define (PLATFORM_IMPL) must be defined here because is a protection that 
/// that does not let you to include PlatformBaseInterface/PlatformBase.h directly.
#define PLATFORM_IMPL
#include "PlatformBaseInterface/PlatformBase.h"
#include <mutex>

namespace platform
{
    class NativeGeoLocatorAdapter;
    
	class GeoLocator : public GeoLocatorBase
	{
		// Do not remove this friend directive. I know that this is not an 'by the book' approach, but solves the encapsulation problems.
		// Without this line the shared_ptr of template class PlatformImpl cannot access the private constructor & destructor.
		// If you remove this line, you need to make public the constructor & destructor => break the initial design => expose unneded/untested behaviour.
		friend class GeoLocatorBase;

		friend class NativeGeoLocatorAdapter;
	
	public:
		void Enable() override;
		void Disable() override;
		bool IsEnabled() override;
		Location GetLocation() override;	
		bool HasValidCoordinates() override;
		State GetState() override;
           
	private:
		void OnLocationChanged(double latitude, double longitude, double altitude, double accuracy);
		void OnAccessDenied();
		void OnServiceDisabled();
		void OnExpiredCoordinates();

	private:
		NativeGeoLocatorAdapter* m_nativeGeoLocatorAdapter;
		Location m_location;
		std::mutex m_guard;
		
		GeoLocator();
		~GeoLocator();
		
		/** This is used to store the current state */
		std::atomic<State> m_state;
	}; 

} // namespace platform

#endif //__GEOLOCATOR_H__