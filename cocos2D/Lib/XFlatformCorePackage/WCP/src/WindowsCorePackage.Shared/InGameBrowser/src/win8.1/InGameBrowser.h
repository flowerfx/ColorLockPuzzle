#pragma once

#include "..\common\InitStructure.h"

namespace WCPToolkit
{
	namespace InGameBrowserLibrary
	{
		public delegate void UnreadNewsChangedCallback(int unreadNewsNumber);
		public delegate void DisplayStateChangedDelegete();
		//public delegate void InGameRedirectCallback(string arg);

		public enum class LinkCategory
		{
			NONE,
			URL,
			SUPPORT,
			BANNED,
			FORUM,
			TERMS,
			PRIVACY,
			NEWS,
			EULA,
			FACEBOOKSHARE,
			FACEBOOKFANPAGE,
			TWITTER,
			GAMEUPDATE,
			GAMEREVIEW
		};

		static Platform::String^ TXT_IGB_LANGUAGES[] = { L"EN", L"FR", L"DE", L"IT", L"SP", L"JP", L"KR", L"CN", L"BR", L"RU", L"TR", L"AR", L"TH", L"ID", L"VI", L"ZT" };

		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class InGameBrowser sealed
		{
		private:
			static InGameBrowser^ m_Instance;

			Windows::UI::Xaml::Controls::WebView^ mWebView;
			Windows::UI::Xaml::Controls::Image^ mBackgroundImage;
			Windows::UI::Xaml::Controls::Image^ mCloseImage;
			Windows::UI::Xaml::Controls::Image^ mBackImage;
			Windows::UI::Xaml::Controls::Image^ mForwardImage;
			Windows::UI::Xaml::Controls::Image^ mReloadImage;
			Windows::UI::Xaml::Controls::Panel^ mPanel;
			Windows::UI::Xaml::Controls::ProgressRing^ prog_ring;
			bool inited_navig = false;

			Platform::String^ INGAMEBROWSER_URL;
			Platform::String^ INGAMEBROWSER_POST_TEMPLATE;

			Platform::String^ REDIRECT_URL;
			Platform::String^ REDIRECT_PARAMS_TEMPLATE;

			Platform::String^ FACEBOOK_PARAMS_TEMPLATE;

			Platform::String^ CHECK_UNREAD_NEWS_NUMBER;
			Platform::String^ SAVE_NEWS_ID;
			Platform::String^ DISPLAY_NEWS;

			int languageIndex = 0;
			Platform::String^ anonymousAccount;
			Platform::String^ facebookID;
			Platform::String^ gliveAccount;
			Platform::String^ gameCode;
			Platform::String^ gameOperator;
			Platform::String^ gameVersion;

			Platform::String^ HDIDFV;
			LinkCategory CTG_TYPE;
			Platform::String^ clientID;
			Platform::String^ waid;
			Platform::String^ XboxID;
			int userAge = 0;
			Platform::String^ m_firmware;

			enum UserGender
			{
				GENDER_UNKNOWN,
				GENDER_MALE,
				GENDER_FEMALE,
			};

		private:
			UserGender userGender = UserGender::GENDER_UNKNOWN;

			bool m_IGB_initialized = false;
			bool m_isShowing = false;

			static int unreadNewsNumber;

			static Windows::UI::Core::CoreDispatcher^ s_dispatcher;

			UnreadNewsChangedCallback^ unreadCallback;
			//private InGameRedirectCallback redirectCallback = null;

		private:
			Platform::Array<Platform::String^>^ TXT_NET_ERROR;

			Platform::Array<Platform::String^>^ TXT_OK;

		public:
			InGameBrowser();

			event DisplayStateChangedDelegete^ Shown;
			event DisplayStateChangedDelegete^ Hidden;

			Platform::String^ solveTemplate(Platform::String^ template_value);
			void Init(WCPToolkit::InGameBrowserLibrary::InitParams^ initParams);

		private:
			Platform::String^ getUDID();

			Platform::String^ getDevice();

			Platform::String^ getCountry();

		public:

			void SetUnreadNewsChangedCallback(UnreadNewsChangedCallback^ callback);

			void SetGameLanguage(Platform::String^ language);

			void SetAnonymousAccount(Platform::String^ account);

			void SetFacebookId(Platform::String^ id);

			void SetGLiveAccount(Platform::String^ account);

			void SetGameCode(Platform::String^ code);

			void SetGameOperator(Platform::String^ op);

			void SetGameVersion(Platform::String^ version);

			void SetHDIDFV(Platform::String^ sHDIDFV);

			void SetWaid(Platform::String^ sWaid);

			void SetXboxID(Platform::String^ sXboxID);

			void SetBaseUrl(Platform::String^ url);

			void SetClientID(Platform::String^ id);

			void SetUserAge(int age);

			void SetUserGender(int gender);

			static InGameBrowser^ GetInstance();

			void SetParentPanel(Windows::UI::Xaml::Controls::Panel^ parent);

			void mPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

			void ShowCustomerCare();

			void ShowCustomerCareWithBanType(int ban_type);

			void ShowInGameBrowserWithUrl(Platform::String^ url);

			void ShowForum();

			void ShowTerms();

			void ShowNews();

			void ShowPrivacyPolicy();

			void ShowEULA();

			void ShowFaceBookShare(bool openExternally);

			void ShowFaceBookPage(bool openExternally);

			void CallShowOnUI(LinkCategory cat, Platform::String^ url);

			void OpenInExternalBrowser(LinkCategory cat, Platform::String^ extra);

			Platform::String^ GetUserGenderText();

			Platform::String^ GetLink(LinkCategory cat, Platform::String^ extraParam);

			bool HandleBackKey();

			int GetUnreadNewsNumber();

			bool IsOnScreen();

			void RefreshUnreadNewsNumber();

		protected:
			void injectJavaScript(Platform::String^ js);

			void OnControllerEvent(int crt_event, double value);

			void RefreshUnreadNewsNumberHelper();

			void RefreshUnreadNewsNumberInternal();

			void RefreshUnreadNewsNumberBase(Platform::String^ serverURL, bool bSaveNumber);

			void SaveJsonData(Windows::Data::Json::JsonObject^ json, bool saveNumber);

			void SaveLastNewsIndex(Platform::String^ lastNewsIndex);

			Platform::String^ GetLastNewsIndex();

			Platform::String^ postData(Platform::String^ url, Platform::String^ paramz);

			void Show();

			void Hide();

			void NavigateTo(LinkCategory ctg, Platform::String^ extraParam);

			void _debug(Platform::String^ message);

			void PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

			void PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

			void PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

			void PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

			void NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args);

			void NavigationCompleted(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args);
		};

		
	}
}