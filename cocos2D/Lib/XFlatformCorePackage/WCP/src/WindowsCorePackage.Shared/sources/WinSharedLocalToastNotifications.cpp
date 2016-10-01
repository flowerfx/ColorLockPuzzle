#include "pch.h"

#include "WinShared.h"

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{

	int WCPToolkitWinPhone80CppCallbacks::nativeShowToastNotification(Platform::String^ title, Platform::String^ content, int delay)
	{
		return ShowToastNotification(title, content, delay);
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeRemoveToastNotificationsById(int id)
	{
		return RemoveToastNotificationsById(id);
	}
}
#endif

namespace WCPToolkit
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	static Windows::UI::Notifications::ToastNotifier^ s_tilemanager = nullptr;
#endif
	void LocalToastNotifications::ClearToastNotificationsById(int id)
	{

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)

		if(nullptr == s_tilemanager)
			s_tilemanager = Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();

		Windows::Foundation::Collections::IVectorView<Windows::UI::Notifications::ScheduledToastNotification^>^ scheduled = s_tilemanager->GetScheduledToastNotifications();

		if(id == -1)
		{
			for(unsigned int i = 0; i < scheduled->Size; i++)
			{
				s_tilemanager->RemoveFromSchedule(scheduled->GetAt(i));
			}

		}
		else
		{
			for(unsigned int i = 0; i < scheduled->Size; i++)
			{
				if(id == _wtoi(scheduled->GetAt(i)->Id->Data()))
				{
					s_tilemanager->RemoveFromSchedule(scheduled->GetAt(i));
				}

			}
		}
#elif defined(OS_WP8)  
		NATIVE_CALL(nativeRemoveToastNotificationsById) (id);
#endif

	}
	int LocalToastNotifications::ShowToastNotification(const std::wstring & titleText, const std::wstring & bodytext, const int & delay, const std::wstring & imagePath /*= L""*/)
	{

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		if(nullptr == s_tilemanager)
			s_tilemanager = Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();


		Windows::UI::Notifications::ToastTemplateType toastTemplate;

		if(imagePath.empty())
		{
			toastTemplate = Windows::UI::Notifications::ToastTemplateType::ToastText02;
		}
		else
		{
			toastTemplate = Windows::UI::Notifications::ToastTemplateType::ToastImageAndText02;
		}

		Windows::Data::Xml::Dom::XmlDocument^ toastXml = Windows::UI::Notifications::ToastNotificationManager::GetTemplateContent(toastTemplate);


		Windows::Data::Xml::Dom::XmlNodeList^ toastTextElements = toastXml->GetElementsByTagName("text");
		toastTextElements->Item(0)->InnerText = ref new Platform::String(titleText.c_str());
		toastTextElements->Item(1)->InnerText = ref new Platform::String(bodytext.c_str());

		if(!imagePath.empty())
		{
			Windows::Data::Xml::Dom::XmlNodeList^ toastImageAttributes = toastXml->GetElementsByTagName("image");
			static_cast<Windows::Data::Xml::Dom::XmlElement^>(toastImageAttributes->Item(0))->SetAttribute("src", ref new Platform::String(imagePath.c_str()));
			static_cast<Windows::Data::Xml::Dom::XmlElement^>(toastImageAttributes->Item(0))->SetAttribute("alt", "");
		}

		if(delay > 0)
		{
			Windows::Globalization::Calendar cl;
			cl.SetToNow();
			cl.AddSeconds(delay);
			Windows::Foundation::DateTime startTime = cl.GetDateTime();
			Windows::UI::Notifications::ScheduledToastNotification^ toast = ref new Windows::UI::Notifications::ScheduledToastNotification(toastXml, startTime);

			int id_of_toast = rand();

			wchar_t  id[15];
			_itow(id_of_toast, id, 10);

			toast->Id = ref new Platform::String(id);

			s_tilemanager->AddToSchedule(toast);

			return id_of_toast;

		}
		else
		{
			Windows::UI::Notifications::ToastNotification^ toast = ref new Windows::UI::Notifications::ToastNotification(toastXml);
			s_tilemanager->Show(toast);
		}

		return 0;

#elif defined(OS_WP8)

		return NATIVE_CALL(nativeShowToastNotification) (ref new Platform::String(titleText.c_str()), ref new Platform::String(bodytext.c_str()), delay);

#endif // OS_W8

	}

}