
#include "MenuMain.h"
#include "FXFlatform/XFlatform.h"
#include "Social/SocialManager.h"
#include "IAP/IAPManager.h"

#include "UI/AdvanceUI/wText.h"
//test
using namespace RKUtils;


MenuMain::MenuMain()
{
    p_avatar_fb = NULL;
    
    p_isInitAds = false;
}

MenuMain::~MenuMain()
{

}

int MenuMain::Init()
{
	RKString _menu = "main";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "main_ipad";
	}

	InitMenuWidgetEntity(_menu);
    
    //
    
    percent_ratio_appear_rating = 1;

	return 1;
}

void MenuMain::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
    
}

void MenuMain::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_avatar_fb)
	{
		p_avatar_fb->visit(renderer, transform, flags);
	}
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuMain::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_avatar_fb)
	{
		p_avatar_fb->draw(renderer, transform, flags);
	}
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuMain::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(MAIN_MENU);
}

void MenuMain::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	RKString setting_menu = "setting";
	float cur_ratio = GetGameSize().width / GetGameSize().height;

	if (name == "setting_btn")
	{
		OnCallProcessOfChild("layer_2.setting_btn", "ROTATE");
		MenuMgr->SwitchToMenu(SETTING_MENU,  MENU_LAYER::MENU_NONE);
		SetLayerInteractive(false);
	}
	else if (name == "start_btn")
	{
		MenuMgr->SwitchToMenu(SELECTION_LEVEL_MENU, MAIN_MENU);
	}
	else if (name == "shop_btn")
	{
		MenuMgr->SwitchToMenu(IAP_MENU, MENU_LAYER::MENU_NONE);
		SetLayerInteractive(false);
	}
    else if (name == "leaderboard_btn")
    {
#ifdef GAME_IOS
        MenuMgr->ShowLeaderboardGameCenter();
#endif
        
    }
}

void MenuMain::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuMain::OnBeginFadeIn()
{
	int LevelReach = (int)(File::SaveMgr->GetDataSave()->GetDataByName(SAVE_LEVEL_UNLOCK));
	static_cast<TextWidget*>(GetWidgetChildByName("layer_3.level_score"))->SetText(std::to_string(LevelReach));
}

void MenuMain::SetRankText(int idx_rank , int idx_score)
{
	if (idx_rank <= 0 || idx_score <= 0)
		return;

	static_cast<TextWidget*>(GetWidgetChildByName("layer_3.rank_score"))->SetText(std::to_string(idx_rank));
    static_cast<TextWidget*>(GetWidgetChildByName("layer_3.best_score"))->SetText(std::to_string(idx_score));
    
}

void MenuMain::OnSetProfileFB()
{

	/*auto p_avatar_sprite = SocialMgr->GetSpriteAvatarFromID(XFlatform::FB_GetUserProfile("id"));
    auto fb_state = GetResourceByName("fb_login_state");
    Vec2 size =fb_state->GetSizeResource();
    Vec2 pos = fb_state->GetPositionResource();

	p_avatar_sprite->setScale(size.x  / p_avatar_sprite->getContentSize().width, size.y / p_avatar_sprite->getContentSize().height);

	auto sprite_stencil = TextureMgr->GetSpriteByName("filled_circle");
	sprite_stencil->setScale(size.x  / sprite_stencil->getContentSize().width, size.y / sprite_stencil->getContentSize().height);

	p_avatar_fb = ClippingNode::create();
    p_avatar_fb->setPosition(pos);
	p_avatar_fb->setStencil(sprite_stencil);
	p_avatar_fb->addChild(p_avatar_sprite);
	p_avatar_fb->setAlphaThreshold(0.5f);

    RKString name_fb = XFlatform::FB_GetUserProfile("name");

	p_avatar_fb->retain();

	fb_state->VisibleResource(false);
	GetResourceByName("fb_status_circle")->VisibleResource(true);
    ((TextEntity*)GetResourceByName("fb_login_txt"))->SetText(name_fb);*/
}

void MenuMain::OnFadeInComplete()
{
	//check rating
	MenuWidget::OnFadeInComplete();

    if(File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_RATING)!="1")
    {
        //check % appear is 5%
        int max_ratio = 100 - (5 * percent_ratio_appear_rating);
        int appear = GetRandom(0, max_ratio > 5 ? max_ratio : 5);
        if(appear < 5)
        {
            MenuMgr->OnShowPopUp("\n Rating Game","\n\n\nDo you want to\nRate the game?", 2, "command_rate", (void*)this, MenuMain::CallBackFunction);
            percent_ratio_appear_rating = 1;
        }
        else{
            percent_ratio_appear_rating ++;
        }
    }
    
    //show ad
    if(!p_isInitAds)
    {
        IAPMgr->InitAds();
        
        p_isInitAds = true;
    }
    if(GetRandom(1,2) == 1) //rate 50%
    {
        IAPMgr->ShowAds();
    }
}

void MenuMain::CallBackFunction(void * p_Object, const char * str)
{
    MenuMain * self = (MenuMain*)p_Object;
    self->OnProcess(str);
}

void MenuMain::OnProcess(const char * str)
{
    if(strcmp(str,"command_rate")==0)
    {
        RKString id_game = "";
#ifdef GAME_IOS
        id_game = "1099618898";
#endif
        XFlatform::OnRateTheGame(id_game);
        
        File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_RATING, "1");
        File::SaveMgr->SaveTheGameSave();
    }
}
