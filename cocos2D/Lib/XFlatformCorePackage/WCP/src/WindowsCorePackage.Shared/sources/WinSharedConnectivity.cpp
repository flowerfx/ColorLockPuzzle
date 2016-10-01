#include "pch.h"

#include "WinShared.h"


#if defined(OS_WP81)
#include "client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
#if (WCP_TEST_SAMPLE == 1)
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetInternetConnectionInfo()
	{
		std::string sStr = WCPToolkit::Connectivity::GetInternetConnectionInfo().ToString();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
#endif

	int WCPToolkitWinPhone80CppCallbacks::nativeGetInternetConnectionInfo()
	{
		return GetInternetConnectionInfo();
	}
}
#endif

namespace WCPToolkit
{

	Connectivity::InternetConnectionInfo Connectivity::GetInternetConnectionInfo()
	{
		Connectivity::InternetConnectionInfo result;
		result.connectionType = Connectivity::InternetConnectionType::UnknownConnection;
		result.isRoaming = false;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		Windows::Networking::Connectivity::ConnectionProfile^ InternetConnectionProfile = Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();

		if (InternetConnectionProfile == nullptr)
		{
			result.connectionType = InternetConnectionType::NoConnection;
			return result;
		}

		Windows::Networking::Connectivity::NetworkConnectivityLevel Conn = InternetConnectionProfile->GetNetworkConnectivityLevel();

		if (Conn == Windows::Networking::Connectivity::NetworkConnectivityLevel::None)
		{
			result.connectionType = InternetConnectionType::NoConnection;
			return result;
		}
		if (Conn == Windows::Networking::Connectivity::NetworkConnectivityLevel::LocalAccess)
		{
			result.connectionType = InternetConnectionType::LocalAccess;
			return result;
		}
		if (Conn == Windows::Networking::Connectivity::NetworkConnectivityLevel::ConstrainedInternetAccess)
		{
			result.connectionType = InternetConnectionType::ConstrainedIAccess;
			return result;
		}

		// Get the connection profile associated with the internet connection.
		Windows::Networking::Connectivity::ConnectionProfile^ profile = Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();
	
		if ( profile != nullptr)
		{
			/*
				*	Required client version: 8.1
			*/
	
			if( profile->IsWlanConnectionProfile )
			{
				result.connectionType = InternetConnectionType::WirelessConnection;
			}
			else if( profile->IsWwanConnectionProfile )
			{
				result.connectionType = InternetConnectionType::MobileConnection;
	
				Windows::Networking::Connectivity::WwanConnectionProfileDetails^ details = profile->WwanConnectionProfileDetails;
				Windows::Networking::Connectivity::WwanDataClass dataClass = details->GetCurrentDataClass();
				Windows::Networking::Connectivity::ConnectionCost^ cost = profile->GetConnectionCost();
	
				switch (dataClass)
				{
				case Windows::Networking::Connectivity::WwanDataClass::None:
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Gprs:
					result.connectionType = InternetConnectionType::Mobile2GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Edge:
					result.connectionType = InternetConnectionType::Mobile2GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Umts:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Hsdpa:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Hsupa:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::LteAdvanced:
					result.connectionType = InternetConnectionType::Mobile4GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma1xRtt:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma1xEvdo:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma1xEvdoRevA:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma1xEvdv:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma3xRtt:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Cdma1xEvdoRevB:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::CdmaUmb:
					result.connectionType = InternetConnectionType::Mobile3GConnection;
					break;
				case Windows::Networking::Connectivity::WwanDataClass::Custom:
					break;
				default:
					break;
				}
	
				result.isRoaming = cost->Roaming;
			}
			else if (profile->NetworkAdapter->IanaInterfaceType == 6)
			{
				result.connectionType = InternetConnectionType::EthernetConnection;
			}
			else
			{
				result.connectionType = InternetConnectionType::UnknownConnection;
			}
		}
#elif defined(OS_WP8)
		result.connectionType = (InternetConnectionType)NATIVE_CALL(nativeGetInternetConnectionInfo)();
#endif
	
		return result;
	}

	std::string Connectivity::InternetConnectionInfo::ToString()
	{
		static const char* InternetConnectionTypeAsString[] = 
		{
			"ETHERNET",
			"WIFI",
			"4G",
			"3G", 
			"2G",
			"MOBILE", 
			"Local Access",
			"Constrained Internet Access",
			"UNKNOWN",
			"OFF"
		};
	
		return std::string(InternetConnectionTypeAsString[connectionType]);
	}

};