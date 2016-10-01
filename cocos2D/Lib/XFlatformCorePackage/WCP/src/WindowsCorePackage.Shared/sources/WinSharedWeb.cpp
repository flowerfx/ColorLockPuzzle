#include "pch.h"

#include "WinShared.h"


#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
#include "WinSharedManaged.h"
#include "InGameBrowser\src\win8.1\InGameBrowser.h"
#endif

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
	void WCPToolkitWinPhone80CppCallbacks::nativeLaunchBrowser(Platform::String^ sUrl)
	{
		LaunchBrowser(sUrl);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeLaunchMarketplaceDetail()
	{
		LaunchMarketplaceDetail();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeLaunchMarketplaceReview()
	{
		LaunchMarketplaceReview();
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeInitIGB(IGBTransferParams^ p)
	{
		InitIGB(p);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowCustomerCare()
	{
		ShowCustomerCare();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowCustomerCareWithBanType(int type)
	{
		ShowCustomerCareWithBanType(type);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowForum()
	{
		ShowForum();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowTerms()
	{
		ShowTerms();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowPrivacy()
	{
		ShowPrivacy();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowNews()
	{
		ShowNews();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowEULA()
	{
		ShowEULA();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowFacebookShare(bool openExternally)
	{
		ShowFacebookShare(openExternally);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowFacebookPage(bool openExternally)
	{
		ShowFacebookPage(openExternally);
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetFacebookShareLink()
	{
		return GetFacebookShareLink();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowCustomLink(Platform::String^ sURL)
	{
		ShowCustomLink(sURL);
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetIGBGameUpdateURL()
	{
		return GetIGBGameUpdateURL();
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetIGBGameReviewURL()
	{
		return GetIGBGameReviewURL();
	}
	
	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBGameLanguage(Platform::String^ language)
	{
		IGBGameLanguageChanged(language);
	}
	
	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBBaseURL(Platform::String^ baseURL)
	{
		IGBBaseURLChanged(baseURL);
	}
	
	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBUserAge(int userAge)
	{
		IGBUserAgeChanged(userAge);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBUserGender(int gender)
	{
		IGBUserGenderSet(gender);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBAnonymousAccount(Platform::String^ anonymousAccount)
	{
		IGBAnonymousAccountChanged(anonymousAccount);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBFacebookID(Platform::String^ facebookID)
	{
		IGBFacebookIDChanged(facebookID);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBGlliveAccount(Platform::String^ glliveAccount)
	{
		IGBGlliveAccountChanged(glliveAccount);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeSetIGBXboxAccount(Platform::String^ xboxAccount)
	{
		IGBXboxAccountChanged(xboxAccount);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeRefreshUnreadNewsNumber()
	{
		RefreshUnreadNewsNumber();
	}

	int WCPToolkitWinPhone80CppCallbacks::nativeGetUnreadNewsNumber()
	{
		return GetUnreadNewsNumber();
	}

	bool WCPToolkitWinPhone80CppCallbacks::nativeIsIGBOnScreen()
	{
		return IsIGBOnScreen();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeIGBControllerEvent(int eventType, double value)
	{
		IGBControllerEvent(eventType, value);
	}
}
#endif




namespace WCPToolkit
{
	void Web::LaunchBrowser(std::string sURL)
	{
		Platform::String ^psURL = ref new Platform::String(std::wstring(sURL.begin(), sURL.end()).c_str());

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::Utils::RunOnUIThread([=]()
		{
			Windows::System::Launcher::LaunchUriAsync(ref new Windows::Foundation::Uri(psURL));
		});
#elif defined(OS_WP8)
		NATIVE_CALL(nativeLaunchBrowser)(psURL);
#endif
	}

	Web::InGameBrowser::InitParams::InitParams() : userAge(0)
	{
		gameLanguage = "EN";
	}

	void Web::InGameBrowser::Init(const InitParams& parameters)
	{
		std::wstring wsParam;
			
		wsParam.assign(parameters.anonymousAccount.begin(), parameters.anonymousAccount.end());
		auto platAnonymousAccount = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.baseURL.begin(), parameters.baseURL.end());
		auto platBaseURL = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.clientID.begin(), parameters.clientID.end());
		auto platClientID = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.gameCode.begin(), parameters.gameCode.end());
		auto platGameCode = ref new Platform::String(wsParam.c_str());

		auto platGameOperator = ref new Platform::String(L"WIN8");

		wsParam.assign(parameters.gameVersion.begin(), parameters.gameVersion.end());
		auto platGameVersion = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.hdidfv.begin(), parameters.hdidfv.end());
		auto platHDIDFV = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.gameLanguage.begin(), parameters.gameLanguage.end());
		auto platGameLanguage = ref new Platform::String(wsParam.c_str());

		auto waid = WCPToolkit::Identifiers::GetAdvertisingID();
		if (waid.empty())
			waid = "ERROR_GETTING_WAID";
		wsParam.assign(waid.begin(), waid.end());
		auto platWAID = ref new Platform::String(wsParam.c_str());

		wsParam.assign(parameters.xboxID.begin(), parameters.xboxID.end());
		auto platXboxID = ref new Platform::String(wsParam.c_str());

		auto os_ver = WCPToolkit::SystemInfo::GetOSVersion();
		wsParam.assign(os_ver.begin(), os_ver.end());
		auto platOsVersion = ref new Platform::String(wsParam.c_str());

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto IGBParams = ref new WCPToolkit::InGameBrowserLibrary::InitParams();
#elif defined(OS_WP8)
		auto IGBParams = ref new IGBTransferParams();
#endif

		IGBParams->AnonymousAccount = platAnonymousAccount;
		IGBParams->BaseUrl = platBaseURL;
		IGBParams->ClientID = platClientID;
		IGBParams->GameCode = platGameCode;
		IGBParams->GameOperator = platGameOperator;
		IGBParams->GameVersion = platGameVersion;
		IGBParams->HDIDFV = platHDIDFV;
		IGBParams->Language = platGameLanguage;
		IGBParams->WAID = platWAID;
		IGBParams->XboxID = platXboxID;
		IGBParams->UserAge = parameters.userAge;
		IGBParams->OsVersion = platOsVersion;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->Init(IGBParams);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeInitIGB)(IGBParams);
#endif
	}

	void Web::InGameBrowser::ShowCustomerCare()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowCustomerCare();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowCustomerCare)();
#endif
	}

	void Web::InGameBrowser::ShowCustomerCareWithBANType(int banType)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowCustomerCareWithBanType(banType);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowCustomerCareWithBanType)(banType);
#endif
	}

	void Web::InGameBrowser::ShowForum()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowForum();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowForum)();
#endif
	}

	void Web::InGameBrowser::ShowTermsOfUse()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowTerms();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowTerms)();
#endif
	}

	void Web::InGameBrowser::ShowPrivacyPolicy()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowPrivacyPolicy();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowPrivacy)();
#endif
	}

	void Web::InGameBrowser::ShowNews()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowNews();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowNews)();
#endif
	}

	void Web::InGameBrowser::ShowEULA()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowEULA();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowEULA)();
#endif
	}

	void Web::InGameBrowser::ShowFacebookShare(bool openExternally)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowFaceBookShare(openExternally);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowFacebookShare)(openExternally);
#endif
	}

	void Web::InGameBrowser::ShowFacebookPage(bool openExternally)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowFaceBookPage(openExternally);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowFacebookPage)(openExternally);
#endif
	}

	void Web::InGameBrowser::GetFacebookShareLink(bool directLink, void(*facebookShareLinkCallback)(const char*))
	{
		auto wsLink = std::wstring(
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
			WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->GetLink(
			WCPToolkit::InGameBrowserLibrary::LinkCategory::FACEBOOKSHARE, "")
#elif defined(OS_WP8)
			NATIVE_CALL(nativeGetFacebookShareLink)()
#endif
		->Data());

		facebookShareLinkCallback(std::string(wsLink.begin(), wsLink.end()).c_str());
	}

	void Web::InGameBrowser::OpenURL(const std::string& sURL)
	{
		std::wstring wsURL = std::wstring(sURL.begin(), sURL.end());
		auto platURL = ref new Platform::String(wsURL.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->ShowInGameBrowserWithUrl(platURL);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeShowCustomLink)(platURL);
#endif
	}

	void Web::InGameBrowser::LaunchBrowserGameUpdate(bool unused)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto PlatURL = WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->GetLink(WCPToolkit::InGameBrowserLibrary::LinkCategory::GAMEUPDATE, "");
#elif defined(OS_WP8)
		auto PlatURL = NATIVE_CALL(nativeGetIGBGameUpdateURL)();
#endif
		std::wstring wsURL = PlatURL->Data();
		std::string sURL = std::string(wsURL.begin(),wsURL.end());
		WCPToolkit::Web::LaunchBrowser(sURL);
	}

	void Web::InGameBrowser::LaunchBrowserGameReview(bool unused)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto PlatURL = WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->GetLink(WCPToolkit::InGameBrowserLibrary::LinkCategory::GAMEREVIEW, "");
#elif defined(OS_WP8)
		auto PlatURL = NATIVE_CALL(nativeGetIGBGameReviewURL)();
#endif
		std::wstring wsURL = PlatURL->Data();
		std::string sURL = std::string(wsURL.begin(), wsURL.end());
		WCPToolkit::Web::LaunchBrowser(sURL);
	}

	void Web::InGameBrowser::SetGameLanguage(const std::string& language)
	{
		std::wstring wsLanguage = std::wstring(language.begin(), language.end());
		auto platLanguage = ref new Platform::String(wsLanguage.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetGameLanguage(platLanguage);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBGameLanguage)(platLanguage);
#endif
	}

	void Web::InGameBrowser::SetBaseURL(const std::string& baseURL)
	{
		std::wstring wsBaseURL = std::wstring(baseURL.begin(), baseURL.end());
		auto platBaseURL = ref new Platform::String(wsBaseURL.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetBaseUrl(platBaseURL);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBBaseURL)(platBaseURL);
#endif
	}

	void Web::InGameBrowser::SetUserAge(unsigned int userAge)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetUserAge((int)userAge);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBUserAge)((int)userAge);
#endif
	}

	void Web::InGameBrowser::SetUserGender(UserGender gender)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetUserGender((int)gender);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBUserGender)((int)gender);
#endif
	}

	void Web::InGameBrowser::SetAnonymousAccount(const std::string& anonymousAccount)
	{
		std::wstring wsAnonymous = std::wstring(anonymousAccount.begin(), anonymousAccount.end());
		auto platAnonymous = ref new Platform::String(wsAnonymous.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetAnonymousAccount(platAnonymous);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBAnonymousAccount)(platAnonymous);
#endif
	}
	
	void Web::InGameBrowser::SetFacebookID(const std::string& facebookID)
	{
		std::wstring wsFacebook = std::wstring(facebookID.begin(), facebookID.end());
		auto platFacebook = ref new Platform::String(wsFacebook.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetFacebookId(platFacebook);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBFacebookID)(platFacebook);
#endif
	}

	void Web::InGameBrowser::SetGLLiveAccount(const std::string& glliveAccount)
	{
		std::wstring wsGllive = std::wstring(glliveAccount.begin(), glliveAccount.end());
		auto platGllive = ref new Platform::String(wsGllive.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetGLiveAccount(platGllive);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBGlliveAccount)(platGllive);
#endif
	}

	void Web::InGameBrowser::SetXBoxAccount(const std::string& xboxAccount)
	{
		std::wstring wsXbox = std::wstring(xboxAccount.begin(), xboxAccount.end());
		auto platXbox = ref new Platform::String(wsXbox.c_str());
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetXboxID(platXbox);
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetIGBXboxAccount)(platXbox);
#endif
	}

	void Web::InGameBrowser::RefreshUnreadNewsNumber()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->RefreshUnreadNewsNumber();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeRefreshUnreadNewsNumber)();
#endif
	}

	int Web::InGameBrowser::GetUnreadNewsNumber()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		return WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->GetUnreadNewsNumber();
#elif defined(OS_WP8)
		return NATIVE_CALL(nativeGetUnreadNewsNumber)();
#endif
	}

	bool Web::InGameBrowser::IsDisplayed()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		return WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->IsOnScreen();
#elif defined(OS_WP8)
		return NATIVE_CALL(nativeIsIGBOnScreen)();
#endif
	}

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	void Web::InGameBrowser::SetParentPanel(Windows::UI::Xaml::Controls::Panel^ panel)
	{
		return WCPToolkit::InGameBrowserLibrary::InGameBrowser::GetInstance()->SetParentPanel(panel);
	}
#endif
};