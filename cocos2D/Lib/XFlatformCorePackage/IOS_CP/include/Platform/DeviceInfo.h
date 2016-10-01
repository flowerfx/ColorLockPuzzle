#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

/// This define (PLATFORM_IMPL) must be defined here because is a protection that 
/// that does not let you to include PlatformBaseInterface/PlatformBase.h directly.
#define PLATFORM_IMPL
#include "PlatformBaseInterface/PlatformBase.h"


namespace platform
{
    class DeviceInfo : public DeviceInfoBase
    {
		// Do not remove this friend directive. I know that this is not an 'by the book' approach, but solves the encapsulation problems.
		// Without this line the shared_ptr of template class PlatformImpl cannot access the private constructor & destructor.
		// If you remove this line, you need to make public the constructor & destructor => break the initial design => expose unneded/untested behaviour.
		friend class DeviceInfoBase;

	public:

		/** Use this to retrieve the Device/Platform model.
		* @return a string with the device model. If the string is
		*         'none' means that the platform does not implement this function.*/
		std::string GetModel() override;

		/** Use this to retrieve the Carrier.
		* @return a string with the Carrier. If the string is
		*         'none' means that the platform does not implement this function.*/
		std::string GetCarrier() override;

		/** Use this to retrieve the country code from device.
		* Is used standard ISO-3166-1-alpha-2 (aka two letters uppercase).
		* The implementation of this function must return "XX" if the country
		* code cannot be detected or is in other format.
		* @return a two letters string equivalent with the country code. */
		std::string GetCountryCode() override;

		/** Use this to retrieve the Language.
		* @return a string with the Language. If the string is
		*         'none' means that the platform does not implement this function.*/
		std::string GetLanguage() override;

		/** Use this to retrieve the Gameloft Device Identifier (GLDID).
		* @return a string with the GLDDID. If the string is
		*         'none' means that the platform does not implement this function.*/
		//std::string GetGLDID() override;

		/** Use this to retrieve the OS Version.
		* @return a string with the OS Version. If the string is
		*         'none' means that the platform does not implement this function.*/
		std::string GetOSVersion() override;
		
		/** Use this to retrieve the screen Width.
		* @return an unsigned int that means the width of the screen*/
		::uint32_t GetScreenW() override;

		/** Use this to retrieve the screen Height.
		* @return an unsigned int that means the height of the screen*/
		::uint32_t GetScreenH() override;
		
		
		/** Use this to get the connectivity type that a device have at a moment. Only the default
		 (active) connectivity type will be returned.*/
		ConnectivityType GetConnectivityType() override;
		
		
		/** Use this to get the resolution of default screen. By default (if a platform did
		 not implement this function) will be returned w=0 and h=0. */
		ScreenRes GetScreenRes() override;

        
        std::string GetIDFV();
        
        bool IsAdvertisingTrackingEnable();
        
        std::string GetIDFA();
        
        std::string GetEtherHwAddress();
        
	private:
		DeviceInfo(){}
		
	#if (_MSC_VER < 1800) /// for VS before 2013 there is a bug that require public destructor for shared pointers.
	public:	
	#endif	
		~DeviceInfo(){}
    }; 
	
} // namespace platform

#endif