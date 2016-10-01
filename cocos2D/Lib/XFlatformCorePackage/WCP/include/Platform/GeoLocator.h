#ifndef __GEOLOCATOR_H__
#define __GEOLOCATOR_H__

#include "../PlatformBaseInterface/PlatformBase.h"

namespace platform
{

#if defined(WCP_DLL)
#define WINTK_EXPORTED __declspec(dllexport)
#else
#define WINTK_EXPORTED __declspec(dllimport)
#endif

	class WINTK_EXPORTED GeoLocator : public GeoLocatorBase<GeoLocator>
	{
		friend class GeoLocatorBase < GeoLocator > ;

#if defined(OS_W8)
	public:
		/** This will enable the service. The OS can/will prompt the user with a question
		regarding this action. The status of the service will be SERVICE__ENABLED. */
		void Enable() override;

		/** This will Disable the service. The status of the service will be SERVICE__DISABLED. */
		void Disable() override;

		/** This is used to get the state of GeoLocator.
		* @return the value of m_state protected member, used to store the state.*/
		State GetState() override;
		
		/** Use this to check if the service was enabled or not. After you call Enabled(), the OS will
		* initialize the service.
		* @return true if the service was enabled.*/
		bool IsEnabled() override;

		/** Use this to check if the GeoLocator can retrieve valid coordinates. The locations cannot be available from the moment
		* when the service is enabled, so , we need to check if the coordinates are valid.
		* @return true if the coordinates are valid. */
		bool HasValidCoordinates() override;

		/** Use it to get the Location.
		* @return a structure of Location type. */
		Location GetLocation() override;
#endif

	private:
		GeoLocator();

#if (_MSC_VER < 1800) /// for VS before 2013 there is a bug that require public destructor for shared pointers.
	public:
#endif	
		~GeoLocator();
private:
		
		/** protect the m_state*/
		 std::mutex m_guard;
		
		/** This is used to store the current state */
		State m_state;
	};

} // namespace platform

#endif //__GEOLOCATOR_H__