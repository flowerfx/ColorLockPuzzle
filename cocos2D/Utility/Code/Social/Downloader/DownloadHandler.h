#ifndef _DOWNLOAD_HANDLER_H_
#define _DOWNLOAD_HANDLER_H_

#include "network/HttpClient.h"
#include "../../DisplayView.h"
#include "../../FileManager.h"

#include "../../../../LIB/RKUtils/Code/RKThread_Code/RKThread.h"
#include "../../../../LIB/RKUtils/Code/RKThread_Code/RKInterlock.h"

using namespace cocos2d;
using namespace network;
using namespace RKUtils;
namespace Utility
{
	namespace Social
	{
		namespace Downloader
		{

			class DownloadHandler
			{
			private:

				struct state_save_url
				{
					RKString url_download;
					RKString name_save;

					state_save_url()
					{
						url_download = "";
						name_save = "";
					}
					state_save_url(RKString p_url_download, RKString p_name_save)
					{
						url_download = p_url_download;
						name_save = p_name_save;
					}

				};

				std::vector<state_save_url> p_list_queue_url_download;


				RKUtils::RKThread *			  p_ThreadUpdateDownload;
				RKUtils::RKInterlock*         m_exitDownload;
				RKUtils::RKCriticalSection*   m_criticalSectionDownload;
				RKUtils::RKThreadCondition*   m_conditionDownload;

				volatile bool p_IsOnUpdateThreadDownload;

				static uint32 ThreadUpdateDownload(void* pThreadData);
				void UpdateDownloadThread();

				void downLoadObject(RKString url, RKString nameSave);
				void onObjectDownLoaded(HttpClient* pSender, HttpResponse* pResponse);
			public:
				DownloadHandler();
				~DownloadHandler();

				void initDownloader();

				void OnDownloadListObject(std::vector<RKString> ListUrl, std::vector<RKString> ListnameSave);
			};
		}
	}
}

#endif//_DOWNLOAD_HANDLER_H_