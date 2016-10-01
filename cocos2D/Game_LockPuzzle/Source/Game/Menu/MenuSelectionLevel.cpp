#include "MenuSelectionLevel.h"

#include "FileManager.h"
#include "SoundManager.h"

#include "UI/AdvanceUI/wPageView.h"
#include "UI/AdvanceUI/wText.h"

#include "../GamePlay/UnitManager.h"

int RecorrectHint(int cur_value, int size)
{
	if (size == 4)
	{
		if (cur_value == 0) return 0;
		else if (cur_value == 1) return 4;
		else if (cur_value == 2) return 8;
		else if (cur_value == 3) return 12;

		else if (cur_value == 4) return 1;
		else if (cur_value == 5) return 5;
		else if (cur_value == 6) return 9;
		else if (cur_value == 7) return 13;

		else if (cur_value == 8) return 2;
		else if (cur_value == 9) return 6;
		else if (cur_value == 10) return 10;
		else if (cur_value == 11) return 14;

		else if (cur_value == 12) return 3;
		else if (cur_value == 13) return 7;
		else if (cur_value == 14) return 11;
		else if (cur_value == 15) return 15;
	}
	else if (size == 5)
	{
		if (cur_value == 0) return 0;
		else if (cur_value == 1) return 5;
		else if (cur_value == 2) return 10;
		else if (cur_value == 3) return 15;
		else if (cur_value == 4) return 20;

		else if (cur_value == 5) return 1;
		else if (cur_value == 6) return 6;
		else if (cur_value == 7) return 11;
		else if (cur_value == 8) return 16;
		else if (cur_value == 9) return 21;

		else if (cur_value == 10) return 2;
		else if (cur_value == 11) return 7;
		else if (cur_value == 12) return 12;
		else if (cur_value == 13) return 17;
		else if (cur_value == 14) return 22;

		else if (cur_value == 15) return 3;
		else if (cur_value == 16) return 8;
		else if (cur_value == 17) return 13;
		else if (cur_value == 18) return 18;
		else if (cur_value == 19) return 23;

		else if (cur_value == 20) return 4;
		else if (cur_value == 21) return 9;
		else if (cur_value == 22) return 14;
		else if (cur_value == 23) return 19;
		else if (cur_value == 24) return 24;
	}

}



MenuSelect::MenuSelect()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
	p_number_level = 0;

	p_page_view = nullptr;
}

MenuSelect::~MenuSelect()
{
	p_page_view = nullptr;
}

int MenuSelect::Init()
{
	RKString _menu = "selection";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "selection_ipad";
	}


	InitMenuWidgetEntity(_menu);

	InitLevel();

	return 1;
}

void MenuSelect::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
}

void MenuSelect::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuSelect::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuSelect::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "back_btn")
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, SELECTION_LEVEL_MENU);
	}
	else if (name.Contains("button_"))
	{
		int currentIdxBtn = atoi(name.Split("_").GetAt(1).GetString());

		int current_idx_layout = p_page_view->GetCurrentIdxSelected();

		int actual_idx_btn = currentIdxBtn + current_idx_layout * 9;

		int current_level = (int)(File::SaveMgr->GetDataSave()->GetDataByName(SAVE_LEVEL_UNLOCK));
		if (current_level > actual_idx_btn)
		{
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_ENTER_AP", 0.1f, 1);
#endif
			MenuMgr->SwitchToMenu(HUD_MENU, SELECTION_LEVEL_MENU);
			MenuMgr->InitActionPhase(actual_idx_btn);

			int id_music_play = GetRandom(1, 3);

			GetSound->PlayMusicEffect("MUSIC_AP_" + std::to_string(id_music_play),false);
		}
		else
		{
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_BTN_DENY", 0.1f, 1);
#endif
			MenuMgr->OnShowPopUp("ERROR !", "This level hasn't been unlocked !\nFinish Previous level and try again", 1);
		}

		p_page_view->SetCurrentIdxSelected(-1);
	}
}

void MenuSelect::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(SELECTION_LEVEL_MENU);
}

void MenuSelect::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuSelect::LoadLevelParam(TiXmlDocument * p_ObjectXML)
{
	if (!p_ObjectXML)
	{
		CCASSERT(false, "object invalid!");
		return;
	}
	auto root = p_ObjectXML->FirstChildElement("GAME");
	if (!root)
		return;

	auto Levelparam = root->FirstChildElement("GameLevel");

	if (Levelparam)
	{
		auto level_detail = Levelparam->FirstChildElement();
		RKString out_put_xml = "\n";
		do
		{
			if (level_detail)
			{
#ifdef RECORRECT_HINT
                if(p_number_level > 24)
                {
                    GameLevelDec * p_level_detail = APManager::CreateLevelDec(level_detail);
                    PASSERT2(p_level_detail, "the level detail is null");
                    RKString output = "\n CURRENT LEVEL :" + std::to_string(p_number_level + 1) + "WITH RC-HINT: ";
                    
                    for (int i = 0; i < p_level_detail->hint.size(); i++)
                    {
                        int value = RecorrectHint(p_level_detail->hint.at(i),p_level_detail->value.size());
                        
                        output += std::to_string(value) + (i < (p_level_detail->hint.size() - 1) ? "_" : "");
                    }
                    PINFO(output.GetString());
                    
                }
#endif
                
                
#ifdef USE_RANDOM_LEVEL
				if (p_number_level > 50)
				{
					GameLevelDec * p_level_detail = APManager::CreateLevelDec(level_detail);
					PASSERT2(p_level_detail, "the level detail is null");

					p_level_detail->number_start_random = p_number_level / (GetRandom(3, 5));
					float start_x = (float)p_level_detail->number_start_random / (float)GetRandom(2, 4);
					p_level_detail->range_random_number = Vec2(
						start_x, start_x * (GetRandom(15, 30) / 10.f)
						);

					bool have_move_diagonally = GetRandom(1, 3) >= 3;
					p_level_detail->move_diagonally = have_move_diagonally;
					if (have_move_diagonally)
					{
						int current_gen_move = GetRandom(0, 101 - p_number_level);
						bool Have_limit_move_dia = current_gen_move < 20;
						if (Have_limit_move_dia)
						{
							p_level_detail->number_move_diagonnally = ((current_gen_move / 3 > 2) ? (current_gen_move / 3) : 2);
						}
						else
						{
							p_level_detail->number_move_diagonnally = -2;
						}
					}

					UnitManager::GenRandomLevel(p_level_detail);
					out_put_xml += "\n";
					out_put_xml += "<Level idx = \"" + std::to_string(p_number_level) + "\"" + " use_diagonally= \"" + (have_move_diagonally ? "1\"> \n" : "0\"> \n");
					for (int i = 0; i < p_level_detail->value.size(); i++)
					{
						RKString detail_line = "     <l value =\"";
						for (int j = 0; j < p_level_detail->value.at(i).size(); j++)
						{
							int value = p_level_detail->getValueAt(j, i);
							detail_line += std::to_string(value) + (j < (p_level_detail->value.at(i).size() - 1) ? "_" : "");
						}
						detail_line += "\"/> \n";
						out_put_xml += detail_line;
					}
					out_put_xml += "     <hint value =\"";

					for (int i = 0; i < p_level_detail->hint.size(); i++)
					{
						int value = p_level_detail->hint.at(i);
						out_put_xml += std::to_string(value) + (i < (p_level_detail->hint.size() - 1) ? "_" : "");
					}
					out_put_xml += "\"/> \n";

					if (have_move_diagonally && p_level_detail->number_move_diagonnally > -2)
					{
						out_put_xml += "     <move_diagonally value =\"" + std::to_string(p_level_detail->number_move_diagonnally) + "\" /> \n";
					}
					out_put_xml += "</Level> \n";

				}

#endif
				p_number_level++;

				level_detail = level_detail->NextSiblingElement();
			}
		} while (level_detail);

		PDBG(out_put_xml.GetString());
	}

}


void MenuSelect::LoadDataLevelFromXML()
{
	TiXmlDocument * p_ObjectXML = NULL;
	p_ObjectXML = new TiXmlDocument();

	bool p_IsUseArkXML = false;
#ifdef USE_ARK_ARCHIVE
	p_IsUseArkXML = true;

	if (p_IsUseArkXML)
	{
		tinyxml2::XMLError error = p_ObjectXML->LoadFileArk(std::string(XML_DIR) + std::string("GamePlayLevel.xml"));
		if (error == tinyxml2::XMLError::XML_NO_ERROR)
		{
			LoadLevelParam(p_ObjectXML);
		}
		else
		{
			p_IsUseArkXML = false;
		}
	}
#endif
	if (!p_IsUseArkXML)
	{
		FILE * f = File::GetFile((std::string(XML_DIR) + std::string("GamePlayLevel.xml")).c_str(), "rb");
		if (f)
		{
			p_ObjectXML->LoadFile(f);
			LoadLevelParam(p_ObjectXML);
			delete p_ObjectXML;
			p_ObjectXML = nullptr;
			fclose(f);
		}
	}
}

void MenuSelect::OnBeginFadeIn()
{
	int LevelReach = (int)(File::SaveMgr->GetDataSave()->GetDataByName(SAVE_LEVEL_UNLOCK));

	int current_idx_goto = LevelReach / 9;

	//p_list_view = static_cast<ListViewWidget*>(GetWidgetChildByName("layer_select.layout_select.list_game_level"));

	p_page_view->JumpToItemIdx(current_idx_goto);
}

void MenuSelect::InitLevel()
{
	LoadDataLevelFromXML();

	int LevelReach = (int)(File::SaveMgr->GetDataSave()->GetDataByName(SAVE_LEVEL_UNLOCK));
	if (LevelReach == 0)
	{
		LevelReach = 1;
		File::SaveMgr->GetDataSave()->SetDataAtName(SAVE_LEVEL_UNLOCK, LevelReach);
		//File::SaveMgr->SaveTheGameSave();
	}

	p_page_view = static_cast<ListViewWidget*>(GetWidgetChildByName("layer_select.layout_select.page_game_level"));

	int number_page = p_number_level / 9;

	int number_at_end = p_number_level % 9;

	if (p_page_view)
	{

		for (int i = 0; i < number_page + 1; i++)
		{
			if (number_at_end == 0 && i == number_page)
			{
				continue;
			}

			auto widget_view = p_page_view->CreateWidgetRefer(i);

			int max_button_in_widget = 9;

			if (widget_view)
			{

				for (int j = 0; j < max_button_in_widget; j++)
				{
					RKString name_buton = ".button_" + std::to_string(j);
					auto current_btn = widget_view->GetWidgetChildByName(name_buton);
					PASSERT2(current_btn != nullptr, "widget not found");

					auto title_1 = current_btn->GetWidgetChildByName(".number_title");
					RKString value_idx = "";
					int number = (i * max_button_in_widget) + (j + 1);
					value_idx = std::to_string(number);
					if (number < 10)
					{
						value_idx = "0" + value_idx;
					}
					static_cast<TextWidget*>(title_1)->SetText(value_idx);
					if (LevelReach > 0)
					{
						current_btn->GetWidgetChildByName(".btn_start_level")->Visible(true);
						current_btn->GetWidgetChildByName(".lock_icon")->Visible(false);
					}
					else
					{
						current_btn->GetWidgetChildByName(".btn_start_level")->Visible(false);
						current_btn->GetWidgetChildByName(".lock_icon")->Visible(true);
					}

					LevelReach--;

					if (i == number_page)
					{
						if (number_at_end <= 0)
						{
							current_btn->Visible(false);
						}
						number_at_end--;
					}

					if (LevelReach <= 0)
					{
						LevelReach = 0;
					}
				}
			}
		}

		//visible first button
		auto button1 = p_page_view->GetWidgetChildByName(".layout_list0.button_0");
		button1->GetWidgetChildByName(".btn_start_level")->Visible(true);
		button1->GetWidgetChildByName(".lock_icon")->Visible(false);
	}
}

void MenuSelect::UnlockLevelIdx(int level)
{
	int number_page = level / 9;

	int number_at_end = level % 9;

	//p_page_view = static_cast<PageViewWidget*>(GetWidgetChildByName("layer_select.layout_select.page_game_level"));

	//if (p_page_view)
	{
		RKString layout_list = ".layout_list" + std::to_string(number_page);
		RKString button_name = ".button_" + std::to_string(number_at_end);

		p_page_view->GetWidgetChildByName(layout_list + button_name + ".btn_start_level")->Visible(true);
		p_page_view->GetWidgetChildByName(layout_list + button_name + ".lock_icon")->Visible(false);
		

	}
}