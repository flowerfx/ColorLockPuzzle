#include "pch.h"

#include "WinShared.h"

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
#include "../PushNotifications/PushNotificationImplementation.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
	// C# and C++ accessible
	void WCPToolkitWinPhone80CppCallbacks::nativeInitPN()
	{		
		InitPN();
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetEndpointPN()
	{
		return GetEndpointPN();
	}
	
	void WCPToolkitWinPhone80CppCallbacks::nativeRegisterCallbackPN(CallbackPackerPN::CallbackPacker^ packerPN)
	{
		RegisterCallbackPN(packerPN);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeUnregisterCallbackPN()
	{
		UnregisterCallbackPN();
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeCloseChannelPN()
	{
		CloseChannelPN();
	}
}
#endif


namespace WCPToolkit
{
	void			OnlinePushNotifications::Init()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		PushNotificationImplementation::Init();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeInitPN)();
#endif
	}

	std::string		OnlinePushNotifications::GetEndpoint()
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto psEndpoint = PushNotificationImplementation::GetEndpoint();
#elif defined(OS_WP8)
		auto psEndpoint = NATIVE_CALL(nativeGetEndpointPN)();
#endif
		std::wstring wsEndpoint = psEndpoint->Data();
		return std::string(wsEndpoint.begin(), wsEndpoint.end());
	}

	void			OnlinePushNotifications::RegisterReceivedCallback(ReceivedCallback callback)
	{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		if (callback != nullptr)
		{
			PushNotificationImplementation::RegisterReceivedCallback(callback);
			WCPToolkit::Utils::Log(L"The Push Notification Received Callback has been Registered");
		}
#elif defined(OS_WP8)
		if (callback != nullptr)
		{
			CallbackPackerPN::CallbackPacker^ packerPN = ref new CallbackPackerPN::CallbackPacker();
			packerPN->SetNativeCallback(callback);
			NATIVE_CALL(nativeRegisterCallbackPN)(packerPN);
			WCPToolkit::Utils::Log(L"The Push Notification Received Callback has been Registered");
		}
#endif
		else
		{
			// DEBUG MESSAGE, pass a non-null pointer to RegisterReceivedCallback()
			WCPToolkit::Utils::Log(L"OnlinePushNotifications::RegisterReceivedCallback() was passed a nullptr callback");
			WCPToolkit::Utils::Log(L"\tPlease pass a valid callback with the format:");
			WCPToolkit::Utils::Log(L"\tbool(*ReceivedCallback) (NotificationData & pushNotificationContext);");
			WCPToolkit::Utils::Log(L"\tThe callback is used for:");
			WCPToolkit::Utils::Log(L"\t\t-informing if the initialization fails");
			WCPToolkit::Utils::Log(L"\t\t-receiving data from the Push Notifications when they arrive to be handled by the game");
		}
	}

	void			OnlinePushNotifications::UnregisterReceivedCallback()
	{
#if defined(OS_W8)
		PushNotificationImplementation::UnregisterReceivedCallback();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeUnregisterCallbackPN)();
#endif 
		WCPToolkit::Utils::Log(L"The Push Notification Received Callback has been Unregistered");
	}

	void			OnlinePushNotifications::CloseChannel()
	{
#if defined(OS_W8)
		PushNotificationImplementation::CloseChannel();
#elif defined(OS_WP8)
		NATIVE_CALL(nativeCloseChannelPN)();
#endif
	}
};