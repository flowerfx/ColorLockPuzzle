#include "MenuTutorial.h"
#include "MenuHUD.h"
#include "../GamePlay/ActionPhaseManager.h"
#include "UI/AdvanceUI/wText.h"
MenuTutorial::MenuTutorial()
{
	p_Step_tutorial = 0;

	p_text_tutorial = nullptr;
	p_hand_tutorial = nullptr;
}

MenuTutorial::~MenuTutorial()
{
	p_Step_tutorial = 0;

	p_text_tutorial = nullptr;
	p_hand_tutorial = nullptr;
}

int MenuTutorial::Init()
{
	RKString _menu = "tutorial";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "tutorial_ipad";
	}

	InitMenuWidgetEntity(_menu);

	p_hand_tutorial = GetWidgetChildByName("layer_tutorial.hand_tut_ico");
	p_text_tutorial = GetWidgetChildByName("layer_tutorial.popup_message");

	return 1;
}

void MenuTutorial::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);

}

void MenuTutorial::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuTutorial::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuTutorial::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "btn_cancel")
	{
		p_Step_tutorial = -1;
		MenuMgr->OnHideTutorial();
		MenuMgr->SetDataFromSave(SAVE_STR_TUTORIAL_STEP, -1);
		File::SaveMgr->SaveTheGameSave();
	}
	else if (name == "btn_ok")
	{
		p_Step_tutorial = 1;
		OnShowOrHideIdxTarget(1);
	}
}

void MenuTutorial::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(TUTORIAL_MENU);
}


void MenuTutorial::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuTutorial::OnBeginFadeIn()
{
	
}

void MenuTutorial::OnFinishTutorialStep()
{
    p_hand_tutorial->ForceFinishAction();
    
    //MenuMgr->OnHideTutorial();
    OnFadeOut();

	//save the step tutorial
	if (p_Step_tutorial >= 5)
	{
		p_Step_tutorial = -1;
	}


	File::SaveMgr->GetDataSave()->SetDataAtName(SAVE_STR_TUTORIAL_STEP, p_Step_tutorial);
	File::SaveMgr->SaveTheGameSave();
}

void MenuTutorial::OnFadeInComplete()
{
	//OnShowTutorialStep(1);
}

void MenuTutorial::OnShowTutorialStep(int step)
{
	p_Step_tutorial = step;
    OnFadeIn();
    
	if (step == 1)
	{
		static_cast<TextWidget*>(p_text_tutorial)->SetText("Drag As Hand");
		p_text_tutorial->SetActionCommandWithName("STEP_1");

		Vec2 pos_obj1 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(6)->GetPosition();
		Vec2 pos_obj2 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(10)->GetPosition();

		pos_obj1 = Vec2((pos_obj1.x / GetGameSize().width) * p_design_size.x, (pos_obj1.y / GetGameSize().height) * p_design_size.y);
		pos_obj2 = Vec2((pos_obj2.x / GetGameSize().width) * p_design_size.x, (pos_obj2.y / GetGameSize().height) * p_design_size.y);

		pos_obj1.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj1.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);

		pos_obj2.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj2.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);

		p_hand_tutorial->SetHookCommand("STEP_1", "act0", 0, 0, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("SET_POSITION", pos_obj1);
			return value;
		}(), "SET_POSITION");

		p_hand_tutorial->SetHookCommand("STEP_1", "act0", 0, 3, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj2);
			return value;
		}(), "POSITION");

        p_hand_tutorial->SetActionCommandWithName("STEP_1",CallFunc::create(
                                                  [this]()
                                                  {
                                                    this->OnFinishTutorialStep();
                                                  }
                                                  ));

		p_hand_tutorial->ClearHookCommand("STEP_1", "act0", 0, 0);
		p_hand_tutorial->ClearHookCommand("STEP_1", "act0", 0, 3);
	}
	else if (step == 2)
	{
        static_cast<TextWidget*>(p_text_tutorial)->SetText("this is the target \n you must get !");
		p_text_tutorial->SetActionCommandWithName("STEP_1_5");

		Vec2 pos_obj1 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(5)->GetPosition();
		Vec2 pos_obj2 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(9)->GetPosition();
		Vec2 pos_obj3 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(10)->GetPosition();
		Vec2 pos_obj4 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(6)->GetPosition();

		pos_obj1 = Vec2((pos_obj1.x / GetGameSize().width) * p_design_size.x, (pos_obj1.y / GetGameSize().height) * p_design_size.y);
		pos_obj2 = Vec2((pos_obj2.x / GetGameSize().width) * p_design_size.x, (pos_obj2.y / GetGameSize().height) * p_design_size.y);
		pos_obj3 = Vec2((pos_obj3.x / GetGameSize().width) * p_design_size.x, (pos_obj3.y / GetGameSize().height) * p_design_size.y);
		pos_obj4 = Vec2((pos_obj4.x / GetGameSize().width) * p_design_size.x, (pos_obj4.y / GetGameSize().height) * p_design_size.y);

		pos_obj1.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj2.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj3.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj4.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);

		pos_obj1.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj2.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj3.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj4.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);

		p_hand_tutorial->SetHookCommand("STEP_2", "act0", 0, 0, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("SET_POSITION", pos_obj1);
			return value;
		}(), "SET_POSITION");

		p_hand_tutorial->SetHookCommand("STEP_2", "act0", 0, 3, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj2);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_2", "act0", 0, 4, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj3);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_2", "act0", 0, 5, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj4);
			return value;
		}(), "POSITION");

		p_hand_tutorial->SetActionCommandWithName("STEP_1_5", CallFunc::create(
			[this]()
		{
			static_cast<TextWidget*>(this->p_text_tutorial)->SetText("now drag as the target");
			this->p_text_tutorial->SetActionCommandWithName("STEP_1_5");

			this->p_hand_tutorial->SetActionCommandWithName("STEP_2", CallFunc::create(
				[this]()
			{
				this->OnFinishTutorialStep();
				this->p_hand_tutorial->ForceFinishAction();
			}
			));

			this->p_hand_tutorial->ClearHookCommand("STEP_2", "act0", 0, 0);
			this->p_hand_tutorial->ClearHookCommand("STEP_2", "act0", 0, 3);
			this->p_hand_tutorial->ClearHookCommand("STEP_2", "act0", 0, 4);
			this->p_hand_tutorial->ClearHookCommand("STEP_2", "act0", 0, 5);
		}
		));

	}
	else if (step == 3)
	{
		static_cast<TextWidget*>(p_text_tutorial)->SetText("sometime, you can move \n diagonally");
		p_text_tutorial->SetActionCommandWithName("STEP_2");

		Vec2 pos_obj1 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(0)->GetPosition();
		Vec2 pos_obj2 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(5)->GetPosition();
		Vec2 pos_obj3 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(6)->GetPosition();
		Vec2 pos_obj4 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(9)->GetPosition();
		Vec2 pos_obj5 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(10)->GetPosition();
		Vec2 pos_obj6 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(15)->GetPosition();

		pos_obj1 = Vec2((pos_obj1.x / GetGameSize().width) * p_design_size.x, (pos_obj1.y / GetGameSize().height) * p_design_size.y);
		pos_obj2 = Vec2((pos_obj2.x / GetGameSize().width) * p_design_size.x, (pos_obj2.y / GetGameSize().height) * p_design_size.y);
		pos_obj3 = Vec2((pos_obj3.x / GetGameSize().width) * p_design_size.x, (pos_obj3.y / GetGameSize().height) * p_design_size.y);
		pos_obj4 = Vec2((pos_obj4.x / GetGameSize().width) * p_design_size.x, (pos_obj4.y / GetGameSize().height) * p_design_size.y);
		pos_obj5 = Vec2((pos_obj5.x / GetGameSize().width) * p_design_size.x, (pos_obj5.y / GetGameSize().height) * p_design_size.y);
		pos_obj6 = Vec2((pos_obj6.x / GetGameSize().width) * p_design_size.x, (pos_obj6.y / GetGameSize().height) * p_design_size.y);

		pos_obj1.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj2.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj3.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj4.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj5.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj6.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);

		pos_obj1.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj2.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj3.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj4.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj5.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
		pos_obj6.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);

		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 0, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("SET_POSITION", pos_obj1);
			return value;
		}(), "SET_POSITION");

		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 3, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj2);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 4, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj3);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 5, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj4);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 6, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj5);
			return value;
		}(), "POSITION");
		p_hand_tutorial->SetHookCommand("STEP_3", "act0", 0, 7, [&]()->xml::BasicDec{
			xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj6);
			return value;
		}(), "POSITION");

		p_hand_tutorial->SetActionCommandWithName("STEP_3", CallFunc::create(
			[this]()
		{
			this->OnFinishTutorialStep();
			this->p_hand_tutorial->ForceFinishAction();
		}
		));

		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 0);
		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 3);
		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 4);
		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 5);
		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 6);
		p_hand_tutorial->ClearHookCommand("STEP_3", "act0", 0, 7);

	}
	else if (step == 4)
	{
		static_cast<TextWidget*>(p_text_tutorial)->SetText("object with different color \n you must drag once more time!");
		p_text_tutorial->SetActionCommandWithName("STEP_2_5");

		p_hand_tutorial->SetActionCommandWithName("STEP_3_5", CallFunc::create(
			[this]()
		{
			static_cast<TextWidget*>(this->p_text_tutorial)->SetText("Now drag as the target");
			this->p_text_tutorial->SetActionCommandWithName("STEP_3");

			Vec2 pos_obj1 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(5)->GetPosition();
			Vec2 pos_obj2 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(6)->GetPosition();
			Vec2 pos_obj3 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(9)->GetPosition();
			Vec2 pos_obj4 = MenuMgr->ApMgr()->GetUnitMgr()->GetUnitAtIdx(10)->GetPosition();

			pos_obj1 = Vec2((pos_obj1.x / GetGameSize().width) * p_design_size.x, (pos_obj1.y / GetGameSize().height) * p_design_size.y);
			pos_obj2 = Vec2((pos_obj2.x / GetGameSize().width) * p_design_size.x, (pos_obj2.y / GetGameSize().height) * p_design_size.y);
			pos_obj3 = Vec2((pos_obj3.x / GetGameSize().width) * p_design_size.x, (pos_obj3.y / GetGameSize().height) * p_design_size.y);
			pos_obj4 = Vec2((pos_obj4.x / GetGameSize().width) * p_design_size.x, (pos_obj4.y / GetGameSize().height) * p_design_size.y);

			pos_obj1.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj2.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj3.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj4.y -= (((p_hand_tutorial->GetSize().y * p_design_size.x) / GetGameSize().width) / 2.f);

			pos_obj1.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj2.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj3.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);
			pos_obj4.x += (((p_hand_tutorial->GetSize().x * p_design_size.x) / GetGameSize().width) / 2.f);

			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 0, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("SET_POSITION", pos_obj1);
				return value;
			}(), "SET_POSITION");

			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 3, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj2);
				return value;
			}(), "POSITION");
			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 4, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj3);
				return value;
			}(), "POSITION");
			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 5, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj4);
				return value;
			}(), "POSITION");
			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 6, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj2);
				return value;
			}(), "POSITION");
			p_hand_tutorial->SetHookCommand("STEP_4", "act0", 0, 7, [&]()->xml::BasicDec{
				xml::BasicDec value; value.InsertDataVector2("POSITION", pos_obj3);
				return value;
			}(), "POSITION");

			p_hand_tutorial->SetActionCommandWithName("STEP_4", CallFunc::create(
				[this]()
			{
				this->OnFinishTutorialStep();
				this->p_hand_tutorial->ForceFinishAction();
			}
			));

			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 0);
			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 3);
			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 4);
			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 5);
			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 6);
			p_hand_tutorial->ClearHookCommand("STEP_4", "act0", 0, 7);
		}
		));

	}
	else if (step == 5)
	{
		static_cast<TextWidget*>(p_text_tutorial)->SetText("Now begin your game");
		p_text_tutorial->SetActionCommandWithName("STEP_4", CallFunc::create(
			[this]()
		{
			this->OnFinishTutorialStep();
			this->p_hand_tutorial->ForceFinishAction();
		}
		));
	}

	p_Step_tutorial = step;
}
