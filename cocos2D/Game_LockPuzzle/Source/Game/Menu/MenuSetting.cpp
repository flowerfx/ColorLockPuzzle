#include "MenuSetting.h"

#include "FileManager.h"
#include "SoundManager.h"
#include "FXFlatform/XFlatform.h"
#include "Social/SocialManager.h"

#include "UI/AdvanceUI/wText.h"
#include "UI/AdvanceUI/WidgetEntity.h"
MenuSetting::MenuSetting()
{
    p_option_scroll = nullptr;
	p_menu_come_from = MENU_LAYER::MENU_NONE;
}

MenuSetting::~MenuSetting()
{
    p_option_scroll = nullptr;
}

int MenuSetting::Init()
{
	RKString _menu = "setting";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "setting_ipad";
	}


	InitMenuWidgetEntity(_menu);


	p_option_scroll = GetWidgetChildByName("layer_options.layout_3.scroll_options");
	//check is music enable
	bool IsMusicEnable = File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_MUSIC) == "1" ? true : false;
	if (IsMusicEnable)
	{
		p_option_scroll->GetWidgetChildByName(".music_btn._on_panel")->Visible(true);
		p_option_scroll->GetWidgetChildByName(".music_btn._off_panel")->Visible(false);
	}
	else
	{
		p_option_scroll->GetWidgetChildByName(".music_btn._on_panel")->Visible(false);
		p_option_scroll->GetWidgetChildByName(".music_btn._off_panel")->Visible(true);
	}

	//check is sound enable

	bool IsSoundEnable = File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_SOUND) == "1" ? true : false;
	if (IsSoundEnable)
	{
		p_option_scroll->GetWidgetChildByName(".sound_btn._on_panel")->Visible(true);
		p_option_scroll->GetWidgetChildByName(".sound_btn._off_panel")->Visible(false);
	}
	else
	{
		p_option_scroll->GetWidgetChildByName(".sound_btn._on_panel")->Visible(false);
		p_option_scroll->GetWidgetChildByName(".sound_btn._off_panel")->Visible(true);
	}
    
//#if defined GAME_IOS
//    GetResourceByName("gamecenter_title")->VisibleResource(true);
//    GetResourceByName("off_gamecenter")->VisibleResource(true);
//    GetResourceByName("gamecenter_btn")->VisibleResource(true);
//#elif defined GAME_ANDROID
//    GetResourceByName("google_title")->VisibleResource(true);
//    GetResourceByName("off_ggoogle")->VisibleResource(true);
//    GetResourceByName("google_btn")->VisibleResource(true);
//#endif
    
#ifdef GAME_IOS
    p_option_scroll->GetWidgetChildByName(".restore_purchase_btn")->Visible(true);
    p_option_scroll->GetWidgetChildByName(".restore_title")->Visible(true);
#else
    p_option_scroll->GetWidgetChildByName(".restore_purchase_btn")->Visible(false);
    p_option_scroll->GetWidgetChildByName(".restore_title")->Visible(false);
#endif
    
    return 1;
}

void MenuSetting::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
    
    if(IAPMgr->StateGetRestorePurchase() != IAP::IAP_NONE)
    {
     if(IAPMgr->StateGetRestorePurchase() == IAP::IAP_FAIL)
     {
         ((TextWidget*)p_option_scroll->GetWidgetChildByName(".restore_title"))->SetText("fail or \nnothing restore");
     }
     else if (IAPMgr->StateGetRestorePurchase() == IAP::IAP_SUCCEED)
     {
         ((TextWidget*)p_option_scroll->GetWidgetChildByName(".restore_title"))->SetText("Restore Succeed!");
     }
     IAPMgr->ClearStateRestorePurchase();
    }
}

void MenuSetting::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuSetting::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuSetting::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "info_btn")
	{
		OnCallProcessOfChild("layer_info", "FADE_IN");
		SetChildInteractive("layer_options", false);
	}
	else if (name == "close_info_btn")
	{
		OnCallProcessOfChild("layer_info.info_panel", "FADE_OUT");
	}
	else if (name == "back_btn")
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, SETTING_MENU);
		if (!MenuMgr->IsOnActionPhase())
		{
			MenuMgr->GetMenuUI(MAIN_MENU)->OnCallProcessOfChild("layer_2.setting_btn", "ROTATE");
		}
	}
	else if (name == "sound_btn")
	{
		auto button = GetWidgetChildByName("layer_options.layout_3.scroll_options.sound_btn");
		auto p1 = button->GetWidgetChildByName("._on_panel");
		auto p2 = button->GetWidgetChildByName("._off_panel");

		if (File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_SOUND) == "1")
		{
			p1->Visible(false);
			p2->Visible(true);
			GetSound->MuteVolume();
			File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_EN_SOUND, "0");
		}
		else
		{
			p1->Visible(true);
			p2->Visible(false);

			GetSound->ResetVolume();
			File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_EN_SOUND, "1");
		}
		File::SaveMgr->SaveTheGameSave();

	}
	else if (name == "music_btn")
	{
		auto button = GetWidgetChildByName("layer_options.layout_3.scroll_options.music_btn");
		auto p1 = button->GetWidgetChildByName("._on_panel");
		auto p2 = button->GetWidgetChildByName("._off_panel");

		if (File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_MUSIC) == "1")
		{
			p1->Visible(false);
			p2->Visible(true);
			GetSound->MuteVolume(false);
			File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_EN_MUSIC, "0");
		}
		else
		{
			p1->Visible(true);
			p2->Visible(false);

			GetSound->ResetVolume(false);
			File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_EN_MUSIC, "1");
		}
		File::SaveMgr->SaveTheGameSave();
	}
	else if (name == "gamecenter_btn")
	{
#ifdef GAME_IOS
        MenuMgr->NavigateToGameCenterPage();
#endif
	}
    else if (name == "restore_purchase_btn")
    {
#ifdef GAME_IOS
        int res = IAPMgr->RestorePurchase();
        if(res == 0)
        {
             ((TextWidget*)p_option_scroll->GetWidgetChildByName(".restore_title"))->SetText("Restoring..");
        }
        else if (res == 1)
        {
            ((TextWidget*)p_option_scroll->GetWidgetChildByName(".restore_title"))->SetText("Restore Complete");
        }
        else if (res == -1)
        {
            ((TextWidget*)p_option_scroll->GetWidgetChildByName(".restore_title"))->SetText("on restoring !");
        }
#endif
    }
    else if (name == "rating_btn")
    {
        RKString id_game = "";
#ifdef GAME_IOS
        id_game = "1099618898";
#endif
        XFlatform::OnRateTheGame(id_game);
        
        File::SaveMgr->GetDataSave()->SetCharAtName(SAVE_STR_RATING, "1");
        File::SaveMgr->SaveTheGameSave();
    }
    else if(name == "btn_email")
    {
        XFlatform::SendEmail("qchien.gl@hotmail.com", "[customer email] your title here!", "ask me anything you want !");
    }
}

void MenuSetting::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(SETTING_MENU);
	if (!MenuMgr->IsOnActionPhase())
	{
		MenuMgr->GetMenuUI(MAIN_MENU)->SetLayerInteractive(true);
	}
}

void MenuSetting::SetInfoGameLogin(RKString name_login, int type)
{
   if(type == 1)
   {
       static_cast<TextWidget*>(GetWidgetChildByName("layer_options.layout_3.scroll_options.login_title"))->SetText(name_login);
       auto button = GetWidgetChildByName("layer_options.layout_3.scroll_options.gamecenter_btn");
       button->GetWidgetChildByName("._on_panel")->Visible(true);
       button->GetWidgetChildByName("._off_panel")->Visible(false);
   }
}

void MenuSetting::OnSetProfileFB()
{
    
}

void MenuSetting::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
	if (idx == 2 && status == STATUS_MENU::M_FADEIN)
	{
		//show the layer 3
		OnCallProcessOfChild("layer_info.info_panel", "FADE_IN");
	}
	else if (idx == 4 && status == STATUS_MENU::M_FADEOUT && p_ListStatus.at(2)->p_IsActive)
	{
		//hide the layer 2
		OnCallProcessOfChild("layer_info", "FADE_OUT");
		SetChildInteractive("layer_options", true);
	}
}