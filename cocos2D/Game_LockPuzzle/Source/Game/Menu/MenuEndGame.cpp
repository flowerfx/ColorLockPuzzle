#include "MenuEndGame.h"
#include "MenuHUD.h"

#include "SoundManager.h"
#include "ParicleManager.h"
MenuEndGame::MenuEndGame()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
}

MenuEndGame::~MenuEndGame()
{

}

int MenuEndGame::Init()
{
	RKString _menu = "EndGameMenu";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "EndGameMenu_ipad";
	}


	InitMenuWidgetEntity(_menu);

	

	return 1;
}

void MenuEndGame::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
}

void MenuEndGame::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuEndGame::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuEndGame::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{

	if (name == "setting_btn")
	{
		MenuMgr->SwitchToMenu(SETTING_MENU, RESULT_MENU);
	}
	else if (name == "home_btn")
	{
		MenuMgr->SwitchToMenu(MAIN_MENU, RESULT_MENU);
#if USE_CC_AUDIO
		GetSound->PlayMusicEffect("MUSIC_MAIN");
#endif
	}
}

void MenuEndGame::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(RESULT_MENU);
}


void MenuEndGame::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuEndGame::OnFadeInComplete()
{
	MenuMgr->CreateParticle("end_game", Vec2(240, 800), "effect_end_game");
}
