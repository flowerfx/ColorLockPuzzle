#include "pch.h"

#include "Platform/DeviceInfo.h"

#include "WinShared.h"


namespace platform
{
	std::string DeviceInfo::GetModel()
	{
		return WCPToolkit::SystemInfo::GetDeviceName();
	}

	std::string DeviceInfo::GetCarrier()
	{
		return WCPToolkit::Utils::Wstr2Str(WCPToolkit::SystemInfo::GetDeviceCarrier());
	}

	std::string DeviceInfo::GetCountryCode()
	{
		return WCPToolkit::SystemInfo::GetDeviceCountry();
	}

	std::string DeviceInfo::GetLanguage()
	{
		return WCPToolkit::SystemInfo::GetDeviceLanguage();
	}

	std::string DeviceInfo::GetOSVersion()
	{
		return WCPToolkit::SystemInfo::GetOSVersion();
	}

	
	DeviceInfo::ConnectivityType DeviceInfo::GetConnectivityType()
	{ 
		WCPToolkit::Connectivity::InternetConnectionType connectionType(WCPToolkit::Connectivity::GetInternetConnectionInfo().connectionType);

		// Check and return
		switch (connectionType)
		{
		case WCPToolkit::Connectivity::InternetConnectionType::EthernetConnection:
			return DeviceInfo::ConnectivityType::_ETHERNET_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::Mobile2GConnection:
			return DeviceInfo::ConnectivityType::_2G_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::Mobile3GConnection:
			return DeviceInfo::ConnectivityType::_3G_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::Mobile4GConnection:
			return DeviceInfo::ConnectivityType::_4G_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::MobileConnection:
			return DeviceInfo::ConnectivityType::_MOBILE_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::NoConnection:
			return DeviceInfo::ConnectivityType::_NO_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::WirelessConnection:
			return DeviceInfo::ConnectivityType::_WIFI_CONNECTIVITY;
		case WCPToolkit::Connectivity::InternetConnectionType::UnknownConnection:
			return DeviceInfo::ConnectivityType::_UNKNOWN_CONNECTIVITY;
		}

		return DeviceInfo::ConnectivityType::_UNKNOWN_CONNECTIVITY; 
	}
}
