#include "UnitManager.h"
#include "InputManager.h"
#include "DisplayView.h"
#include "../Menu/MenuManager.h"
#include "SoundManager.h"
#include "../Menu/MenuHUD.h"

UnitManager::UnitManager()
{
	p_current_index_object_controll = -1;

	p_currentStateIdxLink = nullptr;

	p_current_number_object_have_lock.Clear();
	p_listFlyTextNotice.clear();

	p_IsUseHintShow = false;

	p_current_state_game = STATE_NONE;

	p_hand_hint = nullptr;
	p_mini_circle_hint = nullptr;

	p_diagonally_move = false;

	p_current_move_diagonally_remain = -2;
	p_number_move_dia = -2;

	p_number_failed = 0;
    
    p_IsLevelCreation = false;
    p_count_move_diagonally = 0;
	is_zoom_effect = false;
	p_current_node_have_effect = 0;
}

UnitManager::~UnitManager()
{
	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
		(p_ListControllableObject.at(i))->autorelease();
	}
	p_ListControllableObject.clear();

	p_List_current_idx_link.clear();

	for (int i = 0; i < p_listObjectLinkHL.size(); i++)
	{
		(p_listObjectLinkHL.at(i))->autorelease();
	}
	p_listObjectLinkHL.clear();

	p_current_number_object_have_lock.Clear();

	for (int i = 0; i < p_listFlyTextNotice.size(); i++)
	{
		(p_listFlyTextNotice.at(i))->autorelease();
	}
	p_listFlyTextNotice.clear();

	p_IsUseHintShow = false;

	p_color_hight_light.clear();
	p_gameplay_dec.clear();

	if (p_currentStateIdxLink)
	{
		delete[] p_currentStateIdxLink;
		p_currentStateIdxLink = nullptr;
	}

	p_hand_hint->autorelease();
	p_hand_hint = nullptr;

	p_mini_circle_hint->autorelease();
	p_mini_circle_hint = nullptr;

	p_number_failed = 0;
	p_current_move_diagonally_remain = -2;
	p_number_move_dia = -2;
    
    p_IsLevelCreation = false;
    p_count_move_diagonally = 0;
	p_current_node_have_effect = 0;
}

void UnitManager::GenStepLock(int& chance_each_, int& number_lock, Vec2 step_range, xml::BasicDec* __step_lock)
{
	bool _object_have_lock = GetRandom(0, 100) < chance_each_;
	if (_object_have_lock)
	{
		int ste_to_lock = GetRandom((int)step_range.x, (int)step_range.y);

		//p->InitStepLock(ste_to_lock, __step_lock->GetDataInt("font_idx"), __step_lock->GetDataInt("font_size")* GetGameSize().width / GetGameDesignSize().x, __step_lock->GetDataColor("font_color"));
		p_current_number_object_have_lock.Insert(p_ListControllableObject.size(), p_current_number_object_have_lock.Size());
		number_lock--;

		chance_each_ = __step_lock->GetDataInt("chance_have_lock_each_object");
	}
	else
	{
		chance_each_ += __step_lock->GetDataInt("step_add_each_chance");
	}
}

int UnitManager::InitStepLock(int &chance_each)
{
	auto __step_lock = GetGamePlayDec("step_lock");
	int __step_lock_chance_appear = __step_lock->GetDataInt("chance_appear");
	int __step_lock_number_max = __step_lock->GetDataInt("number_max");
	Vec2 __step_lock_range_step = __step_lock->GetDataVector2("step");

	int number_lock = 0;
	bool HaveLock__ = GetRandom(0, 100) < __step_lock_chance_appear;

	if (HaveLock__)
	{
		number_lock = GetRandom(1, __step_lock_number_max);
		chance_each = __step_lock->GetDataInt("chance_have_lock_each_object");
	}

	return number_lock;
}

bool UnitManager::InitAllObjectWithParam(GameLevelDec * game_mode_level)
{
	
	if (p_currentStateIdxLink)
	{
		delete[] p_currentStateIdxLink;
		p_currentStateIdxLink = nullptr;
	}

	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
		p_ListControllableObject.at(i)->autorelease();
	}
	p_ListControllableObject.clear();

	p_List_current_idx_link.clear();

	for (int i = 0; i < p_listObjectLinkHL.size(); i++)
	{
		(p_listObjectLinkHL.at(i))->autorelease();
	}
	p_listObjectLinkHL.clear();

	p_listFlyTextNotice.clear();

	if (p_hand_hint)
	{
		p_hand_hint->autorelease();
		p_hand_hint = nullptr;
	}
	p_hand_hint = new BasicUnit();
	p_hand_hint->OnInitWideObject("hint_obj");

	if (p_mini_circle_hint)
	{
		p_mini_circle_hint->autorelease();
		p_mini_circle_hint = nullptr;
	}
	p_mini_circle_hint = new BasicUnit();
	p_mini_circle_hint->OnInitWideObject("hint_mini_node");


	p_IsUseHintShow = false;
	
	p_diagonally_move = game_mode_level->move_diagonally;

	static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->OnShowDiagonalIcon(p_diagonally_move);
	static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->SetNumberMoveDia(game_mode_level->number_move_diagonnally);

	p_current_move_diagonally_remain = game_mode_level->number_move_diagonnally;
	p_number_move_dia = game_mode_level->number_move_diagonnally;

	if (!game_mode_level)
	{
		PASSERT2(game_mode_level, "param null!");
		return false;
	}

	p_size_list_controllable_object = Vec2(game_mode_level->value.at(0).size(), game_mode_level->value.size());


	p_current_number_object_have_lock.Clear();

	p_number_failed = 0;
    p_count_move_diagonally = 0;
    
    p_IsLevelCreation = game_mode_level->Is_level_for_creation;

	RKString refix = "";
	if (game_mode_level->value.size() == 5)
	{
		refix = "_5x";
	}

	float p_distance_object = GetGamePlayDec("detail_object_control" + refix)->GetDataInt("distance_object");
	Vec2 p_first_place = GetGamePlayDec("detail_object_control" + refix)->GetDataVector2("first_place");
	//
	p_current_state_game = STATE_GAME::STATE_NONE;

	p_current_node_have_effect = 0;

	int size_width = (int)p_size_list_controllable_object.x;
	int size_height = (int)p_size_list_controllable_object.y;

	if (game_mode_level->number_start_random > 0)
	{
		 GenRandomLevel(game_mode_level);
	}

	p_currentStateIdxLink = new int[size_width * size_height];

	//int idx_state_link = 0;
	for (int i = 0; i < size_width; i++)
	{
		for (int j = 0; j < size_height; j++)
		{
				ControllableUnit * p = new ControllableUnit();
            
				p->Init();

				Vec2 OriginSize = p->GetValueContain()->GetDataVector2("size_origin");

				p->SetPosition((
					Vec2((p_first_place.x + OriginSize.x / 2) + i * (p_distance_object + OriginSize.x),
					GetGameSize().height - ((p_first_place.y + OriginSize.y / 2) + j * (p_distance_object + OriginSize.y))
					)));

				p->SetIdxOnTable(i, j);
				

				int current_value = 0;
				current_value = game_mode_level->getValueAt(i, j);
				if (current_value == 0)
				{
					p->SetActive(false);
				}
				else
				{
					p->SetColorBG(GetColorHLAtIdx(current_value - 1));
				}

            p_ListControllableObject.pushBack(p);
				//
            p_currentStateIdxLink[i*size_width + j] = 0;
		}
	}

	//init step lock
	for (auto it = game_mode_level->index_have_lock.begin(); it != game_mode_level->index_have_lock.end(); it++)
	{
		int idx_obj_lock = it->first;
		p_ListControllableObject.at(idx_obj_lock)->InitStepLock(it->second);
		p_current_number_object_have_lock.Insert(idx_obj_lock, p_current_number_object_have_lock.Size());
	}


	return true;
}

void UnitManager::DrawAllObject(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	for (int i = 0; i < p_listObjectLinkHL.size(); i++)
	{
		p_listObjectLinkHL.at(i)->Draw(renderer, transform, flags);
	}

	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
		p_ListControllableObject.at(i)->Draw(renderer, transform, flags);
	}

	if (p_hand_hint)
	{
		p_hand_hint->Draw(renderer, transform, flags);
	}
	if (p_mini_circle_hint)
	{
		p_mini_circle_hint->Draw(renderer, transform, flags);
	}

	for (int i = 0; i < p_listFlyTextNotice.size(); i++)
	{
		p_listFlyTextNotice.at(i)->Draw(renderer, transform, flags);
	}
}

void UnitManager::VisitAllObject(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	for (int i = 0; i < p_listObjectLinkHL.size(); i++)
	{
		p_listObjectLinkHL.at(i)->Visit(renderer, transform, flags);
	}

	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
		p_ListControllableObject.at(i)->Visit(renderer, transform, flags);
	}

	if (p_hand_hint)
	{
		p_hand_hint->Visit(renderer, transform, flags);
	}
	if (p_mini_circle_hint)
	{
		p_mini_circle_hint->Visit(renderer, transform, flags);
	}

	for (int i = 0; i < p_listFlyTextNotice.size(); i++)
	{
		p_listFlyTextNotice.at(i)->Visit(renderer, transform, flags);
	}


}

void UnitManager::UpdateAllObject(float dt)
{

	if (p_number_failed >= 2)
	{
		static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->OnShowVideoAdsBtn();
		p_number_failed = 0;
	}

	if (MenuMgr->GetCurrentMenuLayer() == MENU_LAYER::HUD_MENU)
	{
		PerformLinkObjectTogether();
	}
	//PerformDragAndMoveObject();

	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
		p_ListControllableObject.at(i)->Update(dt);
	}	

	for (int i = 0; i < p_listObjectLinkHL.size(); i++)
	{
		p_listObjectLinkHL.at(i)->Update(dt);
	}


	std::vector<int> p_listTextFinishFade;
	for (int i = 0; i < p_listFlyTextNotice.size(); i++)
	{
		p_listFlyTextNotice.at(i)->Update(dt);
		/*if (p_listFlyTextNotice.at(i)->GetActionProcessRemain() == 0)
		{
			p_listTextFinishFade.push_back(i);
		}*/
	}

	if (p_hand_hint)
	{
		p_hand_hint->Update(dt);
	}
	if (p_mini_circle_hint)
	{
		p_mini_circle_hint->Update(dt);
	}
}

void UnitManager::OnProcessNextLevel(int current_number)
{

}

int UnitManager::IsFinishLevel()
{
	auto list_correct = MenuMgr->ApMgr()->GetCurrentLevel()->value;

	int count_idx = 0;
	for (int i = 0; i < list_correct.size(); i++)
	{
		for (int j = 0; j < list_correct.at(i).size(); j++)
		{
            
			if (list_correct.at(j).at(i) > 0 && p_currentStateIdxLink[count_idx] != list_correct.at(j).at(i))
			{
				return 0;
			}
			count_idx++;
		}
	}


	return 1;
}

void UnitManager::PerformLinkObjectTogether()
{

	if (InputMgr->GetNumberTouch() > 0 && (p_current_state_game != STATE_NEXT_STEP && p_current_state_game != STATE_GAME::STATE_FINISH && p_current_state_game != STATE_GAME::STATE_FAILED))
	{
		if (InputMgr->GetStateTouchOnScreen() == Control::STATE_HOLD)
		{
			Vec2 point_touch = InputMgr->GetPosAtId(0);
            
			for (int i = 0; i < p_ListControllableObject.size(); i++)
			{
				bool IsPressOnObject = InputMgr->IsPressOnBound(p_ListControllableObject.at(i)->GetBound());
				
                
                if (IsPressOnObject && (
					p_List_current_idx_link.size() == 0 || IsTheIdxNearThePreviousIdx(i)
					))
				{
					if (p_ListControllableObject.at(i)->IsLock() ||
                        !p_ListControllableObject.at(i)->IsActive() 
						)
					{
						continue;
					}
#if USE_CC_AUDIO
					GetSound->PlaySoundEffect("SFX_HL_UNIT", 0.05f, 1);
#endif
                    int idx = InsertIdxLink(i);

				//	PINFO("the object at idx:%i hight light", i);
				//	PINFO("the object at idx:%i have count", idx);

					p_ListControllableObject.at(i)->SetColor(GetColorHLAtIdx(idx));
					p_ListControllableObject.at(i)->SetOnControl(true);

					p_currentStateIdxLink[i] = (idx + 1);

					p_current_state_game = STATE_MOVE;


					for (int j = 0; j < p_current_number_object_have_lock.Size(); j++)
					{
						int idx_have_lock = p_current_number_object_have_lock.GetAt(j);
						p_ListControllableObject.at(idx_have_lock)->SetstepLockRemain();
					}

					//check is lost
					if (idx + 1 >= 2 && !p_IsLevelCreation)
					{
						size_t size_wd = MenuMgr->ApMgr()->GetCurrentLevel()->value.size();
						int y_idx = i % size_wd;
						int x_idx = i / size_wd;
						if (p_currentStateIdxLink[i] > MenuMgr->ApMgr()->GetCurrentLevel()->getValueAt(x_idx, y_idx))
						{
							p_current_state_game = STATE_FAILED;
							p_number_failed++;
							PINFO("Failed Level !");
							return;
						}
					}
					//check win
					if (!p_IsLevelCreation && IsFinishLevel() == 1)
					{
						p_current_state_game = STATE_FINISH;
					}

				}

			}
		}
	}
	else if (InputMgr->GetNumberTouch() <= 0 || p_current_state_game == STATE_GAME::STATE_FAILED)
	{
		if (p_List_current_idx_link.size() > 0)
		{
            RKString output_hint = "";
            
            int ** p_level_create = nullptr;
            if(p_IsLevelCreation)
            {
                p_level_create = new int * [(int)p_size_list_controllable_object.x];
                for(int i = 0 ; i < (int)p_size_list_controllable_object.x ; i ++)
                {
                    p_level_create[i] = new int [(int)p_size_list_controllable_object.y];
                    for(int j = 0 ; j < (int)p_size_list_controllable_object.y ; j++)
                    {
                        p_level_create[i][j] = 0;
                    }
                }
            }
            
			for (int i = 0; i < p_List_current_idx_link.size(); i++)
			{
				int idx = p_List_current_idx_link.at(i);
                
                if(p_IsLevelCreation)
                {
                    output_hint += std::to_string(idx);
                    output_hint += ((i < p_List_current_idx_link.size() - 1) ? "_" : "");
                    
                    p_level_create[idx % (int)p_size_list_controllable_object.y][idx / (int)p_size_list_controllable_object.x] += 1;
                }

				p_ListControllableObject.at(idx)->SetOnControl(false);

				if (p_current_state_game == STATE_GAME::STATE_MOVE || p_current_state_game == STATE_GAME::STATE_NONE)
				{
					p_current_state_game = STATE_NONE;
				}

				p_currentStateIdxLink[idx] = 0;
				for (int j = 0; j < p_current_number_object_have_lock.Size(); j++)
				{
					int idx_have_lock = p_current_number_object_have_lock.GetAt(j);
					p_ListControllableObject.at(idx_have_lock)->SetstepLockRemain(true);
				}
			}

#ifdef DEBUG
			if (p_IsLevelCreation)
			{
				PDBG(("\n the hint is: " + output_hint).GetString());
				PDBG("\n the level is: ");
				RKString output_level = "\n";
				for (int idx_1 = 0; idx_1 < (int)p_size_list_controllable_object.x; idx_1++)
				{
					output_level += "         <l value= \" ";
					for (int idx_2 = 0; idx_2 < (int)p_size_list_controllable_object.y; idx_2++)
					{
						output_level += std::to_string(p_level_create[idx_1][idx_2]);
						output_level += ((idx_2 < (int)p_size_list_controllable_object.y - 1) ? "_" : "");
					}
					output_level += " \"/> \n";
				}
				PDBG(output_level.GetString());
				PDBG(("\n number move diagonally: " + std::to_string(p_count_move_diagonally)).c_str());

				delete p_level_create;
				p_level_create = nullptr;

			}
#endif

#if USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_BTN_DENY", 0.05f, 1);
#endif

			p_current_move_diagonally_remain = p_number_move_dia;
			static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->SetNumberMoveDia(p_current_move_diagonally_remain);

		}
        if(p_current_state_game == STATE_FINISH)
        {
#ifdef DEBUG
			RKString log_debug_correct = "";
			for (int i = 0; i < p_List_current_idx_link.size(); i++)
			{
				log_debug_correct += (std::to_string(p_List_current_idx_link.at(i)) + "_");
			}
			PDBG(RKString("HINT level" + std::to_string(MenuMgr->ApMgr()->GetCurrentLevelNumber()) + ": " + log_debug_correct).GetString());
#endif

			Vec2 pos_hud_to = static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->GetWorldPosHUDBtnMenu();
			Vec2 design_size = Vec2(0, 0);
			if (GetGameSize().width / GetGameSize().height > 0.68)
			{
				//ipad
				design_size = Vec2(768, 1024);
			}
			else
			{
				design_size = Vec2(480, 800);
			}
			Vec2 pos_hud_to_origin = Vec2((pos_hud_to.x / GetGameSize().width) * design_size.x, (pos_hud_to.y / GetGameSize().height) * design_size.y);

			for (int i = 0; i < p_ListControllableObject.size(); i++)
			{
				if (p_ListControllableObject.at(i)->IsActive())
				{
					auto current_sl_node = p_ListControllableObject.at(i)->GetObjectSL();

					Vec2 pos_hud_with_node = p_ListControllableObject.at(i)->GetResource()->convertToNodeSpace(pos_hud_to);
                    
                    pos_hud_with_node = Vec2((pos_hud_with_node.x / GetGameSize().width) * design_size.x, (pos_hud_with_node.y / GetGameSize().height) * design_size.y);

					Vec2 current_pos = current_sl_node->GetPosition();
					
					current_sl_node->SetHookCommand(
						"MOVE_JUMP", "act1", 0, 0, [&]()->xml::BasicDec{
						xml::BasicDec value; value.InsertDataVector2("JUMP_TO", pos_hud_with_node);
						return value;}(), "JUMP_TO"
						);

					float time = ((float)GetRandom(50, 100)) / 200.f;
					current_sl_node->SetHookCommand(
						"MOVE_JUMP", "act1", 0, 0, [&]()->xml::BasicDec{
						xml::BasicDec value; 
						value.InsertDatafloat("TIME", time);
						return value; }(), "TIME"
							);
					current_sl_node->SetHookCommand(
						"MOVE_JUMP", "act1", 0, 1, [&]()->xml::BasicDec{
						xml::BasicDec value;
						value.InsertDatafloat("TIME", time);
						return value; }(), "TIME"
							);
					current_sl_node->SetHookCommand(
						"MOVE_JUMP", "act1", 0, 2, [&]()->xml::BasicDec{
						xml::BasicDec value;
						value.InsertDatafloat("TIME", time);
						return value; }(), "TIME"
							);
					current_sl_node->SetActionCommandWithName("MOVE_JUMP", CallFunc::create(
						[&, i, current_pos]()
						{
							p_ListControllableObject.at(i)->GetObjectSL()->EventActionFinish();

							p_ListControllableObject.at(i)->GetObjectSL()->GetResource()->setPosition(current_pos);
							p_current_node_have_effect--;
							if (p_current_node_have_effect <= 0)
							{
								static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->OnMakeBtnPauseZoom(
									CallFunc::create(
									[&,this]{
									this->p_current_state_game = STATE_NEXT_STEP;
								}));
								p_current_node_have_effect = 0;
							}
						}
					));
					p_current_node_have_effect++;
					current_sl_node->ClearHookCommand("MOVE_JUMP", "act1", 0, 0);


				}
			}

            p_current_state_game = STATE_NONE;
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_PROCESS_NEXT", 0.1f, 1);
#endif
            MenuMgr->ApMgr()->SetDelayTime();
            PINFO("Finish Level !");
        }
        else if (p_current_state_game == STATE_GAME::STATE_FAILED)
        {
#ifdef USE_CC_AUDIO
			GetSound->PlaySoundEffect("SFX_RELEASE_UNIT", 0.1f, 1);
#endif
            SetActionForControlObject("RESET");
            p_current_state_game = STATE_NONE;
        }
        
		p_List_current_idx_link.clear();
		p_listObjectLinkHL.clear();

	}
}

Vec2 UnitManager::CotainIdx(int idx)
{
	int idx_t = -1;
	int count = 0;
	for (int i = 0; i < p_List_current_idx_link.size(); i++)
	{
		if (p_List_current_idx_link[i] == idx) //find the idx in the list, return
		{
			idx_t = i;
			count++;
		}
	}
	return Vec2(idx_t, count);
}

int UnitManager::InsertIdxLink(int idx)
{
	int idx_ecount = -1;
	if (p_List_current_idx_link.size() <= 0)
	{
		p_List_current_idx_link.push_back(idx);
		idx_ecount = 0;
	}
	else
	{
		Vec2 id_count = CotainIdx(idx);
		if (id_count.x >= 0)
		{
			//PINFO("the index:%d have contain", id_count.x)
		}
		idx_ecount = id_count.y;

		//the idx not found, insert it at the end of the list
		p_List_current_idx_link.push_back(idx);

		//create the link texture
		size_t size_current_idx = p_List_current_idx_link.size();
		if (size_current_idx >= 2)
		{
			int idx_1 = p_List_current_idx_link.at(size_current_idx - 1);
			int idx_2 = p_List_current_idx_link.at(size_current_idx - 2);

			int idx_contain = -1;
			for (int i = 0; i < p_listObjectLinkHL.size(); i++)
			{
				Vec2 IdxObject = p_listObjectLinkHL.at(i)->IdxObjectContain;
				if (
                    (((int)IdxObject.x) == idx_1 && ((int)IdxObject.y) == idx_2)
                    ||
					(((int)IdxObject.x) == idx_2 && ((int)IdxObject.y) == idx_1)
					)
				{
					idx_contain = i;
					break;
				}
			}
			if (idx_contain >= 0 && idx_contain < p_listObjectLinkHL.size())
			{ 
				auto idx_object_contain = p_listObjectLinkHL.at(idx_contain);
				idx_object_contain->number_count++;
				int step_hl_color = idx_object_contain->number_count;
				idx_object_contain->SetColor(GetColorHLAtIdx(step_hl_color));

				//PINFO("hl link 2 object index:%i  and index:%i already exist ,now with count : %i", idx_2, idx_1, step_hl_color);
			}
			else if(idx_contain < 0)
			{
				//PINFO("link 2 object index:%i  and index:%i", idx_2, idx_1);
				HLLink * obj_link_hl = GenerateObjectLink(
					p_ListControllableObject.at(idx_1),
					p_ListControllableObject.at(idx_2));
                obj_link_hl->retain();
				obj_link_hl->IdxObjectContain = Vec2(idx_1, idx_2);
				obj_link_hl->number_count = 0;
				p_listObjectLinkHL.pushBack(obj_link_hl);
			}
		}
	}
	return idx_ecount;
}

bool UnitManager::IsTheIdxNearThePreviousIdx(int idx)
{
	if (p_List_current_idx_link.size() <= 0 || p_ListControllableObject.size() <= 0)
		return false;
	//get the pre idx hl
	int idxPrev = p_List_current_idx_link.at(p_List_current_idx_link.size() - 1);

	int x_pre = p_ListControllableObject.at(idxPrev)->GetIdxOnTable().x;
	int y_pre = p_ListControllableObject.at(idxPrev)->GetIdxOnTable().y;

	int x_cur = p_ListControllableObject.at(idx)->GetIdxOnTable().x;
	int y_cur = p_ListControllableObject.at(idx)->GetIdxOnTable().y;

	if ((x_pre == x_cur - 1 && y_pre == y_cur) 
		||(x_pre == x_cur + 1 && y_pre == y_cur) 
		||(y_pre == y_cur - 1 && x_pre == x_cur) 
		||(y_pre == y_cur + 1 && x_pre == x_cur) 

		|| (p_diagonally_move && 
			   (  (x_pre == x_cur - 1 && y_pre == y_cur - 1)
				||(x_pre == x_cur - 1 && y_pre == y_cur + 1)
				||(x_pre == x_cur + 1 && y_pre == y_cur - 1)
				||(x_pre == x_cur + 1 && y_pre == y_cur + 1)
				)
			)
		)
	{

		if ((x_pre == x_cur - 1 && y_pre == y_cur - 1)
			|| (x_pre == x_cur - 1 && y_pre == y_cur + 1)
			|| (x_pre == x_cur + 1 && y_pre == y_cur - 1)
			|| (x_pre == x_cur + 1 && y_pre == y_cur + 1)
			)
		{
            p_count_move_diagonally ++;
            if(p_IsLevelCreation)
            {
                p_current_move_diagonally_remain = -2;
                return true;
            }
            
			p_current_move_diagonally_remain--;
			if (p_current_move_diagonally_remain >= -1)
			{
				static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU))->SetNumberMoveDia(p_current_move_diagonally_remain);
			}
			if (p_current_move_diagonally_remain == -1)
			{
				p_current_move_diagonally_remain = 0;
				return false;
			}
			else if (p_current_move_diagonally_remain < -1)
			{
				p_current_move_diagonally_remain = -2;
			}
		}
	//	int id = CotainIdx(idx); //if the idx link-to have already contained in the list, so delete all the idx link after it
	//	RemoveTheAfterIdxLink(id);

		return true;
	}
	return false;
}		

void UnitManager::RemoveTheAfterIdxLink(int id)
{
	if (id >= 0)
	{
		RKList<int> p_idx_need_to_delete;
		for (int i = 0; i < p_List_current_idx_link.size(); i++)
		{
			if (i < id)
			{
				for (int j = 0; j < p_current_number_object_have_lock.Size(); j++)
				{
					int idx_have_lock = p_current_number_object_have_lock.GetAt(j);
					if (i == 0)
					{
						p_ListControllableObject.at(idx_have_lock)->SetstepLockRemain(true);
					}
					p_ListControllableObject.at(idx_have_lock)->SetstepLockRemain();
				}
			}
			else
			{
				int idx_link = p_List_current_idx_link.at(i);
				p_ListControllableObject.at(idx_link)->SetOnControl(false);
				//
				//
				p_idx_need_to_delete.Insert(i, 0);
			}
		}

		for (int i = 0; i < p_idx_need_to_delete.Size(); i++)
		{
			p_List_current_idx_link.erase(p_List_current_idx_link.begin() + p_idx_need_to_delete.GetAt(i));
			if (p_idx_need_to_delete.GetAt(i) > 0 && (p_idx_need_to_delete.GetAt(i) - 1) < p_listObjectLinkHL.size())
			{
				p_listObjectLinkHL.erase(p_idx_need_to_delete.GetAt(i) - 1);
			}
		}
	}
}

Vec2 UnitManager::GetNumberCotainOfObjectAtlocation(int x, int y)
{
	CCASSERT(x >= 0 && x < (int)p_size_list_controllable_object.x && y >= 0 && y < (int)p_size_list_controllable_object.y, "the x or y is out of stack !");
	for (int j = 0; j < p_ListControllableObject.size(); j++)
	{
		Vec2 IdxOnTable = p_ListControllableObject.at(j)->GetIdxOnTable();
		if (IdxOnTable.x == x && IdxOnTable.y == y)
		{
			int x_x = 0; // p_ListControllableObject.at(j)->GetNumberContain();
			int y_y = j;
			return Vec2(x_x, y_y);
		}
	}
	return Vec2(-1,-1);
}

void UnitManager::SetActionForControlObject(RKString name_action)
{
	for (int i = 0; i < p_ListControllableObject.size(); i++)
	{
        if(p_ListControllableObject.at(i)->IsActive())
            p_ListControllableObject.at(i)->SetActionCommandWithName(name_action);
	}
}

void UnitManager::ShowHintWithListIdx(std::vector<int> list)
{
	float time_move = 0.5f;
	float time_delay = 0.3f;

	p_hand_hint->GetResource()->setAnchorPoint(Vec2(0, 1));
	p_hand_hint->StopCurrentActionWithName("RUN_HINT");
	p_hand_hint->SetVisible(true);
	p_hand_hint->SetPosition(p_ListControllableObject.at(list.at(0))->GetPosition());
	Vector<FiniteTimeAction*> array_Action;
	for (int i = 0; i < list.size(); i++)
	{
		Vec2 pos = p_ListControllableObject.at(list.at(i))->GetPosition();

		auto object_control_i = p_ListControllableObject.at(list.at(i));

		auto action = MoveTo::create(time_move, pos);
		array_Action.pushBack(action);

		array_Action.pushBack(CallFunc::create([object_control_i](){
			object_control_i->SetActionCommandWithName("HINT_HL");
		}));

		auto action_delay = DelayTime::create(time_delay);
		array_Action.pushBack(action_delay);
	}
	array_Action.pushBack(CallFunc::create([&](){
		p_hand_hint->SetVisible(false);
	}));
	array_Action.pushBack(MoveTo::create(time_move, p_ListControllableObject.at(list.at(0))->GetPosition()));

	auto action = Sequence::create(array_Action);
	action->setTag(10);
	action->retain();
	p_hand_hint->SetActionCustomWithName("RUN_HINT", action);

	auto menu_hud = static_cast<MenuHUD*>(MenuMgr->GetMenuUI(HUD_MENU));

	p_mini_circle_hint->GetResource()->setAnchorPoint(Vec2(0.5f, 0.5f));
	p_mini_circle_hint->StopCurrentActionWithName("RUN_HINT");
	p_mini_circle_hint->SetVisible(true);
	p_mini_circle_hint->SetPosition(
		menu_hud->GetPosNodeTargetAtIdx(list.at(0),p_size_list_controllable_object.x)
		);
	Vector<FiniteTimeAction*> array_Action_1;
	for (int i = 0; i < list.size(); i++)
	{
		Vec2 pos = menu_hud->GetPosNodeTargetAtIdx(list.at(i),p_size_list_controllable_object.x);

		auto action = MoveTo::create(time_move, pos);
		array_Action_1.pushBack(action);

		auto action_delay = DelayTime::create(time_delay);
		array_Action_1.pushBack(action_delay);
	}
	array_Action_1.pushBack(CallFunc::create([&](){
		p_mini_circle_hint->SetVisible(false);
	}));
	array_Action_1.pushBack(MoveTo::create(time_move, menu_hud->GetPosNodeTargetAtIdx(list.at(0),p_size_list_controllable_object.x)));

	auto action_1 = Sequence::create(array_Action_1);
	action_1->setTag(10);
	action_1->retain();
	p_mini_circle_hint->SetActionCustomWithName("RUN_HINT", action_1);
}

HLLink* UnitManager::GenerateObjectLink(ControllableUnit* a, ControllableUnit* b)
{
	CCASSERT(a && b, "two unit must be valid !");

	Vec2 pos_a = a->GetPosition();
	Vec2 pos_b = b->GetPosition();

	Vec2 pos = Vec2((pos_a.x + pos_b.x) / 2, (pos_a.y + pos_b.y) / 2);

	HLLink* unit = new HLLink();

	unit->OnInitWideObject("link_obj_hl");
	unit->SetPosition(pos);
	unit->SetColor(p_color_hight_light.at(0));

	Vec2 size = unit->GetSize();
	size.x = sqrt(((pos_a.x - pos_b.x)*(pos_a.x - pos_b.x)) + ((pos_a.y - pos_b.y)*(pos_a.y - pos_b.y)));
	float angle = 0;

	if (b->GetIdxOnTable().x == a->GetIdxOnTable().x && b->GetIdxOnTable().y != a->GetIdxOnTable().y)
	{
		//90
		angle = 90;
	}
	else if (b->GetIdxOnTable().x != a->GetIdxOnTable().x && b->GetIdxOnTable().y == a->GetIdxOnTable().y)
	{
		//180
		angle = 180;
	}
	else if ((b->GetIdxOnTable().x > a->GetIdxOnTable().x && b->GetIdxOnTable().y > a->GetIdxOnTable().y) ||
			 (b->GetIdxOnTable().x < a->GetIdxOnTable().x && b->GetIdxOnTable().y < a->GetIdxOnTable().y))
	{
		//45
		angle = 45;
		//size.x = sqrt(3) * size.x;
	}
	else if ((b->GetIdxOnTable().x > a->GetIdxOnTable().x && b->GetIdxOnTable().y < a->GetIdxOnTable().y) ||
		     (b->GetIdxOnTable().x < a->GetIdxOnTable().x && b->GetIdxOnTable().y > a->GetIdxOnTable().y))
	{
		//135
		angle = 135;
		//size.x = sqrt(3) * size.x;
	}
	unit->SetSize(size);
	unit->SetAngle(angle);

	return unit;

}


Vec2 UnitManager::GetDirectMove(int t)
{
	switch (t)
	{
	case 0:
		return Vec2(1, 0);
		break;
	case 1:
		return Vec2(-1, 0);
		break;
	case 2:
		return Vec2(0, 1);
		break;
	case 3:
		return Vec2(0, -1);
		break;
	case 4:
		return Vec2(1, 1);
		break;
	case 5:
		return Vec2(-1, 1);
		break;
	case 6:
		return Vec2(1, -1);
		break;
	case 7:
		return Vec2(-1, -1);
		break;
	default:
		return Vec2(0, 0);
		break;
	}
}

void UnitManager::GenRandomLevel(GameLevelDec * level)
{
	vector<vector<int>> return_vector;

	int size_level = level->value.size();

	std::vector<Vec2> list_move;

	Vec2 random_location_first = Vec2(
		GetRandom(0, size_level - 1), GetRandom(0, size_level - 1)
		);

	list_move.push_back(random_location_first);
	int number_move = level->number_start_random + GetRandom((int)(level->range_random_number.x), (int)(level->range_random_number.y));

	int current_number_move_dia = level->number_move_diagonnally;
	do
	{
		int except_move = -1;
		do
		{

			int rand_move = 0;
			if (level->move_diagonally)
			{
				if (current_number_move_dia > 0 || current_number_move_dia < -1)
				{
					rand_move = GetRandom(0, 7, except_move);
				}
				else
				{
					rand_move = GetRandom(0, 3, except_move);
				}

				if (rand_move >= 4)
				{
					current_number_move_dia--;
					if (current_number_move_dia == -1)
					{
						current_number_move_dia = 0;
					}
					else if (current_number_move_dia < -1)
					{
						current_number_move_dia = -2;
					}
				}
			}
			else
			{
				rand_move = GetRandom(0, 3, except_move);
			}

			Vec2 state_move_rand = GetDirectMove(rand_move);

			Vec2 last_move_in_list = list_move.at(list_move.size() - 1);

			last_move_in_list.x = last_move_in_list.x + state_move_rand.x;
			last_move_in_list.y = last_move_in_list.y + state_move_rand.y;

			if (last_move_in_list.x < 0 || last_move_in_list.x >= size_level || last_move_in_list.y < 0 || last_move_in_list.y >= size_level)
			{
				except_move = rand_move;
				continue;
			}
			else
			{
				list_move.push_back(last_move_in_list);
				break;
			}

		} while (true);

		number_move--;

	} while (number_move > 0);

	int ** value_contain = nullptr;

	value_contain = new int *[size_level];
	for (int i = 0; i < size_level; i++)
	{
		value_contain[i] = new int[size_level];
		for (int j = 0; j < size_level; j++)
		{
			value_contain[i][j] = 0;
		}
	}

	level->hint.clear();
	for (int i = 0; i < list_move.size(); i++)
	{
		int current_value_idx = value_contain[(int)(list_move.at(i).x)][(int)(list_move.at(i).y)];
		current_value_idx++;
		value_contain[(int)(list_move.at(i).x)][(int)(list_move.at(i).y)] = current_value_idx;

		level->hint.push_back(list_move.at(i).y * level->value.size() + list_move.at(i).x);
	}

	for (int i = 0; i < size_level; i++)
	{
		level->value.at(i).clear();
	}
	level->value.clear();

	for (int i = 0; i < size_level; i++)
	{
		vector<int> it_value;
		for (int j = 0; j < size_level; j++)
		{
			int value = value_contain[i][j];
			it_value.push_back(value);
		}
		level->value.push_back(it_value);
	}


#if defined DEBUG && ! defined USE_RANDOM_LEVEL
	RKString output = "";
	for (int i = 0; i < size_level; i++)
	{
		output += "\n";
		for (int j = 0; j < size_level; j++)
		{
			int value = value_contain[i][j];
			output += std::to_string(value) + (j < (size_level - 1) ? "_" : "");
		}
	}
	PDBG(output.GetString());

	RKString output_1 = "\n";
	for (int i = 0; i < level->hint.size(); i++)
	{
		output_1 += std::to_string(level->hint.at(i)) + (i < (level->hint.size() - 1) ? "_" : "");
	}
	PDBG(output_1.GetString());
#endif

	for (int i = 0; i < size_level; i++)
	{
		delete[] value_contain[i];
	}
	delete[] value_contain;
	value_contain = nullptr;
}

void UnitManager::SetObjectFlyTo(RKString resource, Vec2 pos_from, Vec2 pos_to,int number, float size , Color4B color , float delay_first)
{
	if (resource == "")
		return;
#if USE_CC_AUDIO

#endif

	SetObjFlyFromPos(resource, "", pos_from, pos_to, "object_action_" + resource, number, size, color, delay_first);
}

void UnitManager::SetTextFlyTime(int number, Vec2 pos_from, float size, Color4B color, float delay_first)
{
	if (number <= 0)
		return;

#if USE_CC_AUDIO
#endif
	//Vec2 pos_to = ((MenuHUD*)MenuMgr->GetMenuUI(HUD_MENU))->GetResourceByName("Time_Remain")->GetPositionResource();

	//SetObjFlyFromPos("text_fly", "+ " + std::to_string(number) + " second", pos_from, pos_to, "text_fly_time", number, size, color, delay_first);

}

void UnitManager::SetTextFlyScore(int number, Vec2 pos_from, float size, Color4B color, float delay_first)
{
	if (number <= 0)
	{
		return;
	}

#if USE_CC_AUDIO
	if (number < 10)
	{
		RKString id_sound = "SFX_ADD_" + std::to_string(number) + "_SCORE";
		GetSound->PlaySoundEffect(id_sound, delay_first + 0.15f, 1);
	}
	else
	{
		GetSound->PlaySoundEffect("SFX_ADD_10_SCORE", delay_first + 0.15f, 1);
	}

#endif
	//Vec2 pos_to = ((MenuHUD*)MenuMgr->GetMenuUI(HUD_MENU))->GetResourceByName("number_score")->GetPositionResource();

	//SetObjFlyFromPos("text_fly", "+ " + std::to_string(number) + " score", pos_from, pos_to, "text_fly", number, size, color, delay_first);
}

void UnitManager::SetObjFlyFromPos(RKString resource, RKString text, Vec2 pos, Vec2 pos_to, RKString tag_name, int number, float size, Color4B color, float delay_first)
{

	float delta_move = 100;

	float w_design = GetGameDesignSize().x;

	float game_size_ratio = GetGameSize().width / GetGameSize().height;
	if (game_size_ratio >= 0.7) //ipad size
	{
		w_design = 768.f;
	}
	else //common smartphone size
	{
		w_design = GetGameDesignSize().x;
	}

	float size_scale_to = 0 * GetGameSize().width / w_design;
	float size_scale_to_1 = -30 * GetGameSize().width / w_design;

	delta_move = delta_move * GetGameSize().width / w_design;

	Vec2 pos_Hud_score = pos_to;

	Vec2 vector_move_to_score = Vec2(pos_Hud_score.x - pos.x, pos_Hud_score.y - pos.y);

	BasicUnit * p_text_fly = new BasicUnit(tag_name, number);
	//p_text_fly->OnInitFirstValueBasicObject(resource);
	if (size > 0)
	{
		p_text_fly->SetSize(Vec2(size, size));
	}
	p_text_fly->SetColor(color);
	p_text_fly->SetPosition(pos);
	p_text_fly->SetText(text);

	//p_text_fly->OnSetProcessMoveDelayByAction(Vec2(pos.x, pos.y + delta_move), 1, 1.0f, false, false, 0.f, 0.f + delay_first);
	//p_text_fly->OnSetProcessMoveDelayToAction(pos_Hud_score, 1, 0.5f, false, false, 0.0f, 1.0f + delay_first);
	//p_text_fly->OnSetProcessScaleAction(size_scale_to, 1, 0.5f, false, false, 0.f, 0.f + delay_first);
	//p_text_fly->OnSetProcessScaleAction(size_scale_to_1, 1, 0.5f, false, false, 0.0f, 0.5f + delay_first);
	//p_text_fly->OnSetProcessFadeAction(0, 255, 1, 0.75, false, true, 0.f, 0.f + delay_first);
	//p_text_fly->retain();
	p_listFlyTextNotice.insert(p_listFlyTextNotice.size(), p_text_fly);
}