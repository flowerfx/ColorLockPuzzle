#include "MenuHUD.h"

#include "FileManager.h"
#include "UI/AdvanceUI/wText.h"
#include "UI/AdvanceUI/wPanel.h"

#include "SoundManager.h"
#include "IAP/IAPManager.h"

extern int RecorrectHint(int cur_value, int size);

MenuHUD::MenuHUD()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
	p_IsExitAP = false;

	p_current_hint_text = nullptr;
	p_current_level_text = nullptr;
	p_layer_icon_diagonally = nullptr;
	p_layer_target = nullptr;
}

MenuHUD::~MenuHUD()
{
	p_current_hint_text = nullptr;
	p_current_level_text = nullptr;
	p_layer_icon_diagonally = nullptr;
	p_layer_target = nullptr;
}

int MenuHUD::Init()
{
	RKString _menu = "HUD";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "HUD_ipad";
	}


	InitMenuWidgetEntity(_menu);

	p_current_hint_text = GetWidgetChildByName("layer_hud.HUD_layer.button_hint.number_hint");

	p_current_level_text = GetWidgetChildByName("layer_hud.HUD_layer.number_title");

	p_layer_icon_diagonally = GetLayerChildByName("layer_hud.diagonally_layer");

	int remain_hint = (int)File::SaveMgr->GetDataSave()->GetDataByName(SAVE_HINT_REMAIN);
	static_cast<TextWidget*>(p_current_hint_text)->SetText(std::to_string(remain_hint));

	return 1;
}

void MenuHUD::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
    
    if(IAPMgr->GetStateVideoAds() == 2)
    {
        GetWidgetChildByName("layer_hud.HUD_layer.button_reload_hint")->Visible(true);
        GetWidgetChildByName("layer_hud.HUD_layer.button_hint")->Visible(false);
        GetWidgetChildByName("layer_hud.button_ads_mov")->Visible(false);
        
        GetSound->ResumeTheCurrentMusic();
        IAPMgr->ResetStateVideoAds();
    }
	if (!GetSound->IsMusicPlaying())
	{
		int id_music_play = GetRandom(1, 3);

		GetSound->PlayMusicEffect("MUSIC_AP_" + std::to_string(id_music_play), false);
	}
}

void MenuHUD::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuHUD::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuHUD::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "pause_btn")
	{
		MenuMgr->SwitchToMenu(INGAME_MENU, MENU_LAYER::MENU_NONE);

		SetLayerInteractive(false);
	}
	else if (name == "button_reload_hint")
	{
		MenuMgr->ApMgr()->ShowHint();
	}
	else if (name == "button_hint")
	{
		int remain_hint = (int)File::SaveMgr->GetDataSave()->GetDataByName(SAVE_HINT_REMAIN);
		if (remain_hint > 0)
		{
			MenuMgr->ApMgr()->ShowHint();
			remain_hint--;
			File::SaveMgr->GetDataSave()->SetDataAtName(SAVE_HINT_REMAIN, remain_hint);
			File::SaveMgr->SaveTheGameSave();

			GetWidgetChildByName("layer_hud.HUD_layer.button_reload_hint")->Visible(true);
			GetWidgetChildByName("layer_hud.HUD_layer.button_hint")->Visible(false);

			static_cast<TextWidget*>(p_current_hint_text)->SetText(std::to_string(remain_hint));
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_USE_BOOST", 0.1f, 1);
#endif
		}
		else
		{
			MenuMgr->OnShowPopUp("ERROR !", "You dont have any hint! \nwanna get more hint ?", 2, "command_open_iap", (void*)this, MenuHUD::CallBackFunction);
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_CLICK_4", 0.1f, 1);
#endif
		}

	}
    else if (name == "button_ads_mov")
    {
        bool res = IAPMgr->ShowVideoAds();
        if(res)
        {
            GetSound->PauseTheCurrentMusic();
        }
        else
        {
            MenuMgr->OnShowPopUp("Some thing wrong!", "no video now ! try again later", 1);
        }
    }

}

void MenuHUD::ProcessNextLevel()
{
	GetWidgetChildByName("layer_hud.HUD_layer.button_reload_hint")->Visible(false);
	GetWidgetChildByName("layer_hud.HUD_layer.button_hint")->Visible(true);

	GetWidgetChildByName("layer_hud.button_ads_mov")->Visible(false);

	if (!p_have_move_diagonally)
	{
		p_layer_icon_diagonally->GetWidgetChildByName("move_diagonally")->GetResource()->stopAllActions();
		p_layer_icon_diagonally->Visible(false);
	}
}

void MenuHUD::OnDeactiveCurrentMenu()
{

	GetWidgetChildByName("layer_hud.HUD_layer.button_reload_hint")->Visible(false);
	GetWidgetChildByName("layer_hud.HUD_layer.button_hint")->Visible(true);
	GetWidgetChildByName("layer_hud.button_ads_mov")->Visible(false);

	p_layer_icon_diagonally->GetWidgetChildByName("move_diagonally")->GetResource()->stopAllActions();
	p_layer_icon_diagonally->Visible(false);

	p_have_move_diagonally = false;

	MenuMgr->CloseCurrentMenu(HUD_MENU);

	MenuMgr->DeInitActionPhase();
	p_IsExitAP = false;
}


void MenuHUD::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuHUD::SetNewTarget(int current_level, int & number_node_level)
{
	auto APMgr = MenuMgr->ApMgr();
	number_node_level = APMgr->GetLevelAtIdx(current_level - 1)->value.size();

	if (number_node_level == 4)
	{
		p_layer_target = GetWidgetChildByName("layer_hud.target_panel_2");
	}
	else if (number_node_level == 5)
	{
		p_layer_target = GetWidgetChildByName("layer_hud.target_panel_1");
	}
	if (p_layer_target)
	{
		auto w_node = GetWidgetChildByName("layer_hud.node_sl");
		bool IsNewLayer = (p_layer_target->GetNumberChildWidget() <= 0);
		for (int i = 0; i < number_node_level; i++)
		{
			for (int j = 0; j < number_node_level; j++)
			{
				WidgetEntity * w_node_temp = nullptr;
				if (IsNewLayer)
				{
					w_node_temp = static_cast<PanelWidget*>(w_node)->Clone();
					Vec2 design_size = this->GetDesignSize();
					float size_node = w_node_temp->GetSize().x;
					float distance = ((8.f * (GetGameSize().width / design_size.x)) + size_node);
					float size_y = p_layer_target->GetSize().y;
					Vec2 final_pos = Vec2(
						(j*distance) + (size_node / 2.f),
						size_y - (i*distance));

					w_node_temp->SetPosition(final_pos);
				}
				else
				{
					w_node_temp = p_layer_target->GetWidgetChildAtIdx((i *  number_node_level) + j);
				}
				int current_value = APMgr->GetLevelAtIdx(current_level - 1)->value.at(i).at(j);
				if (current_value > 0)
				{
					w_node_temp->Visible(true);
					Color4B color_at_idx = APMgr->GetUnitMgr()->GetColorHLAtIdx(current_value - 1);
					w_node_temp->SetColor(Color3B(
						color_at_idx.r, color_at_idx.g, color_at_idx.b
						));
				}
				else
				{
					w_node_temp->Visible(false);
				}
				if (IsNewLayer)
				{
					p_layer_target->AddChildWidget(w_node_temp);
				}
			}
		}
	}
}

void MenuHUD::OnSetTargetPanel(int current_level, bool next_step)
{
	auto title_level = GetWidgetChildByName("layer_hud.HUD_layer.number_title");
	if (current_level)
	{
		RKString text = "";
		if (current_level < 10)
		{
			text = "level 0" + std::to_string(current_level);
		}
		else
		{
			text = "level " + std::to_string(current_level);
		}
		static_cast<TextWidget*>(title_level)->SetText(text);
	}
	//
	auto APMgr = MenuMgr->ApMgr();
	if (APMgr)
	{
		int number_node_level = 0;

		if (!next_step)
		{
			SetNewTarget(current_level, number_node_level);
			p_layer_target->Visible(true);

			if (number_node_level == 4)
			{
				GetWidgetChildByName("layer_hud.target_panel_1")->Visible(false);
			}
			else if (number_node_level == 5)
			{
				GetWidgetChildByName("layer_hud.target_panel_2")->Visible(false);
			}
		}
		else
		{
			p_layer_target->SetActionCommandWithName("NEXT_STEP_1", CallFunc::create(
				[&, number_node_level, current_level]{
						SetNewTarget(current_level, const_cast<int &>(number_node_level));
						p_layer_target->Visible(true);

						if (number_node_level == 4)
						{
							GetWidgetChildByName("layer_hud.target_panel_1")->Visible(false);
						}
						else if (number_node_level == 5)
						{
							GetWidgetChildByName("layer_hud.target_panel_2")->Visible(false);
						}
						p_layer_target->SetActionCommandWithName("NEXT_STEP_2");
					}
			));
		}

		ProcessNextLevel();
	}
}

void MenuHUD::OnBeginFadeIn()
{
	int current_level = MenuMgr->ApMgr()->GetCurrentLevelNumber();

	OnSetTargetPanel(current_level + 1);


}

void MenuHUD::OnFadeInComplete()
{
    int current_step_tutorial = File::SaveMgr->GetDataSave()->GetDataByName(SAVE_STR_TUTORIAL_STEP);
    int current_level = MenuMgr->ApMgr()->GetCurrentLevelNumber();
    
	if (current_step_tutorial >= 0)
	{
		if (current_step_tutorial <= 1 && current_level == 0)
		{
			MenuMgr->OnShowTutorialStep(1);
		}
		else if (current_step_tutorial <= 2 && current_level == 1)
		{
			MenuMgr->OnShowTutorialStep(2);
		}
		else if (current_step_tutorial <= 3 && current_level == 2)
		{
			MenuMgr->OnShowTutorialStep(3);
		}
		else if (current_step_tutorial <= 4 && current_level == 3)
		{
			MenuMgr->OnShowTutorialStep(4);
		}
		else if (current_step_tutorial <= 5 && current_level == 4)
		{
			MenuMgr->OnShowTutorialStep(5);
		}
	}
}

void MenuHUD::OnMakeBtnPauseZoom(CallFunc * func)
{
	GetWidgetChildByName("layer_hud.HUD_layer.pause_btn")->SetActionCommandWithName("ZOOM_EFFECT", func);
}

void MenuHUD::OnMakeTitleLevelZoom()
{
	Vec2 size_text = p_current_level_text->GetSize();

	size_text = Vec2(size_text.x * p_design_size.x / GetGameSize().width, size_text.y * p_design_size.x / GetGameSize().width);

	p_current_level_text->SetHookCommand("ZOOM_EFFECT", "act0", 0, 0, [&]()->xml::BasicDec{
		xml::BasicDec value; value.InsertDataVector2("SIZE", Vec2(size_text.x * 1.3f, size_text.y * 1.3f));
		return value;
	}(), "SIZE");

	p_current_level_text->SetHookCommand("ZOOM_EFFECT", "act0", 0, 1, [&]()->xml::BasicDec{
		xml::BasicDec value; value.InsertDataVector2("SIZE", Vec2(size_text.x , size_text.y));
		return value;
	}(), "SIZE");

	p_current_level_text->SetActionCommandWithName("ZOOM_EFFECT");

	p_current_level_text->ClearHookCommand("ZOOM_EFFECT", "act0", 0, 0);
	p_current_level_text->ClearHookCommand("ZOOM_EFFECT", "act0", 0, 1);
}

void MenuHUD::OnShowVideoAdsBtn()
{
	auto btn_ads_vid = GetWidgetChildByName("layer_hud.button_ads_mov");
	btn_ads_vid->Visible(true);
	btn_ads_vid->SetActionCommandWithName("ON_SHOW");
}

void MenuHUD::OnShowDiagonalIcon(bool value)
{
	p_layer_icon_diagonally->Visible(value);
	if (value)
	{
		p_layer_icon_diagonally->SetActionCommandWithName("ON_SHOW", CallFunc::create([this]()
		{
			this->p_layer_icon_diagonally->ForceFinishAction();
			this->p_layer_icon_diagonally->GetWidgetChildByName("move_diagonally")->SetActionCommandWithName("ROTATE");
		}));
	}

	p_have_move_diagonally = value;
}

void MenuHUD::SetNumberMoveDia(int number)
{
	if (number >= 0)
	{
		p_layer_icon_diagonally->GetWidgetChildByName("cicle_icon")->Visible(true);
		p_layer_icon_diagonally->GetWidgetChildByName("number_hint")->Visible(true);
		static_cast<TextWidget*>(p_layer_icon_diagonally->GetWidgetChildByName("number_hint"))->SetText(std::to_string(number));
	}
	else
	{
		p_layer_icon_diagonally->GetWidgetChildByName("cicle_icon")->Visible(false);
		p_layer_icon_diagonally->GetWidgetChildByName("number_hint")->Visible(false);
	}
}

void MenuHUD::CallBackFunction(void * p_Object, const char * str)
{
	MenuHUD * self = (MenuHUD*)p_Object;
	self->OnProcess(str);
}

void MenuHUD::OnProcess(const char * str)
{
	if (strcmp(str, "command_open_iap") == 0)
	{
		MenuMgr->SwitchToMenu(IAP_MENU, MENU_NONE);
		this->SetLayerInteractive(false);
	}
}

Vec2 MenuHUD::GetPosNodeTargetAtIdx(int idx, int size)
{
	PASSERT2(p_layer_target != nullptr, "target table null!");

	auto node = p_layer_target->GetWidgetChildAtIdx(RecorrectHint(idx,size));

	Vec2 pos_return = node->GetPosition();


	pos_return.x = p_layer_target->GetPosition().x - (p_layer_target->GetSize().x / 2) + pos_return.x;
	pos_return.y = p_layer_target->GetPosition().y - (p_layer_target->GetSize().y / 2) + pos_return.y;

	return pos_return;
}

Vec2 MenuHUD::GetWorldPosHUDBtnMenu()
{
	auto return_value = GetWidgetChildByName("layer_hud.HUD_layer.pause_btn");

	Vec2 pos = return_value->GetResource()->convertToWorldSpace(return_value->GetPosition());

	return pos;
}