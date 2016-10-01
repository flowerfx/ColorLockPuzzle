#include "pch.h"

#include "WinSharedManaged.h"
#include "d3d11.h"
#pragma comment(lib, "d3d11.lib")
#include "dxgi.h"
#pragma comment(lib, "dxgi.lib")

#if defined(OS_WP81)
#include "client.h"
#endif

#ifdef OS_W10
#include "wrl/client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
	static WCPToolkit::SystemInfo::CoppaCountryCallback s_CoppaCallback = nullptr;

#if (WCP_TEST_SAMPLE == 1)
	int WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetSharedSystemMemory()
	{
		return (int)WCPToolkit::SystemInfo::GetSharedSystemMemory();
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetDedicatedVideoMemory()
	{
		return (int)WCPToolkit::SystemInfo::GetDedicatedVideoMemory();
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetDedicatedSystemMemory()
	{
		return (int)WCPToolkit::SystemInfo::GetDedicatedSystemMemory();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetDeviceCountry()
	{
		std::string sStr = WCPToolkit::SystemInfo::GetDeviceCountry();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
	bool WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEHasKeyboardInput()
	{
		return WCPToolkit::SystemInfo::HasKeyboardInput();
	}
	bool WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEHasMouseInput()
	{
		return WCPToolkit::SystemInfo::HasMouseInput();
	}
	bool WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEHasTouchInput()
	{
		return WCPToolkit::SystemInfo::HasTouchInput();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetGraphicsFeatureLevel()
	{
		std::string sStr = WCPToolkit::SystemInfo::GetGraphicsFeatureLevel();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetDeviceGPUName()
	{
		std::string sStr = WCPToolkit::SystemInfo::GetDeviceGPUName();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetNumberOfCpuCores()
	{
		return WCPToolkit::SystemInfo::GetNumberOfCpuCores();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetProcessorDetails()
	{
		std::string sStr = WCPToolkit::SystemInfo::GetProcessorDetails();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
#endif

	void WCPToolkitWinPhone80CppCallbacks::nativeBRIDGECoppaCallback(Platform::String^ iso)
	{
		s_sCoppaCountry = iso;
		std::string s_iso = WCPToolkit::Utils::Wstr2Str(WCPToolkit::Utils::PlatformStr2Wstr(iso));
		WCPToolkit::SystemInfo::CoppaCallback(s_iso);
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceCarrier()
	{
		return GetDeviceCarrier();
	}	
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceLanguage()
	{
		return GetDeviceLanguage();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceName()
	{
		return GetDeviceName();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceFirmware()
	{
		return GetDeviceFirmware();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceManufacturer()
	{
		return GetDeviceManufacturer();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetOSVersion()
	{
		return GetOSVersion();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetFreeRam()
	{
		return GetFreeRam();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceTotalMemory()
	{
		return GetDeviceTotalMemory();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetCurrentMemoryUsage()
	{
		return GetCurrentMemoryUsage();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetMemoryWorkingLimit()
	{
		return GetMemoryWorkingLimit();
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeGetAppMemoryUsageLimit()
	{
		return GetAppMemoryUsageLimit();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetPeakMemoryUsage()
	{
		return GetPeakMemoryUsage();
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeGetFreeDiskSpace()
	{
		return GetFreeDiskSpace();
	}
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetDeviceGPUName()
	{
	return GetDeviceGPUName();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeGetCoppaCountry(Platform::String^ udid)
	{
		GetCoppaCountry(udid);
	}
}
#endif

namespace WCPToolkit
{
#if defined(OS_WP8)
	void SystemInfo::CoppaCallback(std::string iso)
	{
		if (s_CoppaCallback)
			s_CoppaCallback(iso);

		s_CoppaCallback = nullptr;
	}
#endif

	void SystemInfo::GetCoppaCountry(CoppaCountryCallback callback, std::string hdidfv)
	{
		WCPToolkit::Connectivity::InternetConnectionInfo infoCon = WCPToolkit::Connectivity::GetInternetConnectionInfo();
		
		bool hasInternet = (infoCon.connectionType == WCPToolkit::Connectivity::InternetConnectionType::NoConnection ||
			infoCon.connectionType == WCPToolkit::Connectivity::InternetConnectionType::LocalAccess) ? false : true;
		
		if ((hasInternet) && (hdidfv != ""))
		{
			Platform::String^ s_hdidfv = Utils::Wstr2PlatformStr(std::wstring(hdidfv.begin(), hdidfv.end()));

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
			Windows::Web::Http::HttpClient^ httpClient = ref new Windows::Web::Http::HttpClient();
			Windows::Foundation::Uri^ resourceAddress = ref new Windows::Foundation::Uri("http://iap.gameloft.com/windows/country-operator/?country");

			concurrency::cancellation_token_source cancellationTokenSource = concurrency::cancellation_token_source();

			Windows::Web::Http::HttpRequestMessage^ ms = ref new Windows::Web::Http::HttpRequestMessage();
			ms->RequestUri = resourceAddress;
			ms->Headers->Append("x-device-id", s_hdidfv);

			std::async(std::launch::async, [=]()
			{
				try
				{
					auto task1 = concurrency::create_task(httpClient->SendRequestAsync(ms), cancellationTokenSource.get_token());
					task1.wait();
					auto task1_response = task1.get();

					task1_response->EnsureSuccessStatusCode();

					if (task1_response->StatusCode == Windows::Web::Http::HttpStatusCode::Ok)
					{
						auto task2 = concurrency::create_task(task1_response->Content->ReadAsStringAsync());
						task2.wait();
						auto contentString = task2.get();
						Windows::Data::Xml::Dom::XmlDocument^ doc = ref new Windows::Data::Xml::Dom::XmlDocument();
						doc->LoadXml(contentString);
						auto elements = doc->GetElementsByTagName("country");
						Windows::Data::Xml::Dom::XmlElement^ element = safe_cast<Windows::Data::Xml::Dom::XmlElement^>(elements->First()->Current);

						Platform::String^ iso = element->GetAttribute("iso")->ToString();

						std::string strIso = std::string(iso->Begin(), iso->End());

						if (!strIso.empty())
						{
							std::transform(strIso.begin(), strIso.end(), strIso.begin(), ::toupper);
							callback(strIso);
						}
						else
						{
							WCPToolkit::Utils::Log(L"Empty ISO code returned by server.  Returning GetDeviceCountry() instead.");
							callback(WCPToolkit::SystemInfo::GetDeviceCountry());
						}
					}
					else
					{
						WCPToolkit::Utils::Log(L"Error code returned by server.  Returning GetDeviceCountry() instead.");
						callback(WCPToolkit::SystemInfo::GetDeviceCountry());
					}
				}
				catch (Platform::Exception^ e)
				{
					WCPToolkit::Utils::Log(L"Exception thrown when getting country from server.  Returning GetDeviceCountry() instead.");
					callback(WCPToolkit::SystemInfo::GetDeviceCountry());
				}
			}
			);

#elif defined OS_WP8
			if (!s_CoppaCallback)
				s_CoppaCallback = callback;

			Platform::String^ str = WCPToolkit::Utils::Wstr2PlatformStr(WCPToolkit::Utils::Str2Wstr(hdidfv));
			NATIVE_CALL(nativeGetCoppaCountry)(str);
#endif
		}
		else
		{
#if defined(OS_WP8)
			Platform::String^ str = WCPToolkit::Utils::Wstr2PlatformStr(WCPToolkit::Utils::Str2Wstr(WCPToolkit::SystemInfo::GetDeviceCountry()));
			WCPToolkit::WCPToolkitWinPhone80CppCallbacks::s_sCoppaCountry = str;
#endif
			callback(WCPToolkit::SystemInfo::GetDeviceCountry());
		}
	}

	std::string SystemInfo::GetDeviceName()
	{
		std::string str = WCP_NOT_AVAILABLE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation info;
		Platform::String^ sString = info.SystemProductName;
		str = std::string(sString->Begin(), sString->End());
#elif defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeGetDeviceName)();
		str = std::string(sString->Begin(), sString->End());
#endif

		return str;
	}

	std::string SystemInfo::GetDeviceFirmware()
	{
		std::string str = WCP_NOT_AVAILABLE;

#if defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeGetDeviceFirmware)();
		str = std::string(sString->Begin(), sString->End());
#endif

		return str;
	}

	float SystemInfo::GetDeviceTotalMemory()
	{
		float fARam = 0.0f;

		unsigned long long sum = 0;
		sum += SystemInfo::GetDedicatedSystemMemory();
		sum += SystemInfo::GetDedicatedVideoMemory();
		sum += SystemInfo::GetSharedSystemMemory();
		fARam = sum / 1000000.0f;

		return fARam;
	}

	float SystemInfo::GetFreeRam()
	{
		float fFRam = 0.0f;
#if defined(OS_W8)
#elif defined(OS_WP8)
		fFRam = NATIVE_CALL(nativeGetFreeRam)();
#endif

		return fFRam;
	}

	float SystemInfo::GetCurrentMemoryUsage()
	{
		float fFRam = 0.0f;
#if defined(OS_W8)
#elif defined(OS_WP8)
		fFRam = NATIVE_CALL(nativeGetCurrentMemoryUsage)();
#endif

		return fFRam;
	}

	float SystemInfo::GetPeakMemoryUsage()
	{
		float fFRam = 0.0f;
#if defined(OS_W8)
#elif defined(OS_WP8)
		fFRam = NATIVE_CALL(nativeGetPeakMemoryUsage)();
#endif

		return fFRam;
	}

	float SystemInfo::GetMemoryWorkingLimit()
	{
		float fFRam = 0.0f;
#if defined(OS_W8)
#elif defined(OS_WP8)
		fFRam = NATIVE_CALL(nativeGetMemoryWorkingLimit)();
#endif

		return fFRam;
	}

	std::string SystemInfo::GetDeviceManufacturer()
	{
		std::string str = WCP_NOT_AVAILABLE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation info;
		Platform::String^ devManufacturer = info.SystemManufacturer;
		str = std::string(devManufacturer->Begin(), devManufacturer->End());
#elif defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeGetDeviceManufacturer)();
		str = std::string(sString->Begin(), sString->End());
#endif

		return str;
	}
	
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	template <typename F>
	auto run_now(F f)->decltype(f())
	{
		typedef decltype(f()) result_type;

		result_type result;
		std::thread operation_wrapper(
			[f, &result]()
		{
			concurrency::task<result_type> operation = concurrency::task<result_type>(f);
			result = operation.get();
		});
		operation_wrapper.join();

		return result;
	}
	Platform::String^ DEVICE_CLASS_KEY = "{A45C254E-DF1C-4EFD-8020-67D146A850E0},10";
	Platform::String^ DEVICE_DRIVER_VERSION_KEY = "{A8B865DD-2E3D-4094-AD97-E593A70C75D6},3";
	Platform::String^ ROOT_CONTAINER = "{00000000-0000-0000-FFFF-FFFFFFFFFFFF}";
	Platform::String^ ROOT_CONTAINER_QUERY = "System.Devices.ContainerId:=\"" + ROOT_CONTAINER + L"\"";
	Platform::String^ DEVICE_CLASS_KEY_NO_SEMICOLON = "{A45C254E-DF1C-4EFD-8020-67D146A850E0}10";
	Platform::String^ HAL_DEVICE_CLASS = "{4d36e966-e325-11ce-bfc1-08002be10318}";
#endif

	std::string SystemInfo::GetOSVersion()
	{
		std::string str = WCP_NOT_AVAILABLE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		//Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation osInfo;
		//Platform::String^ os = osInfo.OperatingSystem;
		//std::string sOS = std::string(std::string(os->Begin(), os->End()));

		return run_now(
			[]() -> std::string
		{
			using namespace Platform::Collections;
			using namespace Concurrency;

			auto properties = ref new Platform::Collections::Vector<Platform::String^>();
			properties->Append(DEVICE_DRIVER_VERSION_KEY);
			properties->Append(DEVICE_CLASS_KEY);

			auto t = task<Windows::Devices::Enumeration::Pnp::PnpObjectCollection^>(Windows::Devices::Enumeration::Pnp::PnpObject::FindAllAsync(Windows::Devices::Enumeration::Pnp::PnpObjectType::Device, properties, ROOT_CONTAINER_QUERY))
				.then([&](Windows::Devices::Enumeration::Pnp::PnpObjectCollection^ containers) -> std::string
			{
				char version[64] = "XX";
				for (unsigned int i = 0; i < containers->Size; i++)
				{
					Windows::Devices::Enumeration::Pnp::PnpObject^ container = containers->GetAt(i);
					if (container != nullptr && container->Properties != nullptr)
					{
						auto prop = container->Properties->Lookup(DEVICE_CLASS_KEY_NO_SEMICOLON);
						if (prop != nullptr && prop->ToString()->Equals(HAL_DEVICE_CLASS))
						{
							auto ver = container->Properties->Lookup(DEVICE_DRIVER_VERSION_KEY);
							if (ver != nullptr)
							{
								auto v = ver->ToString();
								wcstombs_s(NULL, version, 64, v->Data(), v->Length());
								return version;
							}
						}
					}
				}
				return version;
			});
			std::string version = t.get();
			if (version.find("6.3") == 0)
				return "8.1";
			if (version.find("6.2") == 0)
				return "8.0";
			if (version.find("10.0") == 0)
				return "10.0";
			return t.get();
		});

#elif defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeGetOSVersion)();
		str = std::string(sString->Begin(), sString->End());
#endif	

		return str;
	}

	float SystemInfo::GetFreeDiskSpace()
	{
		float fFreeDiskSpace = 0.0f;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		// Get local folder.
		const wchar_t *path = Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data();
		__int64 totalFreeBytes	= 0;
			
		// Get the number of free bytes on the drive containing the local folder.
		if( GetDiskFreeSpaceEx(path, NULL, NULL, (PULARGE_INTEGER) &totalFreeBytes) != 0)
		{
			fFreeDiskSpace = totalFreeBytes / 1024.0f;
		}
		else
		{
#if defined (_DEBUG)
			// Error handling.
			std::wstring errorMessage = L"ERROR - Could not get disk free space. Error code: " + std::to_wstring(GetLastError());
			WCPToolkit::Utils::Log(errorMessage.c_str());
#endif
		}
		
#elif defined(OS_WP8)
		fFreeDiskSpace = NATIVE_CALL(nativeGetFreeDiskSpace)();
#endif

		return fFreeDiskSpace;
	}

	
	std::string SystemInfo::GetProcessorDetails()
	{
		std::string str = WCP_NOT_AVAILABLE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		using namespace Concurrency;
		using namespace Windows::Foundation;
		using namespace Windows::Devices::Enumeration;
	
		class InnerClass : public agent
		{
		public:
			InnerClass() : output() {}
	
			void run()
			{
						
				// Processor GUID.	{97FADB10-4E33-40AE-359C-8BEF029DBDD0}
				// Memory GUID.		{3FD0F03D-92E0-45FB-B75C-5ED8FFB01021}
	
				std::string guidStr = "{97FADB10-4E33-40AE-359C-8BEF029DBDD0}";
	
				// Selector string.
				std::string sel = "System.Devices.InterfaceClassGuid:=\"" + guidStr + "\"";
				Platform::String^ selector = ref new Platform::String( std::wstring(sel.begin(), sel.end()).c_str() );
	
				try
				{
					task<DeviceInformationCollection^> ( DeviceInformation::FindAllAsync( selector, nullptr) ).
						then([this] (DeviceInformationCollection^ devices)
					{
						if( devices != nullptr )
						{
									
							if( devices->Size != 0 )
							{
								// The output is for each logical core, so just process the first one.
								DeviceInformation^ di = devices->GetAt( 0 );
	
								auto test = di->Id;
	
								auto it = di->Properties->First();
	
								while( it->HasCurrent )
								{
									auto key = it->Current->Key;
	
									if ( key->Equals( L"System.Devices.DeviceInstanceId" ) )
									{
										auto val = it->Current->Value->ToString();
	
										output = std::string( val->Begin(), val->End() );
	
										//std::replace( output.begin(), output.end(), '\\', '\n' );
										//std::replace( output.begin(), output.end(), '-', '\n' );
										//std::remove( output.begin(), output.end(), '_' );
									}
	
									it->MoveNext();
								}
							}
						}
	
						this->done();
					}
					);
	
				}
				catch(std::exception& e)
				{
					// Error handling.
					WCPToolkit::Utils::Log(WCPToolkit::Utils::Str2Wstr(e.what()));
					this->done();
					e.~exception();
				}
			}
	
			std::string output;
		};
	
		InnerClass* ag;
	
		ag = new InnerClass();
				
		for (;;)
		{
			if(ag == nullptr) 
			{
				WCPToolkit::Utils::Log(L"ERROR - Could not create the task to get the processor details.");
				return "ERROR";
			}
	
			if( false == ag->start() )
			{
				WCPToolkit::Utils::Log(L"ERROR - Could not start the task to get the processor details.");
				return "ERROR";
			}
	
			concurrency::agent::wait(ag);
	
			break;
		}
	
		if ( ag )
		{
			str = std::string(ag->output);
			delete ag;
		}
#elif defined(OS_WP8)
#endif
		return str;
	}

	int SystemInfo::GetNumberOfCpuCores()
	{
		// http://msdn.microsoft.com/en-us/library/windows/desktop/ms724958(v=vs.85).aspx
		SYSTEM_INFO sysInfo;
		GetNativeSystemInfo(&sysInfo);
		return sysInfo.dwNumberOfProcessors;
	}
	
	std::string SystemInfo::GetDeviceGPUName()
	{
		std::string sStr = WCP_NOT_AVAILABLE;

		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;

		DXGI_ADAPTER_DESC adapterDesc;

		CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&factory);
		factory->EnumAdapters(0, &adapter);
		adapter->GetDesc(&adapterDesc);
		Platform::String^ output = ref new Platform::String(adapterDesc.Description);

		std::wstring wResult(output->Begin());
		sStr = WCPToolkit::Utils::Wstr2Str(wResult);

		return sStr;
	}

	unsigned long long SystemInfo::GetSharedSystemMemory()
	{
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		DXGI_ADAPTER_DESC adapterDesc;

		CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&factory);
		factory->EnumAdapters(0, &adapter);
		adapter->GetDesc(&adapterDesc);

		return adapterDesc.SharedSystemMemory;
	}
	unsigned long long SystemInfo::GetDedicatedVideoMemory()
	{
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		DXGI_ADAPTER_DESC adapterDesc;

		CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&factory);
		factory->EnumAdapters(0, &adapter);
		adapter->GetDesc(&adapterDesc);

		return adapterDesc.DedicatedVideoMemory;
	}
	unsigned long long SystemInfo::GetDedicatedSystemMemory()
	{
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		DXGI_ADAPTER_DESC adapterDesc;

		CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&factory);
		factory->EnumAdapters(0, &adapter);
		adapter->GetDesc(&adapterDesc);

		return adapterDesc.DedicatedSystemMemory;
	}

	int SystemInfo::GetAppMemoryUsageLimit()
	{
		int nMemLimit = 0;

#if defined(OS_W8)
#elif defined(OS_WP8)
		nMemLimit = NATIVE_CALL(nativeGetAppMemoryUsageLimit)();
#endif

		return nMemLimit;
	}
	

	std::wstring SystemInfo::GetDeviceCarrier()
	{
		std::wstring str = Utils::Str2Wstr(WCP_NOT_AVAILABLE);
#if defined(OS_W8)
		auto deviceAccountIds = Windows::Networking::NetworkOperators::MobileBroadbandAccount::AvailableNetworkAccountIds;
		if (deviceAccountIds->Size != 0)
		{
			auto mobileBroadbandAccount = Windows::Networking::NetworkOperators::MobileBroadbandAccount::CreateFromNetworkAccountId(deviceAccountIds->GetAt(0));
			auto serviceProvider = mobileBroadbandAccount->ServiceProviderName;
			str = std::wstring(serviceProvider->Begin(), serviceProvider->End());
			//str = msclr::interop::marshal_as<std::wstring>(serviceProvider);
		}
#elif defined(OS_WP8)
		Platform::String^ sStr = NATIVE_CALL(nativeGetDeviceCarrier)();
		str = std::wstring(sStr->Begin(), sStr->End());
#endif
		return str;
	}

	std::string SystemInfo::GetDeviceCountry()
	{
		std::string str = WCP_NOT_AVAILABLE;
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto userGeoRegion = (ref new Windows::Globalization::GeographicRegion())->CodeTwoLetter;
		str = std::string(userGeoRegion->Begin(), userGeoRegion->End());
#elif defined(OS_WP8)
		GEOID geoID = GetUserGeoID(GEOCLASS_NATION);
		int size = GetGeoInfo(geoID, GEO_ISO2, NULL, 0, 0);
		WCHAR* buffer = new WCHAR[size];
		GetGeoInfo(geoID, GEO_ISO2, buffer, size, 0);
		char* output = new char[size];
		size_t sizeb;
		wcstombs_s(&sizeb, output, size, buffer, size);

		str = std::string(output);
#endif

		if (!str.empty())
		{
			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		}

		return str;
	}

	std::string SystemInfo::GetDeviceLanguage()
	{
		std::string str = WCP_NOT_AVAILABLE;
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto languageTag = (ref new Windows::Globalization::Language(Windows::System::UserProfile::GlobalizationPreferences::Languages->GetAt(0)))->LanguageTag;
		// Check
		if (!languageTag->IsEmpty()) 
		{
			// Convert it to std::string
			std::string languageTagStdString(languageTag->Begin(), languageTag->End());

			// Find if the '-'delimiter exists 
			std::size_t foundDelimiterAt = languageTagStdString.find_last_of('-');

			// Ckeck
			if (foundDelimiterAt != std::string::npos)
			{
				// Return (en from en-US)
				str = languageTagStdString.substr(0, foundDelimiterAt);
			}
			else 
			{
				// Return (en)
				str = languageTagStdString;
			}
		}
#elif defined(OS_WP8)
		Platform::String^ sStr = NATIVE_CALL(nativeGetDeviceLanguage)();
		str = std::string(sStr->Begin(), sStr->End());
#endif
		return str;
	}

	std::string SystemInfo::GetGraphicsFeatureLevel()
	{
		HRESULT hr = E_FAIL;
		D3D_FEATURE_LEVEL MaxSupportedFeatureLevel = D3D_FEATURE_LEVEL_9_1;
		D3D_FEATURE_LEVEL FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			FeatureLevels,
			ARRAYSIZE(FeatureLevels),
			D3D11_SDK_VERSION,
			NULL,
			&MaxSupportedFeatureLevel,
			NULL
			);

		std::string featureLevel = WCP_NOT_AVAILABLE;

		if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_11_0)
		{
			featureLevel = "11.0";
		}
		else if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_10_1)
		{
			featureLevel = "10.1";
		}
		else if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_10_0)
		{
			featureLevel = "10.0";
		}
		else if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_9_3)
		{
			featureLevel = "9.3";
		}
		else if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_9_2)
		{
			featureLevel = "9.2";
		}
		else if (MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_9_1)
		{
			featureLevel = "9.1";
		}

		return featureLevel;
	}

	bool SystemInfo::HasKeyboardInput()
	{
		bool bRet = false;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		bRet = (Windows::Devices::Input::KeyboardCapabilities().KeyboardPresent == 1);
#elif defined(OS_WP8)
#endif

		return bRet;
	};

	bool SystemInfo::HasMouseInput()
	{
		bool bRet = false;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		bRet = (Windows::Devices::Input::MouseCapabilities().MousePresent == 1);
#elif defined(OS_WP8)
#endif

		return bRet;
	}

	bool SystemInfo::HasTouchInput()
	{
		bool bRet = false;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		bRet = (Windows::Devices::Input::TouchCapabilities().TouchPresent == 1);
#elif defined(OS_WP8)
		bRet = true;
#endif

		return bRet;
	}
};