#pragma once

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)

#include "WinShared.h"

namespace PushNotificationImplementation
{
	void					Init();

	::Platform::String^		GetEndpoint();
	
	void					RegisterReceivedCallback(WCPToolkit::OnlinePushNotifications::ReceivedCallback cb);
	
	void					UnregisterReceivedCallback();

	void					CloseChannel();
}
#endif

#ifdef OS_WP8
	// implementation for Windows Phone is in PushNotifications.cs
#endif