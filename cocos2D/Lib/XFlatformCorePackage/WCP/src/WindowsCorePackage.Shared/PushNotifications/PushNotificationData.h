#pragma once
#include "WinShared.h"

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
namespace PushNotificationData
{
	// Helper Functions for Online Push Notifications
	//////////////////////////////////////////////////////////////////////////
	void OnPushNotificationReceived(
		Windows::Networking::PushNotifications::PushNotificationChannel^ sender,
		Windows::Networking::PushNotifications::PushNotificationReceivedEventArgs^ e
	);
	
	// data used by Online Push Notifications
	//////////////////////////////////////////////////////////////////////////
	Windows::Networking::PushNotifications::PushNotificationChannel^		sChannel		= nullptr;
	Platform::String^														sEndpoint		= "";

	Windows::UI::Notifications::BadgeUpdater^								sBadgeUpdater	= nullptr;
	Windows::UI::Notifications::TileUpdater^								sTileUpdater	= nullptr;
	Windows::UI::Notifications::ToastNotifier^								sToastNotifier	= nullptr;
	
	// callback
	WCPToolkit::OnlinePushNotifications::ReceivedCallback					sCallback		= nullptr;
}
#endif

#ifdef OS_WP8
namespace WCPToolkit
{
	namespace CallbackPackerPN
	{
		// data is received from C# using this types, callback implemented in C++ will use the native std::String
		public value struct RuntimeOnlinePNContext						
		{
			Platform::String^	text1;
			Platform::String^	text2;
			Platform::String^	text3;
			Platform::String^	launchParameter;
		};

		// runtime type(class) that is used to pass the notification data
		public ref class CallbackPacker sealed
		{
		public:
			CallbackPacker() {};
			virtual ~CallbackPacker() {};
			
		internal:
			void	SetNativeCallback(WCPToolkit::OnlinePushNotifications::ReceivedCallback callback)	{ mCallback = callback; };

			void	ConvertRuntimeTypeToNativeType(
				RuntimeOnlinePNContext									inData_RunTime,
				WCPToolkit::OnlinePushNotifications::NotificationData	&outData_Native
				);

		private:
			WCPToolkit::OnlinePushNotifications::ReceivedCallback		mCallback;

		public:			//this will be exposed to the C# code
			property	RuntimeOnlinePNContext pushNotificationContext; 
			bool		CallNativeCallback();
		};

	}
}
#endif
