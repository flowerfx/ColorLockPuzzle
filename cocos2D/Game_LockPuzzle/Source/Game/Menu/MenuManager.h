#ifndef __MENU_MANAGER_H__
#define __MENU_MANAGER_H__

#include "MenuEntityWrapper.h"
#include "Factory.h"
#include "../GamePlay/ActionPhaseManager.h"
#include "Background/BGObject.h"
#include "RKThread_Code/RKThread.h"
#include "RKThread_Code/RKInterlock.h"
#include "RKThread_Code/RKThreadWrapper.h"
#include "ParicleManager.h"
#include "cocos2d.h"

#include "Scene3D/SceneManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

#define SAVE_LEVEL_UNLOCK "LevelUnLock"
#define SAVE_HINT_REMAIN "HintRemain"

#define SAVE_STR_TUTORIAL_STEP "Tutorial_Step"



class MenuManager
{
private:
	static MenuManager * m_Instance;

	bool p_IsInActionPhase;
	bool p_IsPopUpAppear;

	//menu list

	std::vector<MENU_LAYER>							p_CurretUIActive;
	std::map<MENU_LAYER, MenuEntityWrapper*>		pListLayer;

	Factory::FactoryWithIndex<MenuEntityWrapper>	s_factory_menu_entity;
	//
	APManager * p_APMgr;
	//
	Vec2 p_current_size_game;
#ifdef	USE_DYNAMIC_BG
	Background::BackgroundGame * p_BGGame;

#ifdef USE_THREAD_BG
	RKUtils::ThreadWrapper *	  p_ThreadUpdateBackground;

	float p_dt;

	static uint32 ThreadUpdateBackground(void* pThreadData);
	void UpdateBGWithThread();
#endif

#endif

	Particle::ParticleManager *		m_ParticleMgr;
    
    RKUtils::RKThread *				p_threadCheckLoginService;
    static uint32 ThreadUpdateLoginService(void* pThreadData);
    
	Utility::Scene::SceneManager * p_SceneMgr;

protected:
	void SetActiveUI(MENU_LAYER layer, int idx = -1);
	void DeActiveUI(MENU_LAYER layer);

	void RegisterMenuUI();
public:
	MenuManager();
	~MenuManager();
	static MenuManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new MenuManager();
		}
		return m_Instance;
	}

	MENU_LAYER GetCurrentMenuLayer();
	void  SwitchToMenu(MENU_LAYER target_menu, MENU_LAYER from_menu = MENU_LAYER::MENU_NONE);
	void  CloseCurrentMenu(MENU_LAYER m);
    void  ShowCurrentMenu(MENU_LAYER m, int idx =-1);

	MenuEntityWrapper* GetMenuUI(MENU_LAYER name);
	MenuEntityWrapper* GetMenuUIByName(RKString name);

	bool IsOnActionPhase() { return p_IsInActionPhase; }
	void SetOnActionPhase(bool val) { p_IsInActionPhase = val; }

	int		Init();
	void	Update(float dt);
	void	Visit(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void	Draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	//AP event
	int		InitActionPhase(int level = 0);
	void	DeInitActionPhase();
	APManager * ApMgr() { return p_APMgr; }

	//call popup from menu mgr
	void OnShowPopUp(RKString title = "", RKString str = "", int numberBtn = 2, const char * strCB = "", void * objectCallBack = nullptr, void(*pt2Function)(void* pt2Object, const char * str) = NULL);
	void OnHidePopUp();

	//call tutorial from menu mgr
	void OnShowTutorialStep(int step);
	void OnHideTutorial();
	void PushMenuToTop(MENU_LAYER layer);

	float GetDataFromSave(RKString name);
	void  SetDataFromSave(RKString name, float data, std::string str = "");

	//create the particle
	void CreateParticle(RKString par_name, Vec2 pos, RKString name);
    
    //IGM when interrupt
    void ShowIGM();
    
    //game center
#ifdef GAME_IOS
    void LoginGameCenter();
    void ShowLeaderboardGameCenter();
    void PostScoreToGameCenter(int score , RKString str_id);
    void NavigateToGameCenterPage();
    RKString getIdLeaderboard();
#endif

	//handle size change
	void HandleSizeChange();

	//init scene3d

	void InitSceneManager();
    
    static void OnHandleDownloadAvatarCompelete(void * data_handler);
};

#define MenuMgr MenuManager::GetInstance()
#endif //__MENU_MANAGER_H__

