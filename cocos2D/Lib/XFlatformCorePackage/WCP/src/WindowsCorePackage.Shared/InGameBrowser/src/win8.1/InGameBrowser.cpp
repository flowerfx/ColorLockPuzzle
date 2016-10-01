#include "pch.h"

#include "WinShared.h"
#include "InGameBrowser.h"
#include "..\..\..\Utils\Encryptor.h"
#include "..\..\..\Utils\ZOrdering.h"

namespace WCPToolkit
{
	namespace InGameBrowserLibrary
	{
		Windows::UI::Core::CoreDispatcher^ InGameBrowser::s_dispatcher = nullptr;
		InGameBrowser^ InGameBrowser::m_Instance = nullptr;
		int InGameBrowser::unreadNewsNumber = -1;

		void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
		{
			if (from.empty())
				return;
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::wstring::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
			}
		}

		InGameBrowser::InGameBrowser()
		{
			mWebView = nullptr;
			mBackgroundImage = nullptr;
			mCloseImage = nullptr;
			mBackImage = nullptr;
			mForwardImage = nullptr;
			mReloadImage = nullptr;
			mPanel = nullptr;
			prog_ring = nullptr;
			inited_navig = false;

			INGAMEBROWSER_URL = L"";
			INGAMEBROWSER_POST_TEMPLATE = L"from=FROM&op=OPERATOR&country=COUNTRY&lg=LANG&udid=UDID&game_ver=VERSION&d=DEVICE&f=FIRMWARE&anonymous=ANONYMOUS_ACCOUNT&fbid=FACEBOOK_ID&gliveusername=GLIVE_USERNAME&xbox=XBOX_ID&os=win&hdidfv=HDIDFV&clientid=CLIENT_ID&waid=D_WAID&user_age=CURRENT_USER_AGE";

			REDIRECT_URL = L"";
			REDIRECT_PARAMS_TEMPLATE = L"from=FROM&op=OPERATOR&game_ver=VERSION&lg=LANG&country=COUNTRY&d=DEVICE&f=FIRMWARE&udid=UDID&hdidfv=HDIDFV&clientid=CLIENT_ID&waid=D_WAID&user_age=CURRENT_USER_AGE";

			FACEBOOK_PARAMS_TEMPLATE = L"from=FROM&op=OPERATOR&game_ver=VERSION&lg=LANG&country=COUNTRY&d=DEVICE&f=FIRMWARE&udid=UDID&hdidfv=HDIDFV";

			CHECK_UNREAD_NEWS_NUMBER = L"";
			SAVE_NEWS_ID = L"";
			DISPLAY_NEWS = L"";

			languageIndex = 0;
			anonymousAccount = L"";
			facebookID = L"";
			gliveAccount = L"";
			gameCode = L"";
			gameOperator = L"";
			gameVersion = L"";

			HDIDFV = L"";
			CTG_TYPE = LinkCategory::NONE;
			clientID = L"";
			waid = L"";
			XboxID = L"";
			userAge = 0;
			m_firmware = L"";

			userGender = UserGender::GENDER_UNKNOWN;

			m_IGB_initialized = false;
			m_isShowing = false;

			int unreadNewsNumber = -1;

			unreadCallback = nullptr;
			//private InGameRedirectCallback redirectCallback = null;


			TXT_NET_ERROR = ref new Platform::Array<Platform::String^>(16);

			TXT_NET_ERROR[0] = L"No internet connection available. Please make sure your device is connected to the internet.";
			TXT_NET_ERROR[1] = L"Aucune connexion Internet disponible. Veuillez vous assurer que votre appareil est connecté à Internet";
			TXT_NET_ERROR[2] = L"Keine Internetverbindung verfügbar. Versichere dich bitte, dass dein Gerät mit dem Internet verbunden ist.";
			TXT_NET_ERROR[3] = L"Nessuna connessione disponibile. Assicurati che il dispositivo sia connesso a internet.";
			TXT_NET_ERROR[4] = L"No hay ninguna conexión a internet disponible. Por favor, asegúrate de que tu dispositivo esté conectado a internet.";
			TXT_NET_ERROR[5] = L"インターネット接続を利用できません\nデバイスがインターネットに接続されているかご確認ください";
			TXT_NET_ERROR[6] = L"인터넷에 연결할 수 없습니다. 장치가 인터넷에 연결되어 있는지 확인하세요.";
			TXT_NET_ERROR[7] = L"无可用网络连接。请确保您的设备已连入网络。";
			TXT_NET_ERROR[8] = L"Conexão indisponível. Certifique-se de que seu aparelho esteja conectado à internet.";
			TXT_NET_ERROR[9] = L"Нет соединения с Интернетом. Пожалуйста, удостоверьтесь, что ваше устройство подключено к Интернету.";
			TXT_NET_ERROR[10] = L"İnternet bağlantısı yok. Lütfen cihazının internete bağlı olduğundan emin ol.";
			TXT_NET_ERROR[11] = L"لا يوجد اتصال بالإنترنت. يُرجى التحقق من أن جهازك متصل بالإنترنت.";
			TXT_NET_ERROR[12] = L"ไม่พบ|การเชื่อมต่อ|อินเทอร์เน็ต |กรุณา|ตรวจสอบ|ว่า|เครื่อง|ของคุณ|ได้|เชื่อมต่อ|กับ|อินเทอร์เน็ต";
			TXT_NET_ERROR[13] = L"Koneksi Internet saat ini tidak tersedia. Pastikan perangkatmu sudah terhubung dengan Internet.";
			TXT_NET_ERROR[14] = L"Không có kết nối Internet. Xin đảm bảo thiết bị của bạn đã kết nối Internet.";
			TXT_NET_ERROR[15] = L"无可用网络连接。请确保您的设备已连入网络。";


			TXT_OK = ref new Platform::Array<Platform::String^>(16);

			TXT_OK[0] = L"OK";
			TXT_OK[1] = L"OK";
			TXT_OK[2] = L"OK";
			TXT_OK[3] = L"OK";
			TXT_OK[4] = L"Acep.";
			TXT_OK[5] = L"OK";
			TXT_OK[6] = L"OK";
			TXT_OK[7] = L"OK";
			TXT_OK[8] = L"OK";
			TXT_OK[9] = L"OK";
			TXT_OK[10] = L"OK";
			TXT_OK[11] = L"موافق";//AR
			TXT_OK[12] = L"ตกลง";//TH
			TXT_OK[13] = L"OK";//ID
			TXT_OK[14] = L"OK";//VI
			TXT_OK[15] = L"OK";//ZT
		}

		Platform::String^ InGameBrowser::solveTemplate(Platform::String^ template_value)
		{
			std::wstring result(L"");


			if (template_value != nullptr)
			{
				if (languageIndex < 0 || languageIndex >= sizeof(TXT_IGB_LANGUAGES) / sizeof(*TXT_IGB_LANGUAGES))
					languageIndex = 0;

				std::wstring temp_result(template_value->Data());

				ReplaceAll(temp_result, L"FROM", gameCode->Data());
				ReplaceAll(temp_result, L"OPERATOR", gameOperator->Data());
				ReplaceAll(temp_result, L"UDID", getUDID()->Data());
				ReplaceAll(temp_result, L"COUNTRY", getCountry()->Data());
				ReplaceAll(temp_result, L"LANG", TXT_IGB_LANGUAGES[languageIndex]->Data());
				ReplaceAll(temp_result, L"VERSION", gameVersion->Data());
				ReplaceAll(temp_result, L"DEVICE", getDevice()->Data());
				ReplaceAll(temp_result, L"FIRMWARE", m_firmware->Data());
				ReplaceAll(temp_result, L"ANONYMOUS_ACCOUNT", anonymousAccount->Data());
				ReplaceAll(temp_result, L"FACEBOOK_ID", facebookID->Data());
				ReplaceAll(temp_result, L"GLIVE_USERNAME", gliveAccount->Data());
				ReplaceAll(temp_result, L"XBOX_ID", XboxID->Data());
				ReplaceAll(temp_result, L"HDIDFV", HDIDFV->Data());
				ReplaceAll(temp_result, L"CLIENT_ID", clientID->Data());
				ReplaceAll(temp_result, L"D_WAID", waid->Data());
				ReplaceAll(temp_result, L"CURRENT_USER_AGE", std::to_wstring(userAge));
				ReplaceAll(temp_result, L" ", L"");

				result = temp_result;
			}

			return ref new Platform::String(result.c_str());
		}

		void InGameBrowser::Init(WCPToolkit::InGameBrowserLibrary::InitParams^ initParams)
		{
			m_IGB_initialized = initParams->CheckParams();
			if (m_IGB_initialized)
			{
				SetGameLanguage(initParams->Language);
				SetAnonymousAccount(initParams->AnonymousAccount);
				SetFacebookId(initParams->FacebookId);
				SetGLiveAccount(initParams->GLiveAccount);
				SetGameCode(initParams->GameCode);
				SetGameOperator(initParams->GameOperator);
				SetGameVersion(initParams->GameVersion);
				SetHDIDFV(initParams->HDIDFV);
				SetBaseUrl(initParams->BaseUrl);
				SetClientID(initParams->ClientID);
				SetWaid(initParams->WAID);
				SetXboxID(initParams->XboxID);
				SetUserAge(initParams->UserAge);
				m_firmware = initParams->OsVersion;
			}
		}

		Platform::String^ InGameBrowser::getUDID()
		{
#if defined(OS_WP8) || defined(OS_W8) || defined(OS_W81)
			Platform::String^ UDID = L"";
			auto token = Windows::System::Profile::HardwareIdentification::GetPackageSpecificToken(nullptr);

			if (token != nullptr && token->Id != nullptr)
			{
				auto alg = Windows::Security::Cryptography::Core::HashAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::HashAlgorithmNames::Md5);
				Windows::Security::Cryptography::Core::CryptographicHash^ hash = alg->CreateHash();
				hash->Append(token->Id);
				UDID = Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(hash->GetValueAndReset());
			}
			//new Windows.UI.Popups.MessageDialog("HardwareID: \n" + UDID).ShowAsync();
			return UDID;
#endif
			return L"";
		}

		Platform::String^ InGameBrowser::getDevice()
		{
			Platform::String^ device = L"";
			auto info = ref new Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation();

			device = info->SystemProductName;
			if (device == nullptr || device == "")
			{
				device = L"Windows%208%20device";
			}
			return device;
		}

		Platform::String^ InGameBrowser::getCountry()
		{
			auto region = ref new Windows::Globalization::GeographicRegion();
			return region->CodeTwoLetter;
		}

		void InGameBrowser::SetUnreadNewsChangedCallback(UnreadNewsChangedCallback^ callback)
		{
			unreadCallback = callback;
		}

		void InGameBrowser::SetGameLanguage(Platform::String^ language)
		{
			languageIndex = -1;
			for (int i = 0; i < sizeof(TXT_IGB_LANGUAGES) / sizeof(*TXT_IGB_LANGUAGES); ++i)
			{
				if (TXT_IGB_LANGUAGES[i]->Equals(language))
				{
					languageIndex = i;
					return;
				}
			}
		}

		void InGameBrowser::SetAnonymousAccount(Platform::String^ account)
		{
			anonymousAccount = account;
		}

		void InGameBrowser::SetFacebookId(Platform::String^ id)
		{
			facebookID = id;
		}

		void InGameBrowser::SetGLiveAccount(Platform::String^ account)
		{
			gliveAccount = account;
		}

		void InGameBrowser::SetGameCode(Platform::String^ code)
		{
			gameCode = code;
		}

		void InGameBrowser::SetGameOperator(Platform::String^ op)
		{
			gameOperator = op;
		}

		void InGameBrowser::SetGameVersion(Platform::String^ version)
		{
			gameVersion = version;
		}

		void InGameBrowser::SetHDIDFV(Platform::String^ sHDIDFV)
		{
			HDIDFV = sHDIDFV;
		}

		void InGameBrowser::SetWaid(Platform::String^ sWaid)
		{
			waid = sWaid;
		}

		void InGameBrowser::SetXboxID(Platform::String^ sXboxID)
		{
			XboxID = sXboxID;
		}

		void InGameBrowser::SetBaseUrl(Platform::String^ url)
		{
			if (url != nullptr && !url->IsEmpty())
			{ 
				if (url->End()[-1] != L'/')
					url += L"/";

				INGAMEBROWSER_URL = url + L"redir/ingamebrowser.php";
				REDIRECT_URL = url + L"redir/";
				CHECK_UNREAD_NEWS_NUMBER = url + L"redir/ingamenews.php?action=checkNews&last-id=LAST_ID";
				SAVE_NEWS_ID = url + L"redir/ingamenews.php?action=saveNews&last-id=LAST_ID";
				DISPLAY_NEWS = url + L"redir/ingamenews.php?action=displayNews";
			}
		}

		void InGameBrowser::SetClientID(Platform::String^ id)
		{
			if (id != nullptr)
				clientID = id;
		}

		void InGameBrowser::SetUserAge(int age)
		{
			if (age < 13)
				userAge = 0; //FOR COPPA AND OFT: DON'T SEND REAL USER AGE IF HE IS UNDER 13
			else
				userAge = age;
		}

		void InGameBrowser::SetUserGender(int gender)
		{
			userGender = (UserGender)gender;
		}

		InGameBrowser^ InGameBrowser::GetInstance()
		{
			if (m_Instance == nullptr)
				m_Instance = ref new InGameBrowser();

			return m_Instance;
		}

		void InGameBrowser::SetParentPanel(Windows::UI::Xaml::Controls::Panel^ parent)
		{
			mPanel = parent;

			mPanel->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::mPanel_SizeChanged);
			s_dispatcher = parent->Dispatcher;
		}

		void InGameBrowser::mPanel_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
		{
			double header_height = (0.1 * mPanel->ActualHeight < 50) ? mPanel->ActualHeight * 0.1 : 50;

			if (mBackgroundImage != nullptr && mWebView != nullptr)
			{
				mBackgroundImage->Width = mPanel->ActualWidth;
				mBackgroundImage->Height = mPanel->ActualHeight;
				mWebView->Width = mPanel->ActualWidth;
				mWebView->Height = mPanel->ActualHeight - header_height * 2;
			}
		}

		void InGameBrowser::ShowCustomerCare()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::SUPPORT, nullptr);
		}

		void InGameBrowser::ShowCustomerCareWithBanType(int ban_type)
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::BANNED, ref new Platform::String(std::to_wstring(ban_type).c_str()));
		}

		void InGameBrowser::ShowInGameBrowserWithUrl(Platform::String^ url)
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::URL, url);
		}

		void InGameBrowser::ShowForum()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::FORUM, nullptr);
		}

		void InGameBrowser::ShowTerms()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::TERMS, nullptr);
		}

		void InGameBrowser::ShowNews()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::NEWS, nullptr);
		}

		void InGameBrowser::ShowPrivacyPolicy()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::PRIVACY, nullptr);
		}

		void InGameBrowser::ShowEULA()
		{
			if (!m_IGB_initialized)
				return;
			CallShowOnUI(LinkCategory::EULA, nullptr);
		}

		void InGameBrowser::ShowFaceBookShare(bool openExternally)
		{
			if (!m_IGB_initialized)
				return;
			if (openExternally)
				OpenInExternalBrowser(LinkCategory::FACEBOOKSHARE, nullptr);
			else
				CallShowOnUI(LinkCategory::FACEBOOKSHARE, nullptr);
		}

		void InGameBrowser::ShowFaceBookPage(bool openExternally)
		{
			if (!m_IGB_initialized)
				return;
			if (openExternally)
				OpenInExternalBrowser(LinkCategory::FACEBOOKFANPAGE, nullptr);
			else
				CallShowOnUI(LinkCategory::FACEBOOKFANPAGE, nullptr);
		}

		void InGameBrowser::CallShowOnUI(LinkCategory cat, Platform::String^ url)
		{
			if (url == nullptr)
			{
				url = L"";
			}

			try
			{
				if (s_dispatcher == nullptr)
					s_dispatcher = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher;
				if (s_dispatcher->HasThreadAccess)
				{
					Show();
					NavigateTo(cat, url);
				}
				else
					s_dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
					ref new Windows::UI::Core::DispatchedHandler([=]()
				{
					Show();
					NavigateTo(cat, url);
				}));
			}
			catch (Platform::Exception^ e)
			{
				_debug(" [CallShowOnUI] exception: " + e->Message);
			}
		}

		void InGameBrowser::OpenInExternalBrowser(LinkCategory cat, Platform::String^ extra)
		{
			if (extra == nullptr)
			{
				extra = L"";
			}
			auto uri = ref new Windows::Foundation::Uri(GetLink(cat, extra));

			if (s_dispatcher == nullptr)
				s_dispatcher = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher;
			if (s_dispatcher->HasThreadAccess)
			{
				Windows::System::Launcher::LaunchUriAsync(uri);
			}
			else
				s_dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
				ref new Windows::UI::Core::DispatchedHandler([=]()
			{
				Windows::System::Launcher::LaunchUriAsync(uri);
			}));
		}

		Platform::String^ InGameBrowser::GetUserGenderText()
		{
			switch (userGender)
			{
			case UserGender::GENDER_MALE:
				return L"&gender=male";
			case UserGender::GENDER_FEMALE:
				return L"&gender=female";
			}

			return L"";
		}

		Platform::String^ InGameBrowser::GetLink(LinkCategory cat, Platform::String^ extraParam)
		{
			Platform::String^ url = L"";
			Platform::String^ toEncript = L"";


			switch (cat)
			{
			case LinkCategory::SUPPORT:
				url = INGAMEBROWSER_URL + "?ctg=SUPPORT";
				toEncript = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::BANNED:
				url = INGAMEBROWSER_URL + "?ctg=BANNED";
				toEncript = L"extra_14=" + extraParam + L"&" + solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::FORUM:
				url = REDIRECT_URL + "?ctg=FORUM";
				toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::TERMS:
				url = REDIRECT_URL + "?ctg=TERMS";
				toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::PRIVACY:
				url = REDIRECT_URL + "?ctg=PRIVACY";
				toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::NEWS:
				url = DISPLAY_NEWS;
				toEncript = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::EULA:
				url = REDIRECT_URL + "?ctg=EULA";
				toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::FACEBOOKSHARE:
				url = REDIRECT_URL + "?ctg=FBOOK";
				toEncript = L"game=" + gameCode + L"&" + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
				break;
			case LinkCategory::FACEBOOKFANPAGE:
				url = REDIRECT_URL + "?t=facebook";
				toEncript = "game=" + gameCode + L"&" + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
				break;
			case LinkCategory::TWITTER:
				url = REDIRECT_URL + "?t=twitter";
				toEncript = L"game=" + gameCode + L"&" + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
				break;
			case LinkCategory::GAMEUPDATE:
				url = REDIRECT_URL + "?ctg=UPDATE";
				toEncript = L"game=" + gameCode + L"&" + solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			case LinkCategory::GAMEREVIEW:
				url = REDIRECT_URL + "?ctg=GAME_REVIEW";
				toEncript = L"game=" + gameCode + L"&" + solveTemplate(REDIRECT_PARAMS_TEMPLATE);
				toEncript += GetUserGenderText();
				break;
			default:
				url = extraParam;
				break;
			}

			if (!(toEncript == nullptr) && !toEncript->IsEmpty())
			{
				url = url + L"&data=" + WCPToolkit::Utils::Encryption::Encryptor::Encrypt(toEncript, WCPToolkit::Utils::Encryption::EncryptionType::EET_AES) + L"&enc=4";
			}

			return url;
		}

		bool InGameBrowser::HandleBackKey()
		{
			if (mWebView != nullptr && m_isShowing)
			{
				_debug("escape key triggered");
				if (CTG_TYPE == LinkCategory::NEWS)
					RefreshUnreadNewsNumberInternal();
				Hide();
				return true;
			}
			return false;
		}

		void InGameBrowser::injectJavaScript(Platform::String^ js)
		{
			try
			{
				WCPToolkit::Utils::RunOnUIThread([=]()
				{
					auto arg = ref new Platform::Collections::Vector<Platform::String^>();
					arg->Append(js);
					mWebView->InvokeScriptAsync(ref new Platform::String(L"eval"), arg);
				});
			}
			catch (Platform::Exception^ e)
			{
				_debug(" [injectJavaScript] exception: " + e->Message);
			}
		}

		void InGameBrowser::OnControllerEvent(int crt_event, double value)
		{
			switch ((WCPToolkit::Controllers::ControllerEvents)crt_event)
			{
			case WCPToolkit::Controllers::ControllerEvents::UNDEFINED:
				break;
			case WCPToolkit::Controllers::ControllerEvents::LeftTrigger:
				break;
			case WCPToolkit::Controllers::ControllerEvents::RightTrigger:
				break;
			case WCPToolkit::Controllers::ControllerEvents::LeftStickX:
				break;
			case WCPToolkit::Controllers::ControllerEvents::LeftStickY:
				break;
			case WCPToolkit::Controllers::ControllerEvents::RightStickX:
				break;
			case WCPToolkit::Controllers::ControllerEvents::RightStickY:
				break;
			case WCPToolkit::Controllers::ControllerEvents::DpadEventUp:
				if (m_isShowing && (int)value == 1)
					injectJavaScript(ref new Platform::String(L"window.scrollBy(0, -100)"));
				break;
			case WCPToolkit::Controllers::ControllerEvents::DpadEventDown:
				if (m_isShowing && (int)value == 1)
					injectJavaScript(ref new Platform::String(L"window.scrollBy(0, 100)"));
				break;
			case WCPToolkit::Controllers::ControllerEvents::DpadEventLeft:
				break;
			case WCPToolkit::Controllers::ControllerEvents::DpadEventRight:
				break;
			case WCPToolkit::Controllers::ControllerEvents::LeftBumper:
				if (m_isShowing)
					WCPToolkit::Utils::RunOnUIThread([=]()
				{
					if (mWebView != nullptr && mWebView->CanGoBack)
						mWebView->GoBack();
				});
				break;
			case WCPToolkit::Controllers::ControllerEvents::RightBumper:
			case WCPToolkit::Controllers::ControllerEvents::ButtonY:
				if (m_isShowing)
					WCPToolkit::Utils::RunOnUIThread([=]()
				{
					if (mWebView != nullptr && mWebView->CanGoForward)
						mWebView->GoForward();
				});
				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonA:
				if (m_isShowing)
					WCPToolkit::Utils::RunOnUIThread([=]()
				{
					if (mWebView != nullptr)
						mWebView->Refresh();
				});
				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonX:
				if (m_isShowing)
					WCPToolkit::Utils::RunOnUIThread([=]()
				{
					if (mWebView != nullptr && mWebView->CanGoBack)
						mWebView->GoBack();
				});
				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonB:
				if (m_isShowing && (int)value == 0)
					WCPToolkit::Utils::RunOnUIThread([=]()
				{
					HandleBackKey();
				});

				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonStart:
				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonSelect:
				break;
			case WCPToolkit::Controllers::ControllerEvents::ButtonBack:
				if ((int)value == 0)
					WCPToolkit::Utils::RunOnUIThread([=]() { HandleBackKey(); });
				break;
			case WCPToolkit::Controllers::ControllerEvents::LeftStickButton:
				break;
			case WCPToolkit::Controllers::ControllerEvents::RightStickButton:
				break;
			default:
				break;
			}
		}

		bool InGameBrowser::IsOnScreen()
		{
			return m_isShowing;
		}

		void InGameBrowser::RefreshUnreadNewsNumber()
		{
			RefreshUnreadNewsNumberHelper();
		}

		void InGameBrowser::RefreshUnreadNewsNumberBase(Platform::String^ serverURL, bool bSaveNumber)
		{
			Platform::String^ lastNewsIndex = GetLastNewsIndex();
			std::wstring serverUrl(serverURL->Data());
			ReplaceAll(serverUrl, std::wstring(L"LAST_ID"), std::wstring(lastNewsIndex->Data()));
			Platform::String^ data = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);

			Platform::String^ pURL = ref new Platform::String(serverUrl.data());

			std::async(std::launch::async, [this, bSaveNumber, pURL, data]()
			{
				try
				{
					Platform::String^ response = postData(pURL, data);
					_debug(L"response received: " + response);
					Windows::Data::Json::JsonObject^ json = Windows::Data::Json::JsonObject::Parse(response);
					SaveJsonData(json, bSaveNumber);
				}
				catch (Platform::Exception^ e)
				{

				}
			}
			);
		}

		void InGameBrowser::RefreshUnreadNewsNumberHelper()
		{
			_debug(L"RefreshUnreadNewsNumberHelper()");
			RefreshUnreadNewsNumberBase(CHECK_UNREAD_NEWS_NUMBER, false);
		}

		void InGameBrowser::RefreshUnreadNewsNumberInternal()
		{
			_debug(L"RefreshUnreadNewsNumberInternal()");
			RefreshUnreadNewsNumberBase(SAVE_NEWS_ID, true);
		}

		void InGameBrowser::SaveJsonData(Windows::Data::Json::JsonObject^ json, bool saveNumber)
		{
			bool success = false;
			if (json->HasKey(L"success"))
			{
				Windows::Data::Json::JsonValueType type = json->GetNamedValue(L"success")->ValueType;

				if (type == Windows::Data::Json::JsonValueType::Boolean)
					success = json->GetNamedBoolean(L"success", false);
				else if (type == Windows::Data::Json::JsonValueType::String)
				{
					success = true;
					if (_wcsicmp(json->GetNamedString(L"success", L"false")->Data(), L"false") == 0)
					{
						success = false;
					}
				}
			}

			if (success)
			{
				if (saveNumber)
				{
					Platform::String^ lastNewsIndex = L"-1";
					if (json->HasKey(L"current-id"))
					{
						Windows::Data::Json::JsonValueType type = json->GetNamedValue(L"success")->ValueType;

						if (type == Windows::Data::Json::JsonValueType::String)
							lastNewsIndex = json->GetNamedString(L"current-id", L"-1");
						else if (type == Windows::Data::Json::JsonValueType::Number || type == Windows::Data::Json::JsonValueType::Boolean)
							lastNewsIndex = "" + ((int)json->GetNamedNumber(L"current-id", -1));
					}

					SaveLastNewsIndex(lastNewsIndex);
				}

				unreadNewsNumber = -1;
				if (json->HasKey(L"unread"))
				{
					Windows::Data::Json::JsonValueType type = json->GetNamedValue(L"success")->ValueType;

					if (type == Windows::Data::Json::JsonValueType::Number || type == Windows::Data::Json::JsonValueType::Boolean)
						unreadNewsNumber = (int)json->GetNamedNumber(L"unread", -1);
					else if (type == Windows::Data::Json::JsonValueType::String)
						unreadNewsNumber = (int)(_wtof(json->GetNamedString(L"unread", "-1")->Data()));
				}

				if (unreadCallback != nullptr)
				{
					unreadCallback(unreadNewsNumber);
				}
			}
		}

		int InGameBrowser::GetUnreadNewsNumber()
		{
			return unreadNewsNumber;
		}

		void InGameBrowser::SaveLastNewsIndex(Platform::String^ lastNewsIndex)
		{
			auto settings = Windows::Storage::ApplicationData::Current->LocalSettings->Values;
			if (settings->HasKey(L"InGameBrowser_lastNewsIndex"))
				settings->Remove(L"InGameBrowser_lastNewsIndex");

			settings->Insert(L"InGameBrowser_lastNewsIndex", lastNewsIndex);
		}

		Platform::String^ InGameBrowser::GetLastNewsIndex()
		{
			auto settings = Windows::Storage::ApplicationData::Current->LocalSettings->Values;
			if (settings->HasKey(L"InGameBrowser_lastNewsIndex"))
				return settings->Lookup(L"InGameBrowser_lastNewsIndex")->ToString();
			else
				return L"-1";
		}

		Platform::String^ InGameBrowser::postData(Platform::String^ url, Platform::String^ paramz)
		{
			try 
			{
				_debug("postData making post at: " + url + "\n" + paramz);
				Windows::Web::Http::HttpClient^ client = ref new Windows::Web::Http::HttpClient();

				Windows::Web::Http::IHttpContent^ content = ref new Windows::Web::Http::HttpStringContent(paramz, Windows::Storage::Streams::UnicodeEncoding::Utf8);
				content->Headers->ContentType = Windows::Web::Http::Headers::HttpMediaTypeHeaderValue::Parse(L"application/x-www-form-urlencoded");
				auto Task_PostAsync = client->PostAsync(ref new Windows::Foundation::Uri(url), content);

				while (Task_PostAsync->Status == Windows::Foundation::AsyncStatus::Started)
				{
					std::this_thread::yield();
				}

				if (Task_PostAsync->Status != Windows::Foundation::AsyncStatus::Completed)
				{
					return "";
				}

				Windows::Web::Http::HttpResponseMessage^ response = Task_PostAsync->GetResults();

				if (response->IsSuccessStatusCode)
				{
					auto Task_ReadAsStringAsync = response->Content->ReadAsStringAsync();

					while (Task_ReadAsStringAsync->Status != Windows::Foundation::AsyncStatus::Completed)
					{
						std::this_thread::yield();
					}

					Platform::String^ responseString = Task_ReadAsStringAsync->GetResults();

					delete response;

					//_debug("postData response: " + responseString);
					return responseString;
				}
			}
			catch (Platform::Exception^ ex)
			{

			}

			return "";
		}

		void InGameBrowser::Show()
		{
			if (m_isShowing)
				return;
			m_isShowing = true;

			double header_height = (0.1 * mPanel->ActualHeight < 50) ? mPanel->ActualHeight * 0.1 : 50;

			mBackgroundImage = ref new Windows::UI::Xaml::Controls::Image();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mBackgroundImage, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			mBackgroundImage->Source = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Windows::Foundation::Uri("ms-appx:/WindowsCorePackage/igb_background.png"));
			mBackgroundImage->Width = mPanel->ActualWidth;
			mBackgroundImage->Height = mPanel->ActualHeight;
			mBackgroundImage->Stretch = Windows::UI::Xaml::Media::Stretch::Fill;

			mPanel->Children->Append(mBackgroundImage);

			mCloseImage = ref new Windows::UI::Xaml::Controls::Image();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mCloseImage, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			mCloseImage->Margin = Windows::UI::Xaml::Thickness(10, 0, 0, 0);
			mCloseImage->Source = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Windows::Foundation::Uri("ms-appx:/WindowsCorePackage/igb_cancel.png"));
			mCloseImage->Width = header_height;
			mCloseImage->Height = header_height;
			mCloseImage->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
			mCloseImage->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;

			mCloseImage->PointerPressed += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerPressed);
			mCloseImage->PointerReleased += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerReleased);
			mCloseImage->PointerExited += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerExited);
			mCloseImage->PointerEntered += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerEntered);

			mPanel->Children->Append(mCloseImage);

			mBackImage = ref new Windows::UI::Xaml::Controls::Image();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mBackImage, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			mBackImage->Margin = Windows::UI::Xaml::Thickness(10, 0, 0, 0);
			mBackImage->Source = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Windows::Foundation::Uri("ms-appx:/WindowsCorePackage/igb_back.png"));
			mBackImage->Width = header_height;
			mBackImage->Height = header_height;
			mBackImage->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
			mBackImage->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;

			mBackImage->PointerPressed += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerPressed);
			mBackImage->PointerReleased += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerReleased);
			mBackImage->PointerExited += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerExited);
			mBackImage->PointerEntered += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerEntered);

			mPanel->Children->Append(mBackImage);

			mForwardImage = ref new Windows::UI::Xaml::Controls::Image();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mForwardImage, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			mForwardImage->Margin = Windows::UI::Xaml::Thickness(header_height + 40, 0, 0, 0);
			mForwardImage->Source = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Windows::Foundation::Uri("ms-appx:/WindowsCorePackage/igb_forward.png"));
			mForwardImage->Width = header_height;
			mForwardImage->Height = header_height;
			mForwardImage->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
			mForwardImage->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;

			mForwardImage->PointerPressed += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerPressed);
			mForwardImage->PointerReleased += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerReleased);
			mForwardImage->PointerExited += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerExited);
			mForwardImage->PointerEntered += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerEntered);

			mPanel->Children->Append(mForwardImage);

			mReloadImage = ref new Windows::UI::Xaml::Controls::Image();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mReloadImage, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			mReloadImage->Margin = Windows::UI::Xaml::Thickness(0, 0, 10, 0);
			mReloadImage->Source = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Windows::Foundation::Uri("ms-appx:/WindowsCorePackage/igb_refresh.png"));
			mReloadImage->Width = header_height;
			mReloadImage->Height = header_height;
			mReloadImage->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Right;
			mReloadImage->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;

			mReloadImage->PointerPressed += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerPressed);
			mReloadImage->PointerReleased += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerReleased);
			mReloadImage->PointerExited += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerExited);
			mReloadImage->PointerEntered += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::PointerEntered);

			mPanel->Children->Append(mReloadImage);

			if (mWebView == nullptr)
			{
				mWebView = ref new Windows::UI::Xaml::Controls::WebView();
				WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(mWebView, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			}
			mWebView->Width = mPanel->ActualWidth;
			mWebView->Height = mPanel->ActualHeight - header_height * 2;
			if (!inited_navig)
			{
				mWebView->NavigationCompleted += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView^, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^>(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::NavigationCompleted);
				mWebView->NavigationStarting += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::WebView^, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^>(this, &WCPToolkit::InGameBrowserLibrary::InGameBrowser::NavigationStarting);
				inited_navig = true;
			}

			mPanel->Children->Append(mWebView);

			prog_ring = ref new Windows::UI::Xaml::Controls::ProgressRing();
			WCPToolkit::Utils::ZOrdering::ZOrder::SetZPriority(prog_ring, WCPToolkit::Utils::ZOrdering::ZPriority::INTERSTITIAL_HIGH);
			prog_ring->Width = header_height * 2.5;
			prog_ring->Height = header_height * 2.5;
			prog_ring->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
			prog_ring->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

			mPanel->Children->Append(prog_ring);

			Shown();
		}

		void InGameBrowser::Hide()
		{
			m_isShowing = false;
			CTG_TYPE = LinkCategory::NONE;

			mWebView->NavigateToString(L"");

			unsigned int index;
			if (mPanel->Children->IndexOf(mWebView, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(mBackgroundImage, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(mBackImage, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(mForwardImage, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(mReloadImage, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(mCloseImage, &index))
				mPanel->Children->RemoveAt(index);
			if (mPanel->Children->IndexOf(prog_ring, &index))
				mPanel->Children->RemoveAt(index);

			mBackgroundImage = nullptr;
			mBackImage = nullptr;
			mForwardImage = nullptr;
			mReloadImage = nullptr;
			mCloseImage = nullptr;
			prog_ring = nullptr;

			Hidden();
		}

		void InGameBrowser::NavigateTo(LinkCategory ctg, Platform::String^ extraParam)
		{
			try
			{
				CTG_TYPE = ctg;
				Platform::String^ url = GetLink(ctg, extraParam);
				_debug(ref new Platform::String(L"Navigating to [") + ctg.ToString() + L"] " + url);
			mWebView->Navigate(ref new Windows::Foundation::Uri(url));
			}
			catch (...) {}
		}

		void InGameBrowser::_debug(Platform::String^ message)
		{
			OutputDebugStringW(L"[InGameBrowser]: ");
			OutputDebugStringW(message->Data());
			OutputDebugStringW(L"\n");
		}

		void InGameBrowser::PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
		{
			((Windows::UI::Xaml::Controls::Image^)sender)->Opacity = 0.9;
		}

		void InGameBrowser::PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
		{
			((Windows::UI::Xaml::Controls::Image^)sender)->Opacity = 1;
		}

		void InGameBrowser::PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
		{
			((Windows::UI::Xaml::Controls::Image^)sender)->Opacity = 1;

			if (sender->Equals(mCloseImage))
			{
				if (CTG_TYPE == LinkCategory::NEWS)
					RefreshUnreadNewsNumberInternal();
				Hide();
				return;
			}
			if (sender->Equals(mBackImage))
			{
				if (mWebView && mWebView->CanGoBack)
					mWebView->GoBack();
				return;
			}
			if (sender->Equals(mForwardImage))
			{
				if (mWebView && mWebView->CanGoForward)
					mWebView->GoForward();
				return;
			}
			if (sender->Equals(mReloadImage))
			{
				if (mWebView != nullptr)
					mWebView->Refresh();
				return;
			}
		}

		void InGameBrowser::PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
		{
			((Windows::UI::Xaml::Controls::Image^)sender)->Opacity = 0.6;
		}

		void InGameBrowser::NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args)
		{
			if (args != nullptr && args->Uri != nullptr)
				_debug(L"Navigation Started: " + args->Uri->ToString());

			if (prog_ring != nullptr)
				prog_ring->IsActive = true;
		}

		void InGameBrowser::NavigationCompleted(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args)
		{
			if (args != nullptr && args->Uri != nullptr)
			{
				if (args->IsSuccess)
				{
					_debug("Navigation Completed: " + args->Uri->ToString());
				}
				else
				{
					_debug("Navigation Failed: " + args->Uri->ToString());
				}
			}

			if (prog_ring != nullptr)
				prog_ring->IsActive = false;
		}
	}
}