#include "pch.h"

#include "WinSharedManaged.h"

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
	void WCPToolkitWinPhone80CppCallbacks::nativeExitApplication()
	{
		ExitApplication();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeShowVirtualKeyboard(bool bTextBoxVisible, Platform::String^ sInitialText, int nMaxChars, int nType, int nLeft, int nTop, int nWidth, int nHeight, int nTextAlignment)
	{
		ShowVirtualKeyboard(bTextBoxVisible, sInitialText, nMaxChars, nType, nLeft, nTop, nWidth, nHeight, nTextAlignment);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeHideVirtualKeyboard()
	{
		HideVirtualKeyboard();
	}

	bool WCPToolkitWinPhone80CppCallbacks::nativeBRIDGEVKOnKeyPressed(int nKeyCode)
	{
		return Utils::VirtualKeyboard::GetInstance()->OnKeyPressed(nKeyCode);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeBRIDGEVKOnTextChanged(Platform::String^ sNewText)
	{
		Utils::VirtualKeyboard::GetInstance()->OnTextChanged(WCPToolkit::Utils::PlatformStr2Wstr(sNewText));
	}
}
#endif

namespace WCPToolkit
{
	Windows::UI::Core::CoreDispatcher^ UIDispatcher = nullptr;

#pragma region ParentPanel

#if defined(OS_W8)
	Windows::UI::Xaml::Controls::Panel^ s_pParentPanel = nullptr;

	void Utils::SetParentPanel(Windows::UI::Xaml::Controls::Panel^ pPanel)
	{
		s_pParentPanel = pPanel;
	}

	Windows::UI::Xaml::Controls::Panel^ Utils::GetParentPanel()
	{
		return s_pParentPanel;
	}
#elif defined(OS_WP8)
#endif

#pragma endregion ParentPanel

#pragma region VirtualKeyboard

	Utils::VirtualKeyboard* Utils::VirtualKeyboard::m_Instance = NULL;
	std::wstring Utils::VirtualKeyboard::m_sKeyboardString;
	bool Utils::VirtualKeyboard::m_bVisible = false;
	Utils::VKOnKeyPressedCallback Utils::VirtualKeyboard::m_pVKOnKeyPressedCallback = nullptr;
	Utils::VKOnTextChangedCallback Utils::VirtualKeyboard::m_pVKOnTextChangedCallback = nullptr;

	void Utils::VirtualKeyboard::RegisterVKOnKeyPressedCallback(Utils::VKOnKeyPressedCallback callback)
	{
		if (Utils::VirtualKeyboard::m_pVKOnKeyPressedCallback == NULL)
		{
			Utils::VirtualKeyboard::m_pVKOnKeyPressedCallback = callback;
		}
		else
		{
			//nop
		}
	}
	void Utils::VirtualKeyboard::RegisterVKOnTextChangedCallback(Utils::VKOnTextChangedCallback callback)
	{
		if (Utils::VirtualKeyboard::m_pVKOnTextChangedCallback == NULL)
		{
			Utils::VirtualKeyboard::m_pVKOnTextChangedCallback = callback;
		}
		else
		{
			//nop
		}
	}

	Utils::VirtualKeyboard* Utils::VirtualKeyboard::GetInstance()
	{
		if (!Utils::VirtualKeyboard::m_Instance)
			m_Instance = new VirtualKeyboard();

		return Utils::VirtualKeyboard::m_Instance;
	};
	Utils::VirtualKeyboard::~VirtualKeyboard()
	{
		if (Utils::VirtualKeyboard::m_Instance)
		{
			delete Utils::VirtualKeyboard::m_Instance;
		}
	}
	bool Utils::VirtualKeyboard::OnKeyPressed(int nKeyCode)
	{
		bool bHandled = false;

		if (m_pVKOnKeyPressedCallback)
		{
			bHandled = m_pVKOnKeyPressedCallback(nKeyCode);
		}

		return bHandled;
	}
	void Utils::VirtualKeyboard::OnTextChanged(std::wstring sNewText)
	{
		bool bChange = true;

		if (m_pVKOnTextChangedCallback)
		{
			bChange = m_pVKOnTextChangedCallback(sNewText);
		}

		if (bChange)
		{
			m_sKeyboardString = sNewText;
		}
	}
	std::wstring Utils::VirtualKeyboard::GetString()
	{
		return Utils::VirtualKeyboard::m_sKeyboardString;
	}
	bool Utils::VirtualKeyboard::IsVisible()
	{
		return m_bVisible;
	}

#if defined(OS_W8)
	void VKLostFocus(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
	{
		WCPToolkit::Utils::VirtualKeyboard::GetInstance()->Hide();
	}
	void VKKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
	{
		if (WCPToolkit::Utils::VirtualKeyboard::GetInstance()->OnKeyPressed((int)e->Key) == true)
		{
			e->Handled = true;
		}
	}
	void VKTextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
	{
		Windows::UI::Xaml::Controls::TextBox^ txt = (Windows::UI::Xaml::Controls::TextBox^)sender;

		if (txt != nullptr)
		{
			std::wstring wstr = WCPToolkit::Utils::PlatformStr2Wstr(txt->Text);
			WCPToolkit::Utils::VirtualKeyboard::GetInstance()->OnTextChanged(wstr);
		}
	}
#endif
	void Utils::VirtualKeyboard::Show(bool bTextBoxVisible, const std::wstring& sInitialText, int nMaxChars, int nType, int nLeft, int nTop, int nWidth, int nHeight, int nTextAlignment)
	{
#if defined(OS_W8) 
		if (WCPToolkit::Utils::GetParentPanel() == nullptr)
#else
#endif
		{
			Log(L"[Utils::VirtualKeyboard::Show] panel not set!");
			return;
		}
		if (nMaxChars < 0)
		{
			Log(L"[Utils::VirtualKeyboard::Show] nMaxChars should not have negative value!");
			return;
		}
		if (nType < 0)
		{
			Log(L"[Utils::VirtualKeyboard::Show] nType should not have negative value!");
			return;
		}
		if (nWidth < 0)
		{
			Log(L"[Utils::VirtualKeyboard::Show] nWidth should not have negative value!");
			return;
		}
		if (nHeight < 0)
		{
			Log(L"[Utils::VirtualKeyboard::Show] nHeight should not have negative value!");
			return;
		}
		if (nTextAlignment < 0)
		{
			Log(L"[Utils::VirtualKeyboard::Show] nTextAlignment should not have negative value!");
			return;
		}

#if defined(OS_W8)
		Windows::UI::Xaml::Controls::Panel^ pPanel = WCPToolkit::Utils::GetParentPanel();
		Windows::UI::Xaml::Controls::Button^ btn = (Windows::UI::Xaml::Controls::Button^)pPanel->FindName("WCPVhiddenbtn");
		if (btn == nullptr)
		{
			btn = ref new Windows::UI::Xaml::Controls::Button();
			pPanel->Children->Append(btn);
			btn->Name = "WCPVhiddenbtn";
			btn->Visibility = Windows::UI::Xaml::Visibility::Visible;
			Windows::UI::Xaml::Thickness margin = btn->Margin;
			margin.Left = 0;
			margin.Top = 0;
			margin.Right = 0;
			margin.Bottom = 0;
			btn->Margin = margin;
			btn->Width = 0;
			btn->Height = 0;
			btn->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
			btn->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
		}

		Windows::UI::Xaml::Controls::TextBox^ txt = (Windows::UI::Xaml::Controls::TextBox^)pPanel->FindName("WCPVKeyboard");
		if (txt == nullptr)
		{
			txt = ref new Windows::UI::Xaml::Controls::TextBox();
			pPanel->Children->Append(txt);
		}

		Windows::UI::Xaml::TextAlignment txtalign = (Windows::UI::Xaml::TextAlignment)nTextAlignment;
		Windows::UI::Xaml::Input::InputScope^ scope = ref new Windows::UI::Xaml::Input::InputScope();
		Windows::UI::Xaml::Input::InputScopeName^ name = ref new Windows::UI::Xaml::Input::InputScopeName();

		name->NameValue = (Windows::UI::Xaml::Input::InputScopeNameValue)nType;
		scope->Names->Append(name);

		txt->InputScope = scope;

		Windows::UI::Xaml::Thickness margin = txt->Margin;
		margin.Left = nLeft;
		margin.Top = nTop;
		margin.Right = 0;
		margin.Bottom = 0;
		txt->Margin = margin;
		txt->Width = nWidth;
		txt->Height = nHeight;
		txt->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
		txt->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
		txt->MaxLength = nMaxChars;
		txt->Visibility = Windows::UI::Xaml::Visibility::Visible;

		txt->KeyDown += ref new Windows::UI::Xaml::Input::KeyEventHandler(&VKKeyDown);
		txt->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(&VKTextChanged);
		txt->LostFocus += ref new Windows::UI::Xaml::RoutedEventHandler(&VKLostFocus);

		if (bTextBoxVisible)
		{
			txt->Opacity = 100;
		}
		else
		{
			txt->Opacity = 0;
		}

		txt->Name = "WCPVKeyboard";
		txt->Text = WCPToolkit::Utils::Wstr2PlatformStr(sInitialText);
		txt->TextAlignment = txtalign;
		txt->Select(txt->Text->Length(), 0);

		txt->Focus(Windows::UI::Xaml::FocusState::Programmatic);
#elif defined(OS_WP8)
		Platform::String^ str2 = WCPToolkit::Utils::Wstr2PlatformStr(sInitialText);
		NATIVE_CALL(nativeShowVirtualKeyboard)(bTextBoxVisible, str2, nMaxChars, nType, nLeft, nTop, nWidth, nHeight, nTextAlignment);
#endif

		m_bVisible = true;
	}
	void Utils::VirtualKeyboard::Hide()
	{
#if defined(OS_W8)
		Windows::UI::Xaml::Controls::Panel^ pPanel = WCPToolkit::Utils::GetParentPanel();
		Windows::UI::Xaml::Controls::TextBox^ txt = (Windows::UI::Xaml::Controls::TextBox^)pPanel->FindName("WCPVKeyboard");
		Windows::UI::Xaml::Controls::Button^ btn = (Windows::UI::Xaml::Controls::Button^)pPanel->FindName("WCPVhiddenbtn");
		if (btn != nullptr)
		{
			btn->Focus(Windows::UI::Xaml::FocusState::Pointer);

			//invoke btn click
			//Windows::UI::Xaml::Automation::Peers::ButtonAutomationPeer^ peer = ref new Windows::UI::Xaml::Automation::Peers::ButtonAutomationPeer(btn);
			//Windows::UI::Xaml::Automation::Provider::IInvokeProvider^ invokeProv = (Windows::UI::Xaml::Automation::Provider::IInvokeProvider^)peer->GetPattern(Windows::UI::Xaml::Automation::Peers::PatternInterface::Invoke);
			//invokeProv->Invoke();
		}

		for (unsigned int idx = 0; idx < pPanel->Children->Size; idx++)
		{
			Windows::UI::Xaml::UIElement^ eTXT = pPanel->Children->GetAt(idx);
			if (eTXT == txt)
			{
				pPanel->Children->RemoveAt(idx);
				break;
			}
		}
#elif defined(OS_WP8)
		NATIVE_CALL(nativeHideVirtualKeyboard)();
#endif
		m_bVisible = false;
	}

#pragma endregion VirtualKeyboard

#pragma region Conversions

	std::string Utils::PlatformStr2Str(Platform::String^ str)
	{
		return Utils::Wstr2Str(Utils::PlatformStr2Wstr(str));
	}

	Platform::String^ Utils::Str2PlatformStr(std::string str)
	{
		return Utils::Wstr2PlatformStr(Utils::Str2Wstr(str));
	}	

	std::wstring Utils::Str2Wstr(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

	std::string Utils::Wstr2Str(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.to_bytes(wstr);
	}

	std::wstring Utils::PlatformStr2Wstr(Platform::String^ str)
	{
		return std::wstring(str->Begin(), str->End());
	}

	Platform::String^ Utils::Wstr2PlatformStr(std::wstring wstr)
	{
		return ref new Platform::String(wstr.c_str());
	}

#pragma endregion Conversions

	void Utils::SetCoreDispatcher(Windows::UI::Core::CoreDispatcher^ dispatcher)
	{
		UIDispatcher = dispatcher;
	}

	void Utils::ExitApplication()
	{
#if defined(OS_W8)
#elif defined(OS_WP8)
		NATIVE_CALL(nativeExitApplication)();
#endif
	}

	void Utils::RunOnUIThread(std::function<void()> func)
	{
		if (UIDispatcher != nullptr)
		{
			if (UIDispatcher->HasThreadAccess)
			{
				func();
			}
			else
			{
				UIDispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
				{
					func();
				}));
			}
			return;
		}

#if defined(OS_W8)
		if (Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->HasThreadAccess)
		{
			func();
		}
		else
		{
			Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(
				Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
			{
				func();
			}));
		}
#elif defined(OS_WP8)
		//Windows::ApplicationModel::Core::CoreApplication::MainView doesn't work on WP8.0, so we just call the function directly on current thread
		Log(L"Warning: This function will not execute on the UI thread for a WP8.0 application.");
		func();
#endif
	}

	void Utils::Log(const std::wstring& message, bool lineReturn)
	{
#if defined (_DEBUG)
		OutputDebugString(message.c_str());

		if (lineReturn)
		{
			OutputDebugString(L"\n");
		}
#endif	
	}
	 

};