#include "ActionProcess.h"

#include "LayerEntity.h"

#include "WidgetEntity.h"
namespace Utility
{


	namespace UI_Widget
	{
		ActionProcess::ActionProcess()
		{
			p_action_process = 0;

			p_origin_value = new xml::BasicDec();
			p_first_value = new xml::BasicDec();

			p_origin_value->InsertDataVector2("pos", Vec2(0, 0));
			p_first_value->InsertDataVector2("pos", Vec2(0, 0));

			p_origin_value->InsertDatafloat("rotate",0);
			p_first_value->InsertDatafloat("rotate", 0);

			p_origin_value->InsertDataInt("alpha", 255);
			p_first_value->InsertDataInt("alpha", 255);

			p_origin_value->InsertDatafloat("scale", 1.f);
			p_first_value->InsertDatafloat("scale", 1.f);

			p_origin_value->InsertDataColor("tint", Color4B::WHITE);
			p_first_value->InsertDataColor("tint", Color4B::WHITE);


			p_Visible = true;
			menu_point_to = nullptr;
			p_parent_layer = nullptr;
			p_parent_widget = nullptr;

			p_currentStatusMenu = STATUS_MENU::M_IDLE;

			p_list_command.clear();

			p_IsInteractive = true;
		}
		
		ActionProcess::~ActionProcess()
		{
			CC_SAFE_DELETE(p_origin_value);
			CC_SAFE_DELETE(p_first_value);

			for (auto it = p_list_command.begin(); it != p_list_command.end(); it++)
			{
				delete p_list_command.at(it->first);
			}

			p_list_command.clear();

			menu_point_to = nullptr;
			p_parent_layer = nullptr;
			p_parent_widget = nullptr;
		}

#pragma region normal_method

		void ActionProcess::CloneThis(ActionProcess * p_clone)
		{
			p_clone->p_Name = p_Name;
#ifdef OS_IOS
            memcpy(p_clone->p_origin_value, p_origin_value, sizeof(p_clone->p_origin_value));
            memcpy(p_clone->p_first_value,  p_first_value,  sizeof(p_clone->p_first_value));
#else
			memcpy_s(p_clone->p_origin_value, sizeof(p_clone->p_origin_value), p_origin_value, sizeof(p_origin_value));
			memcpy_s(p_clone->p_first_value, sizeof(p_clone->p_first_value), p_first_value, sizeof(p_first_value));
#endif

			//PASSERT2(p_clone->p_list_command.size() == p_list_command.size(), "two value must be equal");

			for (auto it = p_list_command.begin(); it != p_list_command.end(); it++)
			{
				p_clone->p_list_command.insert(std::pair<RKString ,ActionCommand*>( it->first,it->second->Clone()));
			}
		}

		bool ActionProcess::Visible()
		{
			//return p_Visible;
			return GetResource()->isVisible();
		}

		void ActionProcess::Visible(bool b)
		{
			p_Visible = b;
			GetResource()->setVisible(b);
		}

		unsigned int ActionProcess::GetIndexAppear()
		{
			return p_index_appear;
		}

		void ActionProcess::SetIndexAppear(unsigned int idx)
		{
			p_index_appear = idx;
		}

		void ActionProcess::SetScale(cocos2d::Vec2 s)
		{
			cocos2d::Vec2 p_scale(1, 1);
			int mipmap = TextureMgr->GetLevelMipMap();

			p_scale.x = GetMulMul(s.x, mipmap);
			p_scale.y = GetMulMul(s.y, mipmap);


			GetResource()->setScaleX(p_scale.x);
			GetResource()->setScaleY(p_scale.y);
			
		}

		Vec2 ActionProcess::GetScale()
		{
			cocos2d::Vec2 p_scale(1, 1);
			int mipmap = TextureMgr->GetLevelMipMap();

			p_scale.x = GetMulDiv(GetResource()->getScaleX(), mipmap);
			p_scale.y = GetMulDiv(GetResource()->getScaleY(), mipmap);


			return p_scale;
		}

		Vec2 ActionProcess::GetSize()
		{
			cocos2d::Vec2 p_Size(1, 1);
			p_Size.x = GetResource()->getContentSize().width * GetResource()->getScaleX();
			p_Size.y = GetResource()->getContentSize().height * GetResource()->getScaleY();

			return p_Size;
		}

		void ActionProcess::SetSize(cocos2d::Vec2 s)
		{
			cocos2d::Vec2 p_Size(1, 1);

			p_Size.x = s.x / GetResource()->getContentSize().width ;
			p_Size.y = s.y / GetResource()->getContentSize().height;
			GetResource()->setScaleX(p_Size.x);
			GetResource()->setScaleY(p_Size.y);


		}

		void ActionProcess::SetRotate(float r)
		{
			GetResource()->setRotation(r);
		}

		float ActionProcess::GetRotate()
		{
			return GetResource()->getRotation();
		}

		void ActionProcess::SetPosition(Vec2 p)
		{
			GetResource()->setPosition(p);
		}

		Vec2 ActionProcess::GetPosition()
		{
			return GetResource()->getPosition();		
		}

        void ActionProcess::SetOpacity(RKUtils::BYTE p)
		{
			GetResource()->setOpacity(p);
		}

		RKUtils::BYTE ActionProcess::GetOpacity()
		{
				return GetResource()->getOpacity();
		}

		void ActionProcess::SetColor(Color3B p)
		{

			return GetResource()->setColor(p);			
		}

		Color3B ActionProcess::GetColor()
		{
			return GetResource()->getColor();
		}

		void ActionProcess::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			//if (p_Visible)
			//{
				//GetResource()->visit(renderer, transform, flags);
			//}
		}

		void ActionProcess::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			//if (p_Visible)
			//{
				//GetResource()->draw(renderer, transform, flags);
			//}
		}

#pragma endregion normal_method

#pragma region action_process_method
		bool ActionProcess::SetActionProcess(STATUS_MENU stateFade, bool force_only)
		{
			//check
			if (force_only)
			{
				p_currentStatusMenu = stateFade;
				return false;
			}

			if (p_list_command.size() > 0)
			{
				RKString name_cmd = "";
				if (stateFade == STATUS_MENU::M_FADEIN)
				{
					name_cmd = "FADE_IN";
				}
				else if (stateFade == STATUS_MENU::M_FADEOUT)
				{
					name_cmd = "FADE_OUT";
				}

				bool val = SetActionCommandWithName(name_cmd);

				if (val)
				{
					p_currentStatusMenu = stateFade;
					return true;
				}

			}

			//this time we support 4 action s
			if (p_l_fd.Size() >= UI_STATE_FADE_ACTION::FADE_COUNT)
			{
				CCASSERT(false, "check size again"); // check size again
				return false;
			}
			p_currentStatusMenu = stateFade;
			Sequence * action = NULL;
			action = GetSequenceActionFromList(stateFade, p_l_fd, p_timeAction);
			if (!action)
			{
				p_currentStatusMenu = STATUS_MENU::M_IDLE;
				return false;
			}

			GetResource()->runAction(action);
			return true;
			
		}

		void ActionProcess::ForceFinishAction()
		{
			//
			FinishRunAction();

			//reset all action tag
			RemoveActionByTag(true);

			p_count_action_process = 0;
			p_action_process = 0;
		}


		void ActionProcess::ActionProcessFinish(UI_STATE_FADE_ACTION current_fd)
		{
			p_count_action_process ^= current_fd;
			if (p_count_action_process <= 0)
			{
				//
				FinishRunAction();

				//reset all action tag
				RemoveActionByTag();
			}
		}

		void ActionProcess::RemoveActionByTag(bool IsAll)
		{
			if (GetResource())
			{
					if (IsAll)
					{
						GetResource()->getActionManager()->removeAllActionsFromTarget(GetResource());
					}
					else
					{
						GetResource()->getActionManager()->removeActionByTag(p_action_process, GetResource());
					}
			}
			ResetActionFade();
			p_currentStatusMenu = STATUS_MENU::M_IDLE;
		}

		Vector<FiniteTimeAction*> ActionProcess::CreateListActionFromSingleAction(UI_STATE_FADE_ACTION action_state, int loop, bool reserve, float time)
		{
			Vector<FiniteTimeAction*> list_action;

			ActionInterval * ac1 = NULL;
			ActionInterval * ac_reserve = NULL;

			if (loop == 0)
				return list_action;

			if (time <= 0)
			{
				time = p_timeAction;
			}

			if (action_state == UI_STATE_FADE_ACTION::FADE_ZOOM)
			{
				ac1 = ScaleTo::create(time, GetFirstScale());
				if (reserve)
				{
					ac_reserve = ScaleTo::create(time, GetOriginScale());
				}
			}
			else if (action_state == UI_STATE_FADE_ACTION::FADE_TRANS)
			{
				ac1 = FadeTo::create(time, GetFirstAlpha());
				if (reserve)
				{
					ac_reserve = FadeTo::create(time, GetOriginAlpha());
				}
			}
			else if (action_state == UI_STATE_FADE_ACTION::FADE_ROTATE)
			{
				ac1 = RotateTo::create(time, abs(GetFirstAngle() - GetOriginAngle()));
				if (reserve)
				{
					ac_reserve = RotateTo::create(time, abs(GetOriginAngle() - GetFirstAngle()));
				}
			}
			else if (action_state == UI_STATE_FADE_ACTION::FADE_MOVE)
			{
				ac1 = MoveTo::create(time, GetFirstPos());
				if (reserve)
				{
					ac_reserve = MoveTo::create(time, GetOriginPos());
				}
			}

			p_action_process |= (action_state * loop);

			if (ac_reserve)
			{
				p_action_process |= ((action_state << 1) * loop);
			}



			if (loop < 0)
			{
				list_action.insert(list_action.size() , RepeatForever::create(
					Sequence::create(ac1, ac_reserve, nullptr)
					));

			}
			else if (loop > 0)
			{

				do
				{

					list_action.insert(list_action.size(), ac1);
					if (reserve)
					{
						list_action.insert(list_action.size(), ac_reserve);
					}
					loop--;
				} while (loop > 0);

			}

			auto action_complete = CallFunc::create(
				[=]()
			{
				RemoveActionByTag();
				///
			});

			list_action.insert(list_action.size(), action_complete);
			//list_action.insert(list_action.size(), nullptr);

			return list_action;
		}

		Sequence * ActionProcess::CreateActionFromList(Vector<ActionInterval*> list_spawn_action)
		{
			Sequence * action = NULL;

			int size_list = (int)list_spawn_action.size();

			if (size_list <= 0)
			{
				return NULL;
			}

			if (size_list == 1){
				action = Sequence::create(list_spawn_action.at(0), nullptr);
			}
			else if (size_list == 2){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), nullptr);
			}
			else if (size_list == 3){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), nullptr);
			}
			else if (size_list == 4){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), nullptr);
			}
			else if (size_list == 5){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), nullptr);
			}
			else if (size_list == 6){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), list_spawn_action.at(5), nullptr);
			}
			else if (size_list == 7){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), list_spawn_action.at(5), list_spawn_action.at(6), nullptr);
			}
			else if (size_list == 8){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), list_spawn_action.at(5), list_spawn_action.at(6), list_spawn_action.at(7), nullptr);
			}
			else if (size_list == 9){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), list_spawn_action.at(5), list_spawn_action.at(6), list_spawn_action.at(7), list_spawn_action.at(8), nullptr);
			}
			else if (size_list == 10){
				action = Sequence::create(list_spawn_action.at(0), list_spawn_action.at(1), list_spawn_action.at(2), list_spawn_action.at(3), list_spawn_action.at(4), list_spawn_action.at(5), list_spawn_action.at(6), list_spawn_action.at(7), list_spawn_action.at(8), list_spawn_action.at(9), nullptr);
			}

			return action;
		}

		void ActionProcess::SetResourceRunManyAction(std::vector<Vector<FiniteTimeAction*>> list_action_)
		{
			Action * action = NULL;

			if (list_action_.size() <= 1)
			{
				CCASSERT(false, "use this function if the list have 2 or more action");
				p_action_process = 0;
				p_count_action_process = 0;
				return;
			}

			//Sequence * action = NULL;
			if (list_action_.size() == 1)
			{
				action = Sequence::create(
					list_action_.at(0));
			}
			else if (list_action_.size() == 2)
			{
				auto list_action_1 = list_action_.at(0);
				auto list_action_2 = list_action_.at(1);

				if (list_action_1.size() != list_action_2.size())
				{
					CCASSERT(false, "size of all list must be equal");
					p_action_process = 0;
					p_count_action_process = 0;
					return;
				}

				Vector<ActionInterval*> list_spawn_action;

				for (int i = 0; i < list_action_1.size() - 1; i++)
				{
					list_spawn_action.pushBack(Spawn::create(
						list_action_1.at(i), list_action_2.at(i), nullptr));
				}

				action = Sequence::create(
					CreateActionFromList(list_spawn_action),
					list_action_1.at(list_action_1.size() - 1), list_action_2.at(list_action_2.size() - 1),
					nullptr);
			}
			else if (list_action_.size() == 3)
			{
				auto list_action_1 = list_action_.at(0);
				auto list_action_2 = list_action_.at(1);
				auto list_action_3 = list_action_.at(2);

				if (list_action_1.size() != list_action_2.size() || list_action_1.size() != list_action_3.size() || list_action_2.size() != list_action_3.size())
				{
					CCASSERT(false, "size of all list must be equal");
					p_action_process = 0;
					p_count_action_process = 0;
					return;
				}

				Vector<ActionInterval*> list_spawn_action;

				for (int i = 0; i < list_action_1.size() - 1; i++)
				{
					list_spawn_action.pushBack(Spawn::create(
						list_action_1.at(i), list_action_2.at(i), list_action_3.at(i), nullptr));
				}

				action = Sequence::create(
					CreateActionFromList(list_spawn_action),
					list_action_1.at(list_action_1.size() - 1), list_action_2.at(list_action_2.size() - 1), list_action_3.at(list_action_3.size() - 1),
					nullptr);
			}
			else if (list_action_.size() == 4)
			{
				auto list_action_1 = list_action_.at(0);
				auto list_action_2 = list_action_.at(1);
				auto list_action_3 = list_action_.at(2);
				auto list_action_4 = list_action_.at(2);

				if (list_action_1.size() != list_action_2.size() || list_action_1.size() != list_action_3.size() || list_action_2.size() != list_action_3.size() ||
					list_action_1.size() != list_action_4.size() || list_action_2.size() != list_action_4.size() || list_action_3.size() != list_action_4.size()
					)
				{
					CCASSERT(false, "size of all list must be equal");
					p_action_process = 0;
					p_count_action_process = 0;
					return;
				}

				Vector<ActionInterval*> list_spawn_action;

				for (int i = 0; i < list_action_1.size() - 1; i++)
				{
					list_spawn_action.pushBack(Spawn::create(
						list_action_1.at(i), list_action_2.at(i), list_action_3.at(i), list_action_4.at(i), nullptr));
				}

				action = Sequence::create(
					CreateActionFromList(list_spawn_action),
					list_action_1.at(list_action_1.size() - 1), list_action_2.at(list_action_2.size() - 1), list_action_3.at(list_action_3.size() - 1), list_action_4.at(list_action_4.size() - 1),
					nullptr);
			}
			else
			{
				//CCASSERT(false,"no thing in list action");
				return ;
			}

			p_count_action_process = p_action_process;
			action->setTag(p_action_process);
			action->retain();

			GetResource()->runAction(action);
		}

		void ActionProcess::SetResourceRunAction(UI_STATE_FADE_ACTION action_state, int loop, bool reserve, float time)
		{
			Action * action = NULL;

			Vector<FiniteTimeAction*> list_action = CreateListActionFromSingleAction(action_state, loop, reserve, time);

			action = Sequence::create(list_action);

			p_count_action_process = p_action_process;
			action->setTag(p_action_process);
			action->retain();

			GetResource()->runAction(action);		

		}

		bool ActionProcess::HaveActionNotFade()
		{
			return (p_currentStatusMenu == STATUS_MENU::M_IDLE && p_action_process != 0);
		}

		Sequence * ActionProcess::GetSequenceActionFromList(STATUS_MENU stateFade , RKList<UI_STATE_FADE_ACTION> l_fd, float time)
		{
			//
			RKList<FiniteTimeAction *> listActions;
			RKList<FiniteTimeAction *> listActionDones;
			int state_fd_insert[UI_STATE_FADE_ACTION::FADE_COUNT] = { 0 };
			//
			for (int i = 0; i < UI_STATE_FADE_ACTION::FADE_COUNT; i++)
			{
				if (i >= l_fd.Size())
				{
					break;
				}
				if (l_fd.GetAt(i) == UI_STATE_FADE_ACTION::FADE_MOVE)
				{
					if (state_fd_insert[0] == 1)
					{
						//already insert
						CC_ASSERT(false);
						continue;
					}
					//
					if (stateFade == STATUS_MENU::M_FADEIN)
						listActions.Append(MoveTo::create(p_timeAction, GetOriginPos()));
					else if (stateFade == STATUS_MENU::M_FADEOUT)
						listActions.Append(MoveTo::create(p_timeAction, GetFirstPos()));
					//
					state_fd_insert[0] = 1;
				}
				else if (l_fd.GetAt(i) == UI_STATE_FADE_ACTION::FADE_TRANS)
				{
					if (state_fd_insert[1] == 1)
					{
						//already insert
						CC_ASSERT(false);
						continue;
					}
					if (stateFade == STATUS_MENU::M_FADEIN)
						listActions.Append(FadeTo::create(p_timeAction, GetOriginAlpha()));
					else if (stateFade == STATUS_MENU::M_FADEOUT)
						listActions.Append(FadeTo::create(p_timeAction, GetFirstAlpha()));
					state_fd_insert[1] = 1;
				}
				else if (l_fd.GetAt(i) == UI_STATE_FADE_ACTION::FADE_ROTATE)
				{
					if (state_fd_insert[2] == 1)
					{
						//already insert
						CC_ASSERT(false);
						continue;
					}
					if (stateFade == STATUS_MENU::M_FADEIN)
						listActions.Append(RotateTo::create(p_timeAction, abs(GetOriginAngle() - GetFirstAngle())));
					else if (stateFade == STATUS_MENU::M_FADEOUT)
						listActions.Append(RotateTo::create(p_timeAction, abs(GetFirstAngle() - GetOriginAngle())));
					state_fd_insert[2] = 1;
				}
				else if (l_fd.GetAt(i) == UI_STATE_FADE_ACTION::FADE_ZOOM)
				{
					if (state_fd_insert[3] == 1)
					{
						//already insert
						CC_ASSERT(false);
						continue;
					}
					if (stateFade == STATUS_MENU::M_FADEIN)
						listActions.Append(ScaleTo::create(p_timeAction, GetOriginScale()));
					else if (stateFade == STATUS_MENU::M_FADEOUT)
						listActions.Append(ScaleTo::create(p_timeAction, GetFirstScale()));
					state_fd_insert[3] = 1;
				}
				AddActionFade(l_fd.GetAt(i));
				listActionDones.Append(CallFunc::create(CC_CALLBACK_0(ActionProcess::ActionProcessFinish, this, l_fd.GetAt(i))));
				
			}
			Sequence * action = NULL;
			if (listActions.Size() == 1)
			{
				action = Sequence::create(
						listActions[0],
						listActionDones[0],
					nullptr);
			}
			else if (listActions.Size() == 2)
			{
				action = Sequence::create(
					Spawn::create(	
						listActions[0],		listActions[1],		nullptr),
						listActionDones[0], listActionDones[1],
					nullptr);
			}
			else if (listActions.Size() == 3)
			{
				action = Sequence::create(
					Spawn::create(
						listActions[0],		listActions[1],		listActions[2],		nullptr),
						listActionDones[0],	listActionDones[1], listActionDones[2],
					nullptr);
			}
			else if (listActions.Size() == 4)
			{
				action = Sequence::create(
					Spawn::create(
						listActions[0],		listActions[1],		listActions[2],		listActions[3],		nullptr),
						listActionDones[0], listActionDones[1], listActionDones[2], listActionDones[3],
					nullptr);
			}
			else
			{
				//CCASSERT(false,"no thing in list action");
				return 0;
			}
			action->setTag(p_action_process);

			return action;
		}

#pragma endregion action_process_method

		bool ActionProcess::UpdateActionProcess(float dt)
		{
			if (!IsAllChildFinishAction() && p_action_process == 0)
				return false;

			if (p_currentStatusMenu == STATUS_MENU::M_IDLE && p_action_process == 0)
			{
				return true;
			}

			if (p_action_process == 0)
				return true;

			//auto actionTag = GetResource()->getActionByTag(p_action_process);
			//if (actionTag)
			//	actionTag->step(dt);
			
			return false;
		}

		void ActionProcess::ResetSizeAndPos(int cur_width, int cur_height)
		{
			/*
			//set position

			Vec2 cur_pos = GetPosition();
			cur_pos.x = (cur_pos.x / cur_width) * GetGameSize().width;
			cur_pos.y = (cur_pos.y / cur_height) * GetGameSize().height;
			SetPosition(cur_pos);
			//set origin pos
			p_originPos.x = (p_originPos.x / cur_width) * GetGameSize().width;
			p_originPos.y = (p_originPos.y / cur_height) * GetGameSize().height;

			//set first pos
			p_firstPos.x = (p_firstPos.x / cur_width) * GetGameSize().width;
			p_firstPos.y = (p_firstPos.y / cur_height) * GetGameSize().height;

			//set size
			if (p_typeUI == UI_MENU_TYPE::MENU_TEXT)
			{
				Vec2 cur_scale = GetScaleResource();
				cur_scale.x = (cur_scale.x / cur_width) * GetGameSize().width;
				cur_scale.y = (cur_scale.y / cur_height) * GetGameSize().height;
				SetScaleResource(cur_scale);
			}
			else
			{
				Vec2 cur_size = GetSizeResource();
				cur_size.x = (cur_size.x / cur_width) * GetGameSize().width;
				cur_size.y = (cur_size.y / cur_height) * GetGameSize().height;
				SetSizeResource(cur_size);
			}

			//set first scale
			p_first_scale = (p_first_scale / cur_width) * GetGameSize().width;

			//set origin scale
			p_target_scale_to = (p_target_scale_to / cur_width) * GetGameSize().width;
			*/
		}

		void ActionProcess::SetValueFromXML(xml::UIWidgetDec * xml_value)
		{
			//set size value
			SetSizeValueXML(xml_value);

			//set pos value
			SetPosValueXML(xml_value);

			SetFirstAlpha((RKUtils::BYTE)xml_value->FirstValue->GetDataInt("opacity"), xml_value->GeneralValue->GetDataInt("not_fade_opacity") == 0);
			SetOriginAlpha((RKUtils::BYTE)xml_value->OriginValue->GetDataInt("opacity"));
			SetFirstAngle(xml_value->FirstValue->GetDatafloat("rotate"), xml_value->GeneralValue->GetDataInt("not_fade_rotate") == 0);
			//
			Color4B color = xml_value->GeneralValue->GetDataColor("tint");
			SetOriginColor(Color3B(color.r,color.g,color.b), true);
			//
			InitSetFade(xml_value->actionFade, xml_value->TimeProcessFade);
			Visible(xml_value->visible == 1 ? true : false);
			SetIndexAppear(xml_value->index_appear);

			//
			SetCommandValueXML(xml_value);
		}

		void ActionProcess::SetSizeValueXML(xml::UIWidgetDec * xml_value)
		{
			Vec2 origin_ScaleRatio = xml_value->OriginValue->GetDataVector2("scale_ratio");
			Vec2 first_ScaleRatio = xml_value->FirstValue->GetDataVector2("scale_ratio");

			auto size_origin = Vec2(origin_ScaleRatio.x * GetGameSize().width, origin_ScaleRatio.y * GetGameSize().height);
			auto size_first = Vec2(first_ScaleRatio.x * GetGameSize().width, first_ScaleRatio.y * GetGameSize().height);

			bool IsStickScreen = (xml_value->GeneralValue->GetDataInt("stick_screen") == 1);

			if (IsStickScreen)
			{
				size_origin = Vec2(origin_ScaleRatio.x * GetGameSize().width, origin_ScaleRatio.y * GetGameSize().height);
				size_first = Vec2(first_ScaleRatio.x * GetGameSize().width, first_ScaleRatio.y * GetGameSize().height);
			}
			else
			{
				size_origin = Vec2(origin_ScaleRatio.x * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x), 
							       origin_ScaleRatio.y * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x));
				size_first = Vec2(first_ScaleRatio.x   * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x),
								  first_ScaleRatio.y   * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x));
			}

			if (GetResource()->getContentSize().width == 0 || GetResource()->getContentSize().height == 0)
			{
				if (IsStickScreen)
					GetResource()->setContentSize(cocos2d::Size(
					((origin_ScaleRatio.x + first_ScaleRatio.x) / 2) * xml_value->p_menu_widget->DesignSize.x,
					((origin_ScaleRatio.y + first_ScaleRatio.y) / 2)  * xml_value->p_menu_widget->DesignSize.y));
				else
                    GetResource()->setContentSize(cocos2d::Size(((origin_ScaleRatio.x + first_ScaleRatio.x) / 2),
													   ((origin_ScaleRatio.y + first_ScaleRatio.y) / 2)));

			}

			SetFirstSize(size_first, xml_value->GeneralValue->GetDataInt("not_fade_size") == 0);
		}

		void ActionProcess::SetPosValueXML(xml::UIWidgetDec * xml_value)
		{
			auto firstPosRatio = ConvertPos(xml_value->FirstValue->GetDataVector2("pos_ratio"));

			Vec2 posFirst = Vec2(0, 0);
			//if (p_parent_widget)
			//{
			//	//posFirst = 
			//}
			//else
			{
				posFirst = Vec2(GetGameSize().width * firstPosRatio.x, GetGameSize().height * firstPosRatio.y);
			}

			SetFirstPos(posFirst, xml_value->GeneralValue->GetDataInt("not_fade_pos") == 0);
		}

		void ActionProcess::SetCommandValueXML(xml::UIWidgetDec * xml_value)
		{
			if (xml_value->GetCommandSize() <= 0)
				return;

			for (int i = 0; i < xml_value->GetCommandSize(); i++)
			{
				ActionCommand * p_command = new ActionCommand();

				auto cmd_pair = xml_value->GetIterCommandAtIdx(i);

				if (cmd_pair.first != "" && p_command->ReadJsonString(cmd_pair.second.GetString()))
				{
					p_list_command.insert(std::pair<RKString,ActionCommand*>(cmd_pair.first, p_command));
				}
			}
		}

		bool ActionProcess::SetActionCommandWithName(RKString name, CallFunc * func_callback)
		{
			if ((int)p_list_command.size() < 0 || p_list_command.find(name) == p_list_command.end())
				return false;

			//get the action command with name 

			ActionCommand* action_command = p_list_command.at(name);

			int flag = 0;
			auto action_squence = action_command->GetSequenceActionData(p_action_process, this->GetResource(), 
				this->getUIWidgetType() == UI_TYPE::UI_LAYER ? this->GetParentLayer() : nullptr
				, flag ,this->getUIWidgetType() == UI_TYPE::UI_LAYER);

			p_count_action_process = p_action_process;
			if (flag >= 0)
			{
				auto action = Sequence::create(static_cast<Sequence*>(action_squence),
					func_callback != nullptr ? func_callback : CallFunc::create(CC_CALLBACK_0(ActionProcess::ForceFinishAction, this)), nullptr);

				action->setTag(p_action_process);
				action->retain();
				action->autorelease();
				GetResource()->runAction(action);
			}
			else
			{
				GetResource()->runAction(static_cast<RepeatForever*>(action_squence));
			}


			return true;
		}

		bool ActionProcess::SetHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail, xml::BasicDec  value, RKString name_value)
		{
			if (p_list_command.size() < 0 || p_list_command.find(name_command) == p_list_command.end())
			{
				return false;
			}

			ActionCommand* action_command = p_list_command.at(name_command);

			auto action_detail = action_command->GetActionDetail(idx_act, idx_squence, idx_detail);

			if (action_detail)
			{
				action_detail->SetValueHook(value, name_value);
				return true;
			}
			return false;
		}

		bool ActionProcess::ClearHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail)
		{
			if (p_list_command.size() < 0 || p_list_command.find(name_command) == p_list_command.end())
			{
				return false;
			}

			ActionCommand* action_command = p_list_command.at(name_command);

			auto action_detail = action_command->GetActionDetail(idx_act, idx_squence, idx_detail);

			if (action_detail)
			{
				action_detail->ClearHookCommand();
				return true;
			}
			return false;
		}

		void ActionProcess::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			p_Name = name;

			SetInitSize(xml_value);

			SetInitPos(xml_value);

			SetInitRotate(xml_value);

			GetResource()->setCascadeOpacityEnabled(true);
			GetResource()->setCascadeColorEnabled(true);
		}
		
		void ActionProcess::SetInitPos(xml::UIWidgetDec * xml_value)
		{
			auto pos_ratio = xml_value->OriginValue->GetDataVector2("pos_ratio");
			auto pos = Vec2(GetGameSize().width * pos_ratio.x, GetGameSize().height * pos_ratio.y);

			SetPosition(pos);
			p_origin_value->ReplaceDataVector2("pos", pos);
		}

		void ActionProcess::SetInitSize(xml::UIWidgetDec * xml_value)
		{
			auto scale_ratio = xml_value->OriginValue->GetDataVector2("scale_ratio");
			auto size = Vec2(GetGameSize().width * scale_ratio.x, GetGameSize().height * scale_ratio.y);

			bool IsStickScreen = (xml_value->GeneralValue->GetDataInt("stick_screen") == 1);

			if (!IsStickScreen)
			{
				size = Vec2(scale_ratio.x * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x), scale_ratio.y * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x));
			}
			else
			{
				int ttt = 0;
			}

			if (GetResource()->getContentSize().width == 0 || GetResource()->getContentSize().height == 0)
			{
                GetResource()->setContentSize(cocos2d::Size(size.x,size.y));
			}

			SetSize(size);
			p_origin_value->ReplaceDatafloat("scale", GetResource()->getScaleX());
		}

		void ActionProcess::SetInitRotate(xml::UIWidgetDec * xml_value)
		{
			auto rotate = xml_value->OriginValue->GetDatafloat("rotate");
			SetRotate(rotate);
			p_origin_value->ReplaceDatafloat("rotate", rotate);
		}

		void ActionProcess::FinishRunAction()
		{
			if (p_parent_layer)
				p_parent_layer->HaveChildFinishAction();
		}

		void ActionProcess::SetParentLayer(LayerEntity * parent)
		{ 
			if (parent)
			{
				p_parent_layer = parent;
				//p_parent_layer->retain();
			}
		}

	}
}