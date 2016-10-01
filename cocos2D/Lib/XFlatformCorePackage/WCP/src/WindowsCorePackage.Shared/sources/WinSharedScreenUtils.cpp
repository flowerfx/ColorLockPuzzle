#include "pch.h"

#include "WinShared.h"


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
	int WCPToolkitWinPhone80CppCallbacks::nativeGetWindowWidth()
	{
		return GetWindowWidth();
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeGetWindowHeight()
	{
		return GetWindowHeight();
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeSetScreenSleepMode(bool bSleep)
	{
		SetScreenSleepMode(bSleep);
	}
	bool WCPToolkitWinPhone80CppCallbacks::nativeIsScreenSleepModeOn()
	{
		return IsScreenSleepModeOn();
	}
}
#endif


namespace WCPToolkit
{
	static int g_WindowWidth = -1;
	static int g_WindowHeight = -1;

	void ScreenUtils::SetWindowHeight(int nHeight)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		g_WindowHeight = nHeight;
#endif
	}

	void ScreenUtils::SetWindowWidth(int nWidth)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		g_WindowWidth = nWidth;
#endif
	}

	int ScreenUtils::GetWindowWidth()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		if (g_WindowWidth == -1)
		{
			g_WindowWidth = (int)Windows::UI::Xaml::Window::Current->Bounds.Width;
		}
#elif defined(OS_WP8)
		if (g_WindowWidth == -1)
		{
			g_WindowWidth = NATIVE_CALL(nativeGetWindowWidth)();
		}
#endif

		return g_WindowWidth;
	}

	int ScreenUtils::GetWindowHeight()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		if (g_WindowHeight == -1)
		{
			g_WindowHeight = (int)Windows::UI::Xaml::Window::Current->Bounds.Height;
		}
#elif defined(OS_WP8)
		if (g_WindowHeight == -1)
		{
			g_WindowHeight = NATIVE_CALL(nativeGetWindowHeight)();
		}
#endif

		return g_WindowHeight;
	}

	void ScreenUtils::SetScreenSleepMode(bool bSleep)
	{
#if defined(OS_W8)
#elif defined(OS_WP8)
		NATIVE_CALL(nativeSetScreenSleepMode)(bSleep);
#endif
	}

	bool ScreenUtils::IsScreenSleepModeOn()
	{
#if defined(OS_W8)
		return false;
#elif defined(OS_WP8)
		return NATIVE_CALL(nativeIsScreenSleepModeOn)();
#endif
		return false;
	}

	bool ScreenUtils::IsFullScreen()
	{
		bool bRet = false;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		Windows::UI::ViewManagement::ApplicationView^ currentAppView = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
		bRet = currentAppView->IsFullScreen;
#elif defined(OS_WP8)
		//not available
#endif

		return bRet;
	}
};