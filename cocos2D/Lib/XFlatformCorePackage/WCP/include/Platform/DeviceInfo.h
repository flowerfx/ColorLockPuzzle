#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

#include "PlatformBaseInterface/PlatformBase.h"


namespace platform
{
#if defined(WCP_DLL)
#define WINTK_EXPORTED __declspec(dllexport)
#else
#define WINTK_EXPORTED __declspec(dllimport)
#endif

	class WINTK_EXPORTED  DeviceInfo : public DeviceInfoBase<DeviceInfo>
    {
		friend class DeviceInfoBase < DeviceInfo >;
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

		/** Use this to get the connectivity type that a device have at a moment. Only the default
		(active) connectivity type will be returned.*/
		ConnectivityType GetConnectivityType() override;

	private:
		DeviceInfo(){}
		
	#if (_MSC_VER < 1800) /// for VS before 2013 there is a bug that require public destructor for shared pointers.
	public:	
	#endif	
		~DeviceInfo(){}
    }; 
	
} // namespace platform

#endif