#include "MenuInGame.h"
#include "MenuHUD.h"

#include "SoundManager.h"
MenuInGame::MenuInGame()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
}

MenuInGame::~MenuInGame()
{

}

int MenuInGame::Init()
{
	RKString _menu = "IGM";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "IGM_ipad";
	}


	InitMenuWidgetEntity(_menu);

	

	return 1;
}

void MenuInGame::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
}

void MenuInGame::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuInGame::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuInGame::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "back_btn")
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, INGAME_MENU);

		int id_music_play = GetRandom(1, 3);
		GetSound->PlayMusicEffect("MUSIC_AP_" + std::to_string(id_music_play), false);
	}
	else if (name == "setting_btn")
	{
		MenuMgr->SwitchToMenu(SETTING_MENU, INGAME_MENU);
		p_menu_show_next = SETTING_MENU;
	}
	else if (name == "home_btn")
	{
		MenuMgr->OnShowPopUp("", "ARE YOU SURE WANT TO QUIT ?", 2, "command_back_home", (void*)this, MenuInGame::CallBackFunction);
		SetLayerInteractive(false);
	}
	else if (name == "shop_btn")
	{
		MenuMgr->SwitchToMenu(IAP_MENU, INGAME_MENU);
		p_menu_show_next = IAP_MENU;
	}

}

void MenuInGame::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(INGAME_MENU);

	if (p_menu_show_next == MENU_NONE || p_menu_show_next == MAIN_MENU)
	{
		MenuMgr->GetMenuUI(HUD_MENU)->SetLayerInteractive(true);

		if (static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->IsExitAP() == true)
		{
			MenuMgr->SwitchToMenu(MAIN_MENU, HUD_MENU);
		}
	}

	p_menu_show_next = MENU_NONE;
}


void MenuInGame::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuInGame::OnBeginFadeIn()
{
	GetSound->PlayMusicEffect("MUSIC_IGM");
}

void MenuInGame::CallBackFunction(void * p_Object, const char * str)
{
	MenuInGame * self = (MenuInGame*)p_Object;
	self->OnProcess(str);
}

void MenuInGame::OnProcess(const char * str)
{
	if (strcmp(str, "command_back_home") == 0)
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, INGAME_MENU);
		static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->MarkAsExitAP();
#if USE_CC_AUDIO
		GetSound->PlayMusicEffect("MUSIC_MAIN");
#endif
	}
}