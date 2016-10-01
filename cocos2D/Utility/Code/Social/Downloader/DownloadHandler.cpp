#include "DownloadHandler.h"
#include "../../FileManager.h"

#include "../../FXFlatform/XFlatform.h"
#include "../SocialManager.h"
namespace Utility
{
	namespace Social
	{
		namespace Downloader
		{
			DownloadHandler::DownloadHandler() :
				p_ThreadUpdateDownload(0)
				, m_exitDownload(0)
				, m_criticalSectionDownload(0)
				, m_conditionDownload(0)
			{
				//p_CurrentStatus = D_NONE;
			}

			DownloadHandler::~DownloadHandler()
			{
				bool bExit = false;
				if (m_exitDownload)
				{
					bExit = RKInterlock_Query(m_exitDownload) != 0;
				}

				if (!bExit)
				{
					// tell the work thread to exit
					RKInterlock_Lock(m_exitDownload);

					// wake work thread
					if (m_conditionDownload)
					{
						RKThreadCondition_WakeAll(m_conditionDownload);
					}

					if (p_ThreadUpdateDownload)
					{
						RKUtils::RKThread_WaitForExit(p_ThreadUpdateDownload);
						RKUtils::RKThread_Destroy(&p_ThreadUpdateDownload);
					}

					if (m_conditionDownload)
					{
						RKThreadCondition_Destroy(&m_conditionDownload);
					}

					if (m_criticalSectionDownload)
					{
						RKCriticalSection_Destroy(&m_criticalSectionDownload);
					}

					if (!m_exitDownload)
					{
						RKInterlock_Destroy(&m_exitDownload);
					}
				}
			}

			void DownloadHandler::initDownloader()
			{
				p_IsOnUpdateThreadDownload = false;

				if (!m_criticalSectionDownload)
				{
					m_criticalSectionDownload = RKCriticalSection_Create("cs_update_download");
				}

				if (!m_conditionDownload)
				{
					m_conditionDownload = RKThreadCondition_Create("Update_download_Cond");
				}

				if (!m_exitDownload)
				{
					m_exitDownload = RKInterlock_Create("Update_download_Exit");
				}
				RKInterlock_Reset(m_exitDownload);

				if (!p_ThreadUpdateDownload)
				{
					p_ThreadUpdateDownload = RKThread_Create("thread_update_downloader", &DownloadHandler::ThreadUpdateDownload, (void*)this, RKThreadPriority::RKThreadPriority_Lowest);
					RKUtils::RKThread_Start(p_ThreadUpdateDownload);
				}

				p_list_queue_url_download.clear();
			}

			uint32 DownloadHandler::ThreadUpdateDownload(void* pThreadData)
			{
				DownloadHandler * download_ = static_cast<DownloadHandler*>(pThreadData);
				if (download_)
				{
					download_->UpdateDownloadThread();
				}
				return 0;
			}

			void DownloadHandler::OnDownloadListObject(std::vector<RKString> ListUrl, std::vector<RKString> ListnameSave)
			{
				bool wake_up_thread = false;
				if (p_list_queue_url_download.size() == 0)
				{
					wake_up_thread = true;
				}
				
                CCASSERT(ListUrl.size() == ListnameSave.size(), "2 list size must be equal");

                RKCriticalSection_Enter(m_criticalSectionDownload); //enter critical section

                for (int i = 0; i < ListUrl.size(); i++)
                {
                    p_list_queue_url_download.push_back(state_save_url(ListUrl.at(i), ListnameSave.at(i)));
                }

                RKCriticalSection_Leave(m_criticalSectionDownload); //leave critical section
				

				if (wake_up_thread)
				{
					p_IsOnUpdateThreadDownload = true;
					RKThreadCondition_WakeAll(m_conditionDownload);
				}
			}

			void DownloadHandler::UpdateDownloadThread()
			{
				// keep the thread running until we want to stop it
				bool bExit = RKInterlock_Query(m_exitDownload) != 0;
				while (!bExit)
				{
					// wait for work to be added
					RKCriticalSection_Enter(m_criticalSectionDownload);
					bExit = RKInterlock_Query(m_exitDownload) != 0;

					while (!p_IsOnUpdateThreadDownload && !bExit)
					{
						RKThreadCondition_Sleep(m_conditionDownload, m_criticalSectionDownload, 0xFFFFFF); //sleep infinity and wait wake up
						bExit = RKInterlock_Query(m_exitDownload) != 0;
					}

					if (bExit)
					{
						RKCriticalSection_Leave(m_criticalSectionDownload);
						break;
					}

					if (p_list_queue_url_download.size() > 0)
					{
						auto url_download = p_list_queue_url_download.at(0);
						downLoadObject(url_download.url_download, url_download.name_save);
					}

					p_IsOnUpdateThreadDownload = false;

					RKCriticalSection_Leave(m_criticalSectionDownload);
				}
				RKInterlock_Reset(m_exitDownload);
			}

			void DownloadHandler::downLoadObject(RKString url, RKString nameSave)
			{
				HttpRequest* request = new (std::nothrow) HttpRequest();
				request->setUrl(url.GetString());
				request->setRequestType(HttpRequest::Type::GET);
				//request->setResponseCallback(CC_CALLBACK_2(DownloadHandler::onObjectDownLoaded, this));

				const network::ccHttpRequestCallback& call_back = ([=](network::HttpClient * client, network::HttpResponse * pResponse)
				{
					onObjectDownLoaded(client, pResponse);
				});

				request->setResponseCallback(call_back);
				request->setTag(nameSave.GetString());
				HttpClient::getInstance()->send(request);
                
				SocialMgr->SetStateDownload(D_DOWNLOADING);
                
				request->release();
			}



			void DownloadHandler::onObjectDownLoaded(HttpClient* pSender, HttpResponse* pResponse)
			{
				HttpResponse* response = pResponse;

				if (!response)
				{
					SocialMgr->SetStateDownload(D_FAIL);
					cocos2d::log("No Response");
				}
				else
				{
					int statusCode = (int)response->getResponseCode();

					char statusString[64] = {};
					sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
					cocos2d::log("response code: %d", statusCode);

					if (!response->isSucceed())
					{
                        SocialMgr->SetStateDownload(D_FAIL);
						cocos2d::log("response failed");
						cocos2d::log("error buffer: %s", response->getErrorBuffer());
					}
					else
					{
						std::vector<char>*buffer = response->getResponseData();

						Image * img = new Image();
						img->initWithImageData((unsigned char *)&(buffer->front()), buffer->size());

						// Save object file to device.
						std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
						writablePath.append(response->getHttpRequest()->getTag());
                        
						img->saveToFile(writablePath.c_str());
                        //
                        
                        //
                        if(RKString(response->getHttpRequest()->getTag()) == "fb:" + RKString(XFlatform::FB_GetUserProfile("id").c_str()) + ".png")
                        {
                            SocialMgr->processRegistedFunction("download_avatar_complete");
                        }
                        
                        
						//
                        
                        SocialMgr->SetStateDownload(D_SUCCESS);
					}
				}

				if (SocialMgr->GetStateDownload() == D_FAIL || SocialMgr->GetStateDownload()  == D_SUCCESS)
				{
					p_list_queue_url_download.erase(p_list_queue_url_download.begin());//remove the url downloaded or fail ...
					if (p_list_queue_url_download.size() > 0)
					{
						p_IsOnUpdateThreadDownload = true;
						RKThreadCondition_WakeAll(m_conditionDownload);
                        SocialMgr->SetStateDownload(D_DOWNLOADING);
						//continue download when their is the object on the list
					}
					else
					{
						//download all succeed
						SocialMgr->SetStateDownload(D_NONE);
					}
				}
			}
		}
	}
}