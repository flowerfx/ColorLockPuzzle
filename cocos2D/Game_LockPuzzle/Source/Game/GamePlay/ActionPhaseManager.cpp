#include "ActionPhaseManager.h"
#include "../Menu/MenuManager.h"
#include "../Menu/MenuHUD.h"
#include "../Menu/MenuSelectionLevel.h"

#include "../Menu/MenuTutorial.h"

#include "DisplayView.h"

#include "SoundManager.h"
using namespace Utility;


APManager::APManager()
{
	p_value_xml = new xml::BasicDec();
	p_time_delay_next_level = 0;
	p_level_play = 0;

}

APManager::~APManager()
{
	//Scene * scene = Director::getInstance()->getRunningScene();
	//scene->removeChildByName("action_phase");

	delete p_unitManager;
	p_unitManager = NULL;

	for (int i = 0; i < p_levelGame.size(); i++)
	{
		delete p_levelGame.at(i);
	}

	p_levelGame.clear();

	if (p_value_xml)
	{
		delete p_value_xml;
		p_value_xml = nullptr;
	}

}

bool APManager::Init(int level)
{
	p_unitManager = new UnitManager();

	LoadDataAPFromXML();

	LoadDataLevelFromXML();

	InitNewTable(level);

//	auto scene = Director::getInstance()->getRunningScene();
//	if (scene)
//	{
//		this->setName("action_phase");
//		scene->addChild(this);
//	}


	return true;
}

bool APManager::InitNewTable(int level)
{
	p_level_play = level;

	p_unitManager->InitAllObjectWithParam(p_levelGame.at(level));

	p_unitManager->SetActionForControlObject("FADE_IN");
#if USE_CC_AUDIO
	GetSound->PlaySoundEffect("SFX_UNIT_FADE", 0.01f, 1);
#endif
	
	return true;
}

void APManager::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_unitManager)
	{
		p_unitManager->DrawAllObject(renderer, transform, flags);
	}
}

void APManager::visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (p_unitManager)
	{
		p_unitManager->VisitAllObject(renderer, transform, flags);
	}
}

void APManager::Update(float dt)
{
	if (p_unitManager)
	{
		p_unitManager->UpdateAllObject(dt);

		if (p_unitManager->GetCurrentGameState() == STATE_GAME::STATE_NEXT_STEP)
		{
			/*p_time_delay_next_level -= dt;
			if (p_time_delay_next_level <= 0)
			{
				p_time_delay_next_level = 0;
			}

			if (p_time_delay_next_level <= 0)*/
			{

				if (p_level_play >= p_levelGame.size() - 1)
				{
					MenuMgr->SwitchToMenu(RESULT_MENU, HUD_MENU);

					p_unitManager->ResetCurrentGameState();
				}
				else
				{

					p_level_play++;

					InitNewTable(p_level_play);
					if ((p_level_play + 1) >= File::SaveMgr->GetDataSave()->GetDataByName(SAVE_LEVEL_UNLOCK))
					{
						File::SaveMgr->GetDataSave()->SetDataAtName(SAVE_LEVEL_UNLOCK, (p_level_play + 1));
						File::SaveMgr->SaveTheGameSave();

#ifdef GAME_IOS
						MenuMgr->PostScoreToGameCenter(p_level_play + 1, "");
#endif

						static_cast<MenuSelect*>(MenuMgr->GetMenuUI(SELECTION_LEVEL_MENU))->UnlockLevelIdx((p_level_play));
					}
					static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->OnSetTargetPanel((p_level_play + 1), true);
					static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->OnMakeTitleLevelZoom();
				}
				//for the tutorial

				int current_step_tutorial = static_cast<MenuTutorial*>(MenuMgr->GetMenuUI(TUTORIAL_MENU))->GetCurrentStepTutorial();
				int save_step_tutorial = (int)File::SaveMgr->GetDataSave()->GetDataByName(SAVE_STR_TUTORIAL_STEP);
				if (save_step_tutorial < 0)
				{
					current_step_tutorial = -1;
				}

				if (save_step_tutorial >= current_step_tutorial)
				{
					current_step_tutorial = save_step_tutorial;
				}


				int current_level = GetCurrentLevelNumber();

				if (current_step_tutorial == 1 && current_level == 1)
				{
					MenuMgr->OnShowTutorialStep(2);
				}
				else if (current_step_tutorial == 2 && current_level == 2)
				{
					MenuMgr->OnShowTutorialStep(3);
				}
				else if (current_step_tutorial == 3 && current_level == 3)
				{
					MenuMgr->OnShowTutorialStep(4);
				}
				else if (current_step_tutorial == 4 && current_level == 4)
				{
					MenuMgr->OnShowTutorialStep(5);
				}
			}
			
		}
	}
}

void APManager::ShowHint()
{
	auto __list__ = p_levelGame.at(p_level_play)->hint;
	p_unitManager->ShowHintWithListIdx(__list__);
}

void APManager::LoadDataLevelFromXML()
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

void APManager::LoadDataAPFromXML()
{
	TiXmlDocument * p_ObjectXML = NULL;
	p_ObjectXML = new TiXmlDocument();

	bool p_IsUseArkXML = false;
#ifdef USE_ARK_ARCHIVE
	p_IsUseArkXML = true;

	if (p_IsUseArkXML)
	{
		tinyxml2::XMLError error = p_ObjectXML->LoadFileArk(std::string(XML_DIR) + std::string(W_OBJECT_DEC_XML));
		if (error == tinyxml2::XMLError::XML_NO_ERROR)
		{
			LoadAPParam(p_ObjectXML);
		}
		else
		{
			p_IsUseArkXML = false;
		}
	}
#endif
	if (!p_IsUseArkXML)
	{
		FILE * f = File::GetFile((std::string(XML_DIR) + std::string(W_OBJECT_DEC_XML)).c_str(), "rb");
		if (f)
		{
			p_ObjectXML->LoadFile(f);
			LoadAPParam(p_ObjectXML);
			delete p_ObjectXML;
			p_ObjectXML = nullptr;
			fclose(f);
		}
	}
}

GameLevelDec * APManager::CreateLevelDec(TiXmlElement * level_detail)
{
	if (!level_detail)
		return nullptr;

	GameLevelDec * _level = new GameLevelDec();
	if (level_detail->Attribute("use_diagonally"))
	{
		_level->move_diagonally = atoi(level_detail->Attribute("use_diagonally"));
	}

	auto line = level_detail->FirstChildElement("l");
	do
	{
		if (line)
		{
			std::vector<int> _value_row;

			RKString value = line->Attribute("value");
			auto list_value_row = value.Split("_");
			for (int i = 0; i < list_value_row.Size(); i++)
			{
				_value_row.push_back(atoi(list_value_row.GetAt(i).GetString()));
			}

			_level->value.push_back(_value_row);

			line = line->NextSiblingElement("l");
		}

	} while (line);

	line = level_detail->FirstChildElement("hint");
	if (line)
	{
		auto list_hint_idx = RKString(line->Attribute("value")).Split("_");

		for (int i = 0; i < list_hint_idx.Size(); i++)
		{
			_level->hint.push_back(atoi(list_hint_idx.GetAt(i).GetString()));
		}
	}

	line = level_detail->FirstChildElement("step_lock");
	if (line)
	{
		auto list_lock_idx = RKString(line->Attribute("value")).Split("#");

		for (int i = 0; i < list_lock_idx.Size(); i++)
		{
			auto detail_lock = list_lock_idx.GetAt(i).Split("_");
			int idx_have_lock = atoi(detail_lock.GetAt(0).GetString());
			int numberlock = atoi(detail_lock.GetAt(1).GetString());

			_level->index_have_lock.insert(std::pair<int, int>(idx_have_lock, numberlock));
		}
	}

	line = level_detail->FirstChildElement("move_diagonally");
	if (line)
	{
		_level->number_move_diagonnally = atoi(line->Attribute("value"));
	}

	line = level_detail->FirstChildElement("random");
	if (line)
	{
		_level->number_start_random = atoi(line->Attribute("number_start"));
		_level->range_random_number = XMLMgr->ConvertToVector2(line->Attribute("number_delta"));
	}
    
    line = level_detail->FirstChildElement("level_creation");
    if (line)
    {
        _level->Is_level_for_creation = (atoi(line->Attribute("value")) != 0);
    }

	return _level;
}

void APManager::LoadLevelParam(TiXmlDocument * p_ObjectXML)
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
		do
		{
			if (level_detail)
			{
				GameLevelDec * _level = CreateLevelDec(level_detail);
				PASSERT2(_level, "level detail is null");
				p_levelGame.push_back(_level);


				level_detail = level_detail->NextSiblingElement();
			}
		} while (level_detail);


	}
}

void APManager::LoadAPParam(TiXmlDocument * p_ObjectXML)
{
	if (!p_ObjectXML)
	{
		CCASSERT(false, "object invalid!");
		return;
	}
	auto root = p_ObjectXML->FirstChildElement("GAME");
	if (!root)
		return;

	auto APParam = root->FirstChildElement("ActionPhase");

	if (APParam)
	{

		RKString default_detail_obj_control = "detail_object_control";
		float getGameSizeX = GetGameSize().width;
		float getGameSizeY = GetGameSize().height;

		float cur_ratio = getGameSizeX / getGameSizeY;
		if (cur_ratio > 0.68f) //ipad ratio
		{
			default_detail_obj_control = "detail_object_control_ipad";
		}
		else if (cur_ratio <= 0.68 && cur_ratio > 0.65)
		{
			default_detail_obj_control = "detail_object_control_ip_retina";
		}

		auto detail_obj = APParam->FirstChildElement(default_detail_obj_control.GetString());
		if (detail_obj)
		{
			Vec2 design_size = XMLMgr->ConvertToVector2(detail_obj->Attribute("designSize"));

			auto __dec = XMLMgr->GenerateBasicDec(detail_obj, "first_place", "distance_object", NULL);


			auto first_place = __dec->GetDataVector2("first_place");
			first_place.x = (first_place.x / design_size.x) * GetGameSize().width;
			first_place.y = (first_place.y / design_size.y) * GetGameSize().height;
			__dec->ReplaceDataVector2("first_place", first_place);

			float distance_object = (float)__dec->GetDataInt("distance_object");
			distance_object = (distance_object / design_size.x) * GetGameSize().width;
			__dec->ReplaceDataInt("distance_object", (int)distance_object);

			p_unitManager->InsertGamePlayDec("detail_object_control", __dec);
		}

		default_detail_obj_control = "detail_object_control_5x";
		cur_ratio = GetGameSize().width / GetGameSize().height;
		if (cur_ratio > 0.68f) //ipad ratio
		{
			default_detail_obj_control = "detail_object_control_ipad_5x";
		}
		else if (cur_ratio <= 0.68 && cur_ratio > 0.65)
		{
			default_detail_obj_control = "detail_object_control_ip_retina_5x";
		}

		detail_obj = APParam->FirstChildElement(default_detail_obj_control.GetString());
		if (detail_obj)
		{
			Vec2 design_size = XMLMgr->ConvertToVector2(detail_obj->Attribute("designSize"));

			auto __dec = XMLMgr->GenerateBasicDec(detail_obj, "first_place", "distance_object", NULL);


			auto first_place = __dec->GetDataVector2("first_place");
			first_place.x = (first_place.x / design_size.x) * GetGameSize().width;
			first_place.y = (first_place.y / design_size.y) * GetGameSize().height;
			__dec->ReplaceDataVector2("first_place", first_place);

			float distance_object = (float)__dec->GetDataInt("distance_object");
			distance_object = (distance_object / design_size.x) * GetGameSize().width;
			__dec->ReplaceDataInt("distance_object", (int)distance_object);

			p_unitManager->InsertGamePlayDec("detail_object_control_5x", __dec);
		}


		detail_obj = APParam->FirstChildElement("color_hl");
		if (detail_obj)
		{
			auto color_hl = detail_obj->FirstChildElement();
			do
			{
				if (color_hl)
				{
					Color4B val = XMLMgr->ConvertToColor(color_hl->Attribute("value"));
					p_unitManager->InsertColorHL(val);

					color_hl = color_hl->NextSiblingElement();
				}

			} while (color_hl);
		}

		detail_obj = APParam->FirstChildElement("step_lock");
		if (detail_obj)
		{
			auto __dec = XMLMgr->GenerateBasicDec(detail_obj, "step", "number_max", "chance_appear", "chance_have_lock_each_object", "step_add_each_chance", "font_idx", "font_size", "font_color", NULL);
			p_unitManager->InsertGamePlayDec("step_lock", __dec);
		}

		detail_obj = APParam->FirstChildElement("time_param");
		if (detail_obj)
		{
			p_value_xml->InsertDatafloat("delay_each_level", atof(detail_obj->Attribute("delay_each_level")));
		}
	}
}

GameLevelDec * APManager::GetLevelAtIdx(int idx)
{
	if (idx < p_levelGame.size() && idx >= 0)
	{
		return p_levelGame.at(idx);
	}
	return nullptr;
}

void APManager::SetDelayTime()
{
	p_time_delay_next_level = p_value_xml->GetDatafloat("delay_each_level"); //1 sec
}