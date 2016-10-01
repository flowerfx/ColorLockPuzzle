#include "MenuManager.h"

#include "MenuMain.h"
#include "MenuSetting.h"
#include "MenuIAP.h"
#include "MenuSelectionLevel.h"
#include "MenuHUD.h"
#include "MenuInGame.h"
#include "MenuPopUp.h"
#include "MenuTutorial.h"
#include "MenuEndGame.h"

#include "../MainGame.h"

#include "Social/SocialManager.h"
#include "IAP/IAPManager.h"

MenuManager * MenuManager::m_Instance = NULL;

MenuManager::MenuManager()
#ifdef	USE_DYNAMIC_BG
#ifdef USE_THREAD_BG
	:p_ThreadUpdateBackground(0)
#endif
#endif
{
	RegisterMenuUI();

	p_IsInActionPhase = false;
	p_IsPopUpAppear = false;
	p_APMgr = nullptr;
    p_threadCheckLoginService = NULL;

	p_current_size_game = GetGameSize();

	p_SceneMgr = nullptr;
}

MenuManager::~MenuManager()
{
	pListLayer.clear();

	delete m_ParticleMgr;
	m_ParticleMgr = NULL;

	delete p_APMgr;
	p_APMgr = NULL;

#ifdef	USE_DYNAMIC_BG
	delete p_BGGame;
	p_BGGame = NULL;
#ifdef USE_THREAD_BG
	if (p_ThreadUpdateBackground)
	{
		delete p_ThreadUpdateBackground;
		p_ThreadUpdateBackground = nullptr;
	}
#endif

#endif
    
    if(p_threadCheckLoginService)
    {
        RKUtils::RKThread_WaitForExit(p_threadCheckLoginService);
        RKUtils::RKThread_Destroy(&p_threadCheckLoginService);
    }

	if (p_SceneMgr)
	{
		delete p_SceneMgr;
		p_SceneMgr = nullptr;
	}
}

int MenuManager::Init()
{
	//init the background
#ifdef	USE_DYNAMIC_BG
	p_BGGame = new Background::BackgroundGame();
	p_BGGame->Init("background_effect");

#ifdef USE_THREAD_BG

	if (!p_ThreadUpdateBackground)
	{
		p_ThreadUpdateBackground = new ThreadWrapper();

		p_ThreadUpdateBackground->CreateThreadWrapper("thread_update_background", &MenuManager::ThreadUpdateBackground, (void*)this, RKThreadPriority::RKThreadPriority_BelowNormal);
	}
#endif

#endif

	//loop throught all the menu and init its
	for (int i = MAIN_MENU; i < MENU_COUNT; i++)
	{
		auto val = s_factory_menu_entity.CreateNewProduct((MENU_LAYER)i);
		if (val)
		{
			val->Init();
			pListLayer.insert(std::pair<MENU_LAYER, MenuEntityWrapper*>((MENU_LAYER)i, val));
		}
	}

	SwitchToMenu(MAIN_MENU);

	m_ParticleMgr = new Particle::ParticleManager();
	m_ParticleMgr->onInit();

    //thread check service
    if(!p_threadCheckLoginService)
    {
        p_threadCheckLoginService = RKThread_Create("thread_update_checkLoginService", &MenuManager::ThreadUpdateLoginService, (void*)this, RKThreadPriority::RKThreadPriority_Lowest);
        RKUtils::RKThread_Start(p_threadCheckLoginService);
    }
    
    //register function with social manager
    SocialMgr->RegisterFunction("download_avatar_complete",(void*)this,&MenuManager::OnHandleDownloadAvatarCompelete);
    
	return 1;
}

void MenuManager::Update(float dt)
{
	//
	for (int i = p_CurretUIActive.size() - 1; i >= 0; i--)
	{
		auto ui = pListLayer.at(p_CurretUIActive.at(i));
		ui->SetIdxMenuData(i);
		ui->UpdateMenu(dt);
	}
	//update action phase
	//dont up AP when IGM appear
	if (p_APMgr && p_IsInActionPhase 
		&& (p_CurretUIActive.at(0) == HUD_MENU 
			|| p_CurretUIActive.at(0) == TUTORIAL_MENU
		))
	{
		p_APMgr->Update(dt);
	}
	//
#ifdef	USE_DYNAMIC_BG
#ifdef USE_THREAD_BG
	if (!p_ThreadUpdateBackground->IsThreadRunning())
	{
		p_BGGame->RemoveObjectFinish();
		//
		p_dt = dt;
		p_ThreadUpdateBackground->OnWakeUpAndRunThread();
	}
#else
	p_BGGame->Update(dt);
#endif
#endif
	//
	m_ParticleMgr->onUpdate(dt);
	m_ParticleMgr->CheckAndRemoveEndLifeParticle();
    
    //update IAP
    Utility::IAPMgr->Update(dt);

	//update scene
	if (p_SceneMgr)
	{
		p_SceneMgr->Update(dt);
	}
}

void MenuManager::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	//draw scene
	if (p_SceneMgr)
	{
		p_SceneMgr->Visit(renderer, transform, flags);
	}

#ifdef	USE_DYNAMIC_BG
	//draw the background
	p_BGGame->Visit(renderer, transform, flags);
#endif
	//draw action phase event
	//dont draw AP when IGM appear
    if (p_APMgr && p_IsInActionPhase
        &&(
		p_CurretUIActive.at(0) == HUD_MENU
                     || p_CurretUIActive.at(0) == TUTORIAL_MENU
			))
    {
        /*if (p_CurretUIActive.at(0) == HUD_MENU || p_CurretUIActive.at(0) == TUTORIAL_MENU)
         {
         ((MenuHUD*)GetMenuUI(HUD_MENU))->DrawBackPanelAP(renderer, transform, flags);
         }*/
        
        p_APMgr->visit(renderer, transform, flags);
    }
	//
	for (int i = p_CurretUIActive.size() - 1; i >= 0; i--)
	{
		pListLayer.at(p_CurretUIActive.at(i))->VisitMenu(renderer, transform, flags);
	}

	//draw the particle
	m_ParticleMgr->onVisit(renderer, transform, flags);
}

void MenuManager::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	//draw scene
	if (p_SceneMgr)
	{
		p_SceneMgr->Draw(renderer, transform, flags);
	}
#ifdef	USE_DYNAMIC_BG
	//draw the background
	p_BGGame->Draw(renderer, transform, flags);
#endif
	//draw action phase event
	//dont draw AP when IGM appear
	if (p_APMgr && p_IsInActionPhase
		&&(
		p_CurretUIActive.at(0) == HUD_MENU
            || p_CurretUIActive.at(0) == TUTORIAL_MENU
		))
	{
		/*if (p_CurretUIActive.at(0) == HUD_MENU || p_CurretUIActive.at(0) == TUTORIAL_MENU)
		{
			((MenuHUD*)GetMenuUI(HUD_MENU))->DrawBackPanelAP(renderer, transform, flags);
		}*/

		p_APMgr->draw(renderer, transform, flags);
	}
	//
	for (int i = p_CurretUIActive.size() - 1; i >= 0; i--)
	{
		pListLayer.at(p_CurretUIActive.at(i))->DrawMenu(renderer, transform, flags);
	}
	//draw the particle
	m_ParticleMgr->onDraw(renderer, transform, flags);
}

void  MenuManager::SwitchToMenu(MENU_LAYER m, MENU_LAYER from_menu)
{
	if (m != MENU_LAYER::MENU_NONE)
	{
        if(from_menu == MENU_LAYER::MENU_NONE)
        {
			ShowCurrentMenu(m, (int)from_menu);
        }
		if (from_menu!= MENU_LAYER::MENU_NONE && GetMenuUI(from_menu)->GetMenuComeFrom() != m)
		{
			GetMenuUI(m)->SetMenuComFrom(from_menu);
		}
		else if (from_menu == MENU_LAYER::MENU_NONE && GetMenuUI(m)->GetMenuComeFrom() != MENU_LAYER::MENU_NONE)
		{
			GetMenuUI(m)->SetMenuComFrom(from_menu);
		}
        GetMenuUI(m)->SetLayerInteractive(true);
	}
	//
	if (from_menu != MENU_LAYER::MENU_NONE)
	{
		GetMenuUI(from_menu)->OnHide();
        GetMenuUI(from_menu)->SetMenuShowNext(m);
	}
}

void  MenuManager::CloseCurrentMenu(MENU_LAYER m)
{
    
    auto next_menu = GetMenuUI(m)->GetMenuShowNext();
    ShowCurrentMenu(next_menu, 0);
	DeActiveUI(m);
}

void  MenuManager::ShowCurrentMenu(MENU_LAYER m, int idx)
{
    if(m == MENU_LAYER::MENU_NONE)
    {
        return;
    }
	SetActiveUI(m, idx);
    GetMenuUI(m)->OnShow();
}

void MenuManager::SetActiveUI(MENU_LAYER layer, int idx)
{
	if (idx == -1)
		idx = p_CurretUIActive.size();
	else
		idx = 0;
    if(p_CurretUIActive.size() > 0 && p_CurretUIActive.at(idx) == layer)
    {
        return;
    }
    
    p_CurretUIActive.insert(p_CurretUIActive.begin() + idx , layer);
}

void MenuManager::DeActiveUI(MENU_LAYER layer)
{
	int idx = -1;
	for (int i = 0; i < p_CurretUIActive.size(); i++)
	{
		if (p_CurretUIActive.at(i) == layer)
		{
			idx = i;
			break;
		}
	}
	if (idx > -1)
	{
		GetMenuUI(layer)->SetMenuComFrom(MENU_LAYER::MENU_NONE);
		p_CurretUIActive.erase(p_CurretUIActive.begin() + idx);
		if (p_CurretUIActive.size() > 0)
		{
			GetMenuUI(p_CurretUIActive.at(0))->SetLayerInteractive(true);
		}
	}
}

void MenuManager::PushMenuToTop(MENU_LAYER layer)
{
	int idx_contain = -1;
	for (int i = 0; i < p_CurretUIActive.size(); i++)
	{
		if (p_CurretUIActive.at(i) == layer)
		{
			idx_contain = i;
			break;
		}
	}

	if (idx_contain == -1)
		return;

	//remove in stack
	p_CurretUIActive.erase(p_CurretUIActive.begin() + idx_contain);

	//add to top of stack
	p_CurretUIActive.insert(p_CurretUIActive.begin(), layer);
}

MENU_LAYER MenuManager::GetCurrentMenuLayer()
{
	if (p_CurretUIActive.size() > 0)
		return p_CurretUIActive.at(0);
	else
		return MENU_LAYER::MENU_NONE;
}

void MenuManager::RegisterMenuUI()
{
	s_factory_menu_entity.RegisterProduct<MenuMain>(MENU_LAYER::MAIN_MENU);
	s_factory_menu_entity.RegisterProduct<MenuSetting>(MENU_LAYER::SETTING_MENU);
	s_factory_menu_entity.RegisterProduct<MenuHUD>(MENU_LAYER::HUD_MENU);
	s_factory_menu_entity.RegisterProduct<MenuSelect>(MENU_LAYER::SELECTION_LEVEL_MENU);
	s_factory_menu_entity.RegisterProduct<MenuInGame>(MENU_LAYER::INGAME_MENU);
	s_factory_menu_entity.RegisterProduct<MenuPopUp>(MENU_LAYER::POPUP_MENU);
	s_factory_menu_entity.RegisterProduct<MenuEndGame>(MENU_LAYER::RESULT_MENU);
	//s_factory_menu_entity.RegisterProduct<MenuBoost>(MENU_LAYER::BOOST_MENU);
	s_factory_menu_entity.RegisterProduct<MenuIAP>(MENU_LAYER::IAP_MENU);
	s_factory_menu_entity.RegisterProduct<MenuTutorial>(MENU_LAYER::TUTORIAL_MENU);
}

MenuEntityWrapper* MenuManager::GetMenuUI(MENU_LAYER name)
{
	if (pListLayer.size() > 0 && pListLayer.find(name) != pListLayer.end())
	{
		return pListLayer.at(name);
	}
	return 0;
}

MenuEntityWrapper* MenuManager::GetMenuUIByName(RKString name)
{
	if (pListLayer.size() > 0)
	{
		for (auto it = pListLayer.begin(); it != pListLayer.end(); it++)
		{
			if (it->second->GetNameUI() == name)
			{
				return it->second;
			}
		}
	}
	return 0;
}

int		MenuManager::InitActionPhase(int level)
{
	p_APMgr = new APManager();
	p_APMgr->Init(level);

	p_IsInActionPhase = true;
	return 1;
}

void	MenuManager::DeInitActionPhase()
{
	if (p_APMgr)
	{
		delete p_APMgr;
		p_APMgr = NULL;
	}
	p_IsInActionPhase = false;

}

void MenuManager::OnShowPopUp(RKString title, RKString str, int numberBtn, const char * strCB, void * objectCallBack, void(*pt2Function)(void* pt2Object, const char * str))
{
	((MenuPopUp *)GetMenuUI(POPUP_MENU))->OnShowPopUp(title, str, numberBtn, strCB, objectCallBack, pt2Function);
    SetActiveUI(POPUP_MENU , 0); //push to top stack sreen
}

void MenuManager::OnHidePopUp()
{
    GetMenuUI(POPUP_MENU)->OnHide();
	///DeActiveUI(POPUP_MENU);
}

void MenuManager::OnShowTutorialStep(int step)
{
	if (GetDataFromSave(SAVE_STR_TUTORIAL_STEP) < 0)
		return;

	SetActiveUI(TUTORIAL_MENU, 0); //push to top stack sreen
	((MenuTutorial *)GetMenuUI(TUTORIAL_MENU))->OnShowTutorialStep(step);
}

void MenuManager::OnHideTutorial()
{
	if (GetDataFromSave(SAVE_STR_TUTORIAL_STEP) < 0)
		return;

	DeActiveUI(TUTORIAL_MENU);
}

float MenuManager::GetDataFromSave(RKString name)
{
	return File::SaveMgr->GetDataSave()->GetDataByName(name);
}

void  MenuManager::SetDataFromSave(RKString name, float data, std::string str)
{
	if (str == "")
		File::SaveMgr->GetDataSave()->SetDataAtName(name, data);
	else
		File::SaveMgr->GetDataSave()->SetCharAtName(name, str.c_str());
}

uint32 MenuManager::ThreadUpdateLoginService(void* pThreadData)
{
    MenuManager * This = static_cast<MenuManager*>(pThreadData);
    if (This)
    {
        bool IsUserAuthenticate = true;
        bool IsLoginLeaderboardSucceed = true;
        while (true) 
		{
#if defined GAME_IOS
            IsUserAuthenticate = GameMgr->GetGameCenterMgr()->IsGameCenterAuthenticate();
            if(IsUserAuthenticate)
            {
                RKString name_login = GameMgr->GetGameCenterMgr()->GetGameCenterName();
                if(name_login != "")
                {
                    ((MenuSetting*)This->GetMenuUI(SETTING_MENU))->SetInfoGameLogin(name_login , 1);
                    
                    IsLoginLeaderboardSucceed = GameMgr->GetGameCenterMgr()->IsLoadleaderboardInfoSucceed();

					if(IsLoginLeaderboardSucceed)
					{
						int rank = GameMgr->GetGameCenterMgr()->GetRankLeaderboard(This->getIdLeaderboard());
                        int best_score = GameMgr->GetGameCenterMgr()->GetBestScoreLeaderboard(This->getIdLeaderboard());
                        
                        static_cast<MenuMain*>(This->GetMenuUI(MAIN_MENU))->SetRankText(rank , best_score);
					}
                }
            }
            
            RKThread_Sleep(3, 3000); //sleep 3 sec
#endif
            if(IsUserAuthenticate && IsLoginLeaderboardSucceed)
            {
                break;
            }
        }
    }
    return 0;
}


#ifdef USE_THREAD_BG

uint32 MenuManager::ThreadUpdateBackground(void* pThreadData)
{
    MenuManager * This = static_cast<MenuManager*>(pThreadData);
    if (This)
    {
        This->UpdateBGWithThread();
    }
    return 0;
}

void MenuManager::UpdateBGWithThread()
{
	p_ThreadUpdateBackground->OnCheckUpdateThreadWrapper(
			[=](){
				p_BGGame->Update(p_dt);
			}
		);
}
#endif

void MenuManager::CreateParticle(RKString par_name, Vec2 pos, RKString name)
{
	if (m_ParticleMgr)
	{
		m_ParticleMgr->AddParticle(par_name, pos, name);
	}
}

 void MenuManager::ShowIGM()
{
    if(p_APMgr && p_IsInActionPhase && p_CurretUIActive.at(0) == HUD_MENU)
    {
        static_cast<MenuHUD*>(GetMenuUI(HUD_MENU))->OnProcessWidgetChild("pause_btn", UI_TYPE::UI_BUTTON);
    }
}

#ifdef GAME_IOS
void MenuManager::LoginGameCenter()
{
    GameMgr->GetGameCenterMgr()->Init();
}

void MenuManager::ShowLeaderboardGameCenter()
{
    if(GameMgr->GetGameCenterMgr()->IsGameCenterAuthenticate())
    {
        GameMgr->GetGameCenterMgr()->ShowLeaderboard("Level_Goal_Leaderboard");
    }
    else
    {
        //show game center login
        NavigateToGameCenterPage();
    }
}

void MenuManager::PostScoreToGameCenter(int score , RKString str_id)
{
    RKString id_leaderboard = getIdLeaderboard();
    if(id_leaderboard == "" || score <= 0)
        return;
    
    GameMgr->GetGameCenterMgr()->PostScoreToGameCenter(score, id_leaderboard);
}

RKString MenuManager::getIdLeaderboard()
{
    RKString id_leaderboard = "";
    
    id_leaderboard = "Level_Goal_Leaderboard";
   
    return id_leaderboard;
}

void MenuManager::NavigateToGameCenterPage()
{
     GameMgr->GetGameCenterMgr()->ShowGameCenterScheme();
}

#endif

void MenuManager::HandleSizeChange()
{
	for (auto it = pListLayer.begin(); it != pListLayer.end(); it++)
	{
		//it->second->ResetSizeAndPosMenuEntity(p_current_size_game.x, p_current_size_game.y);
	}

	p_current_size_game = GetGameSize();
}

void MenuManager::OnHandleDownloadAvatarCompelete(void * data_handler)
{
    MenuManager * menu_mgr = (MenuManager*)data_handler;
    if(menu_mgr)
    {
        ((MenuMain*)menu_mgr->GetMenuUI(MAIN_MENU))->OnSetProfileFB();
       // ((MenuSetting*)menu_mgr->GetMenuUI(SETTING_MENU))->OnSetProfileFB();
    }
}

void MenuManager::InitSceneManager()
{
	p_SceneMgr = new Utility::Scene::SceneManager();
	p_SceneMgr->CreateScene("desert");
}