#include "SocialManager.h"
#include "../LoadDataXML.h"
#include "../FXFlatform/XFlatform.h"
using namespace cocos2d;
namespace Utility
{
	namespace Social
    {
       
        SocialManager * SocialManager::m_Instance = NULL;
        
        SocialManager::SocialManager()
        {
            p_state_login_fb = FACEBOOK_STATE_LOGIN::FB_NONE;
            
            p_state_download_handler = STATUS_DOWNLOAD::D_NONE;
            
           // func
            
            p_downloader = NULL;
            
            
            //func
            p_list_register_function_callback.clear();
        }
        
        SocialManager::~SocialManager()
        {
            if(p_downloader)
            {
                delete p_downloader;
                p_downloader = NULL;
            }
            p_list_register_function_callback.clear();
        }
        
        void SocialManager::OnUpdate()
        {
            
        }
        
        void SocialManager::Init()
        {
            p_downloader = new Downloader::DownloadHandler();
            p_downloader->initDownloader();
            //
            XFlatform::FB_GetProfile();
        }
        
         void SocialManager::DownloadAvatarFB(RKString url, RKString name_save)
        {
            std::vector<RKString> ListUrl;
            std::vector<RKString> ListnameSave;
            ListUrl.push_back(url);
            ListnameSave.push_back(name_save);
            p_downloader->OnDownloadListObject(ListUrl, ListnameSave);
        }
        
        Sprite * SocialManager::GetSpriteAvatarFromID(RKString id)
        {
            RKString full_name = "fb:" + id + ".png";
            full_name = File::ResolveLocalNamePath(full_name);
            
            return Sprite::create(full_name.GetString());
        }
        
        
        void SocialManager::RegisterFunction(const char * strCB , void * objectCallBack , void(*pt2Function)(void* pt2Object))
        {
            p_list_register_function_callback.insert(std::pair<RKString, CustomFunc*>(strCB,new CustomFunc(objectCallBack,pt2Function)));
        }
        
        void SocialManager::processRegistedFunction(RKString call_command)
        {
            if(p_list_register_function_callback.size() > 0 && p_list_register_function_callback.find(call_command)!=p_list_register_function_callback.end())
            {
                auto function_ = p_list_register_function_callback.at(call_command);
                function_->p_Function(function_->p_ObjectCallBack);
            }
        }
    }
 }