#include "pch.h"
#include <ppltasks.h>

#include "PushNotificationImplementation.h"
#include "PushNotificationData.h"

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
using Windows::Networking::PushNotifications::PushNotificationChannel;
using Windows::Networking::PushNotifications::PushNotificationReceivedEventArgs;
using Windows::Networking::PushNotifications::PushNotificationChannelManager;
using Windows::Networking::PushNotifications::PushNotificationType;


// Implementation Functions For Windows Desktop Push Notifications
///////////////////////////////////////////////////////////////////////////////////////////////////
void				PushNotificationImplementation::Init()
{
	PushNotificationData::sBadgeUpdater		= Windows::UI::Notifications::BadgeUpdateManager::CreateBadgeUpdaterForApplication();
	PushNotificationData::sTileUpdater		= Windows::UI::Notifications::TileUpdateManager::CreateTileUpdaterForApplication();
	PushNotificationData::sToastNotifier	= Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();
	
	auto createChannel_Task = Concurrency::create_task(PushNotificationChannelManager::CreatePushNotificationChannelForApplicationAsync());

	createChannel_Task.then([](Concurrency::task<PushNotificationChannel^> previousTask)
	{
		try
		{
			PushNotificationData::sChannel = previousTask.get();
			
			if (PushNotificationData::sChannel)
			{
				PushNotificationData::sChannel->PushNotificationReceived += ref new Windows::Foundation::TypedEventHandler<PushNotificationChannel^, PushNotificationReceivedEventArgs^>(&PushNotificationData::OnPushNotificationReceived);
				PushNotificationData::sEndpoint = PushNotificationData::sChannel->Uri;

				WCPToolkit::Utils::Log(L"Push Notifications Endpoint:");
				WCPToolkit::Utils::Log(PushNotificationData::sEndpoint->Data(), true);
			}
			else
			{
				PushNotificationData::sEndpoint = "";

				WCPToolkit::Utils::Log(L"Invalid Push Notifications Channel: NULL");
				WCPToolkit::Utils::Log(L"\tCheck Internet Connection. Recall Init()");

				if (PushNotificationData::sCallback)
				{
					// call the callback with error message inside text1
					WCPToolkit::OnlinePushNotifications::NotificationData pushNotificationData;

					pushNotificationData.text1				= L"Invalid Push Notifications Channel! Check Internet Connection! Recall Init()!";
					pushNotificationData.text2				= L"Details: PushNotificationData::mChannel is NULL @ PushNotificationImplementation::Init()";
					pushNotificationData.text3				= L"";
					pushNotificationData.launchParameter	= L"ERROR";

					PushNotificationData::sCallback(pushNotificationData);
				}
			}
		}
		catch (Platform::Exception^ ex)
		{
			WCPToolkit::Utils::Log(L"Invalid Push Notifications Channel: NULL");
			WCPToolkit::Utils::Log(L"\tCheck Internet Connection. Recall Init()");
			WCPToolkit::Utils::Log(L"Exception @ PushNotificationImplementation::Init()");
			WCPToolkit::Utils::Log(ex->Message->Data());

			// call the callback with error message inside text1
			if (PushNotificationData::sCallback)
			{
				// call the callback with error message inside text1
				WCPToolkit::OnlinePushNotifications::NotificationData pushNotificationData;

				pushNotificationData.text1 = L"Invalid Push Notifications Channel! Check Internet Connection! Recall Init()!";
				pushNotificationData.text2 = L"Details: Exception @ PushNotificationImplementation::Init()";
				pushNotificationData.text3 = ex->Message->Data();
				pushNotificationData.launchParameter = L"ERROR";

				PushNotificationData::sCallback(pushNotificationData);
			}
		}
	});
}

Platform::String^	PushNotificationImplementation::GetEndpoint()
{
	return PushNotificationData::sEndpoint;
}

void PushNotificationImplementation::RegisterReceivedCallback(WCPToolkit::OnlinePushNotifications::ReceivedCallback callback)
{
	if (PushNotificationData::sCallback == nullptr)
	{
		PushNotificationData::sCallback = callback;
	}
	else
	{
		// nop
	}
}

void				PushNotificationImplementation::UnregisterReceivedCallback()
{
	PushNotificationData::sCallback = NULL;
}

void				PushNotificationImplementation::CloseChannel()
{
	if (PushNotificationData::sChannel)
	{
		try
		{
			PushNotificationData::sChannel->Close();
			PushNotificationData::sChannel = nullptr;
			WCPToolkit::Utils::Log(L"PushNotifications::CloseChannel()\n");
			PushNotificationData::sEndpoint = "";	// clear the Enpoint. can be used as Init() check
		}
		catch (Platform::Exception^ ex)
		{
			WCPToolkit::Utils::Log(L"Exception at: PushNotifications::CloseChannel()!\t");
			WCPToolkit::Utils::Log(ex->Message->Data(), true);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


// Helper Functions For Windows Desktop Push Notifications
///////////////////////////////////////////////////////////////////////////////////////////////////
void				PushNotificationData::OnPushNotificationReceived(
	PushNotificationChannel^ sender,
	PushNotificationReceivedEventArgs^ e
	)
{		
	switch (e->NotificationType)
	{
		case PushNotificationType::Badge:
		{
			e->Cancel = false;				// always update Badge content. don't cancel default handling by the System 

			Windows::UI::Notifications::BadgeNotification^ badgeNotification = ref new Windows::UI::Notifications::BadgeNotification(e->BadgeNotification->Content);
			PushNotificationData::sBadgeUpdater->Update(badgeNotification);

			break;
		}
		case PushNotificationType::Toast:	// passes Push Notification content to the callback if the callback was set. Display of the PN done by the OS based on the e->Cancel property
		{

			WCPToolkit::Utils::Log(L"\nOnline Push Notification Received()", true);

			// if the callback was set, pass the PN data, call it
			if (PushNotificationData::sCallback)
			{
				WCPToolkit::OnlinePushNotifications::NotificationData pushNotificationData;
				
				//the server should send a Toast Notification derived from the template Text02 (title + body)
				Windows::Data::Xml::Dom::XmlNodeList^ XmlTextNode = e->ToastNotification->Content->GetElementsByTagName(L"text");
				if (XmlTextNode != nullptr)
				{
					if (XmlTextNode->Length == 1)
					{
						//get the title
						std::wstring text1Data(XmlTextNode->Item(0)->InnerText->Data());
						pushNotificationData.text1 = text1Data;

						pushNotificationData.text2 = L"";	

						pushNotificationData.text3 = L"";												
					}

					if (XmlTextNode->Length == 2)
					{
						std::wstring text1Data(XmlTextNode->Item(0)->InnerText->Data());
						pushNotificationData.text1 = text1Data;

						std::wstring text2Data(XmlTextNode->Item(1)->InnerText->Data());
						pushNotificationData.text2 = text2Data;

						pushNotificationData.text3 = L"";
					}

					if (XmlTextNode->Length == 3)
					{
						std::wstring text1Data(XmlTextNode->Item(0)->InnerText->Data());
						pushNotificationData.text1 = text1Data;

						std::wstring text2Data(XmlTextNode->Item(1)->InnerText->Data());
						pushNotificationData.text2 = text2Data;

						std::wstring text3Data(XmlTextNode->Item(2)->InnerText->Data());
						pushNotificationData.text3 = text3Data;
					}
				}
				
				Platform::String^ launchData = e->ToastNotification->Content->DocumentElement->GetAttribute(L"launch");
				if (launchData != nullptr)
				{
					std::wstring wLaunchParam(launchData->Data());
					pushNotificationData.launchParameter = wLaunchParam;
				}

				Platform::String^ sound = e->ToastNotification->Content->DocumentElement->GetAttribute(L"sound");

				
				// pass the data and call the function
				bool wasHandled = PushNotificationData::sCallback(pushNotificationData);

				// SHOW PushNotification or NOT based on the response from the callback
				e->Cancel = wasHandled;

			}
			else
			{
				WCPToolkit::Utils::Log(L"\tCallback was not set, show all PushNotifications by default", true);

				// show all PushNotifications
				e->Cancel = false;
			}

			break;
		}
		case PushNotificationType::Tile:
		{
			e->Cancel = false;	// always update Tile content, don't cancel default handling by the System

			Windows::UI::Notifications::TileNotification^ tileNotification = ref new Windows::UI::Notifications::TileNotification(e->TileNotification->Content);
			PushNotificationData::sTileUpdater->Update(tileNotification);

			break;
		}
		case PushNotificationType::Raw:
		{
			// currently not handled by the lib			
			WCPToolkit::Utils::Log(e->RawNotification->Content->Data(), true);

			break;
		}
	}
}
#endif

#ifdef OS_WP8
bool WCPToolkit::CallbackPackerPN::CallbackPacker::CallNativeCallback()
{
	WCPToolkit::OnlinePushNotifications::NotificationData notificationData;
	CallbackPackerPN::CallbackPacker::ConvertRuntimeTypeToNativeType(this->pushNotificationContext, notificationData);
	return mCallback(notificationData); // call and return the native callback with the notificationData
}

void WCPToolkit::CallbackPackerPN::CallbackPacker::ConvertRuntimeTypeToNativeType(
	RuntimeOnlinePNContext									inContext_RunTime,
	WCPToolkit::OnlinePushNotifications::NotificationData	&outContext_Native)
{
	std::wstring wString (inContext_RunTime.text1->Data());
	outContext_Native.text1 = wString;

	wString = inContext_RunTime.text2->Data();
	outContext_Native.text2 = wString;

	wString = inContext_RunTime.text3->Data();
	outContext_Native.text3 = wString;

	wString = inContext_RunTime.launchParameter->Data();
	outContext_Native.launchParameter = wString;
}
#endif