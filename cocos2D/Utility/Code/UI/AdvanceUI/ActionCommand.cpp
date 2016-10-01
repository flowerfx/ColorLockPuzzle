#include "ActionCommand.h"
#include "LayerEntity.h"
namespace Utility
{
	namespace UI_Widget
	{
		void ActionCommand::ActionDetail::ClearHookCommand(RKString name_value)
		{
			if (!_hook_value)
			{
				delete _hook_value;
				_hook_value = nullptr;
			}
		}

		void ActionCommand::ActionDetail::SetValueHook(xml::BasicDec value, RKString name_value)
		{
			if (!_hook_value)
			{
				_hook_value = new xml::BasicDec();
			}

			if (name_value == "TIME")
			{
				SetTimeAction(value.GetDatafloat(name_value));
				return;
			}

			if (value.HaveDataInt(name_value))
			{
				_hook_value->InsertDataInt(name_value, value.GetDataInt(name_value));
			}
			if (value.HaveDatafloat(name_value))
			{
				_hook_value->InsertDatafloat(name_value, value.GetDatafloat(name_value));
			}
			if (value.HaveDataChar(name_value))
			{
				_hook_value->InsertDataChar(name_value, value.GetDataChar(name_value));
			}
			if (value.HaveDataVector2(name_value))
			{
				_hook_value->InsertDataVector2(name_value, value.GetDataVector2(name_value));
			}
			if (value.HaveDataColor(name_value))
			{
				_hook_value->InsertDataColor(name_value, value.GetDataColor(name_value));
			}

		}

		ActionCommand::ActionDetail * ActionCommand::GetActionDetail(RKString name_act, int idx_squence, int idx_detail)
		{
			ActionCommand::ActionDetail * return_value = nullptr;

			if (p_list_actions.size() <= 0 || p_list_actions.find(name_act) == p_list_actions.end())
				return nullptr;

			auto list_act_detail_squence = p_list_actions.at(name_act);

			if (idx_squence < 0 || idx_squence >= list_act_detail_squence->p_list_action_.size())
				return nullptr;

			auto act_detail_squence = list_act_detail_squence->p_list_action_.at(idx_squence);

			if (idx_detail < 0 || idx_detail >= act_detail_squence.size())
				return nullptr;

			return act_detail_squence.at(idx_detail);
		}

		void ActionCommand::ActionDetail::ParseActionDetail(const Json::Value value, RKString name_value)
		{
			bool AddName = true;
			if (name_value == "SIZE")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 size = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, size);
			}
			else if (name_value == "OPACITY")
			{
				unsigned int detail_value = value[name_value.GetString()].asUInt();
				value_action->InsertDataInt(name_value, detail_value);
			}
			else if (name_value == "POSITION")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 pos = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, pos);
			}
			else if (name_value == "ROTATE")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				value_action->InsertDatafloat(name_value, detail_value);
			}
			else if (name_value == "TIME")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				time_action = detail_value;
				AddName = false;
			}
			else if (name_value == "DELAY")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				value_action->InsertDatafloat(name_value, detail_value);
			}
			else if (name_value == "DELAY")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				value_action->InsertDatafloat(name_value, detail_value);
			}
			else if (name_value == "DESIGN")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 design_size = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, design_size);
				AddName = false;
			}
			else if (name_value == "STICK")
			{
				int IsStick = value[name_value.GetString()].asInt();
				value_action->InsertDataInt(name_value, IsStick);
				AddName = false;
			}
			else if (name_value == "STYLE")
			{
				int Style = value[name_value.GetString()].asInt();
				value_action->InsertDataInt(name_value, Style);
				AddName = false;
			}
			else if (name_value == "SET_POSITION")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 pos = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, pos);
			}
			else if (name_value == "SET_ROTATE")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				value_action->InsertDatafloat(name_value, detail_value);
			}
			else if (name_value == "SET_SIZE")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 size = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, size);
			}
			else if (name_value == "SET_OPACITY")
			{
				unsigned int detail_value = value[name_value.GetString()].asUInt();
				value_action->InsertDataInt(name_value, detail_value);
			}
			else if (name_value == "BEZIER" || name_value == "BEZIER_P1" || name_value == "BEZIER_P2")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 pos = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, pos);
			}
			else if (name_value == "JUMP_TO")
			{
				RKString detail_value = value[name_value.GetString()].asString();
				Vec2 pos = XMLMgr->ConvertToVector2(detail_value);
				value_action->InsertDataVector2(name_value, pos);
			}
			else if (name_value == "JUMP_NUM")
			{
				unsigned int detail_value = value[name_value.GetString()].asUInt();
				value_action->InsertDataInt(name_value, detail_value);
				AddName = false;
			}
			else if (name_value == "JUMP_HEIGHT")
			{
				float detail_value = value[name_value.GetString()].asFloat();
				value_action->InsertDatafloat(name_value, detail_value);
				AddName = false;
			}

			if (AddName)
			{
				name_action = name_value;
				if (name_action == "BEZIER_P2")
				{
					name_action = "BEZIER";
				}
			}
		}

		FiniteTimeAction * ActionCommand::ActionDetail::GetAction(unsigned int & action_tag, cocos2d::Node * it_res, cocos2d::Node * parent, bool IsLayer)
		{
			FiniteTimeAction * action = nullptr;

			if (name_action == "SIZE")
			{
				Vec2 size = value_action->GetDataVector2(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					size = _hook_value->GetDataVector2(name_action);
				}

				Vec2 design_size = value_action->GetDataVector2("DESIGN");
				bool IsStick = value_action->GetDataInt("STICK") != 0;
				size = Vec2(size.x * (GetGameSize().width / design_size.x),
					size.y * (IsStick ? (GetGameSize().height / design_size.y) : (GetGameSize().width / design_size.x)));
				if (parent != nullptr)
				{
					size.x = size.x * (1.f / parent->getScaleX());
					size.y = size.y * (1.f / parent->getScaleY());
				}
				
				Vec2 CurrentSize = (it_res)->getContentSize();
				Vec2 scale = Vec2((size.x / CurrentSize.x), (size.y / CurrentSize.y));
				action = ScaleTo::create(time_action, scale.x, scale.y);
				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_ZOOM;
			}
			else if (name_action == "SET_SIZE")
			{
				Vec2 size = value_action->GetDataVector2(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					size = _hook_value->GetDataVector2(name_action);
				}
				Vec2 design_size = value_action->GetDataVector2("DESIGN");
				bool IsStick = value_action->GetDataInt("STICK") != 0;
				size = Vec2(size.x * (GetGameSize().width / design_size.x),
					size.y * (IsStick ? (GetGameSize().height / design_size.y) : (GetGameSize().width / design_size.x)));
				if (parent != nullptr)
				{
					size.x = size.x * (1.f / parent->getScaleX());
					size.y = size.y * (1.f / parent->getScaleY());
				}

				Vec2 CurrentSize = (it_res)->getContentSize();
				Vec2 scale = Vec2((size.x / CurrentSize.x), (size.y / CurrentSize.y));

				action = CallFunc::create([it_res, scale](){
					it_res->setScale(scale.x, scale.y);
				});
			}
			else if (name_action == "OPACITY")
			{
				int value = value_action->GetDataInt(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataInt(name_action))
				{
					value = _hook_value->GetDataInt(name_action);
				}
				action = FadeTo::create(time_action, (unsigned char)value);
				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_TRANS;
			}
			else if (name_action == "SET_OPACITY")
			{
				int value = value_action->GetDataInt(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataInt(name_action))
				{
					value = _hook_value->GetDataInt(name_action);
				}
				action = CallFunc::create([it_res, value](){
					it_res->setOpacity(value);
				});
			}
			else if (name_action == "ROTATE")
			{
				float value = value_action->GetDatafloat(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDatafloat(name_action))
				{
					value = _hook_value->GetDatafloat(name_action);
				}
				int style = value_action->GetDataInt("STYLE") != 0;
				if (style == 0)
				{
					action = RotateTo::create(time_action, value);
				}
				else
				{
					action = RotateBy::create(time_action, value);
				}
				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_ROTATE;
			}
			else if (name_action == "SET_ROTATE")
			{
				float value = value_action->GetDatafloat(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDatafloat(name_action))
				{
					value = _hook_value->GetDatafloat(name_action);
				}
				action = CallFunc::create([it_res, value](){
					it_res->setRotation(value);
				});
			}
			else if (name_action == "SET_POSITION")
			{
				Vec2 value = value_action->GetDataVector2(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					value = _hook_value->GetDataVector2(name_action);
				}
				Vec2 design_size = value_action->GetDataVector2("DESIGN");

				Vec2 pos = Vec2(value.x * (GetGameSize().width / design_size.x),
					value.y * (GetGameSize().height / design_size.y));

				if (IsLayer)
				{
					pos = ConvertPos1(pos);
					pos.x -= (it_res->getContentSize().width / 2);
					pos.y -= (it_res->getContentSize().height / 2);
				}

				if (parent != nullptr)
				{
					pos.x = pos.x * (1.f / parent->getScaleX());
					pos.y = pos.y * (1.f / parent->getScaleY());
				}

				action = CallFunc::create([it_res,pos](){
					it_res->setPosition(pos);
				});

			}
			else if (name_action == "POSITION")
			{
				Vec2 value = value_action->GetDataVector2(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					value = _hook_value->GetDataVector2(name_action);
				}
				int style = value_action->GetDataInt("STYLE") ;

				Vec2 design_size = value_action->GetDataVector2("DESIGN");
				if (style == 0)
				{
					Vec2 pos = Vec2(value.x * (GetGameSize().width / design_size.x),
						value.y * (GetGameSize().height / design_size.y ));

					if (IsLayer)
					{
						pos = ConvertPos1(pos);
						pos.x -= (it_res->getContentSize().width / 2);
						pos.y -= (it_res->getContentSize().height / 2);
					}

					if (parent != nullptr)
					{
						pos.x = pos.x * (1.f / parent->getScaleX());
						pos.y = pos.y * (1.f / parent->getScaleY());
					}


					action = MoveTo::create(time_action, pos);
				}
				else if (style == 1)
				{
					Vec2 delta_pos = Vec2(value.x - (it_res)->getPosition().x, value.y - (it_res)->getPosition().y);
					Vec2 pos = Vec2(delta_pos.x * (GetGameSize().width / design_size.x),
						delta_pos.y * (GetGameSize().height / design_size.y));


					if (IsLayer)
					{
						pos.x -= (it_res->getContentSize().width / 2);
						pos.y -= (it_res->getContentSize().height / 2);
					}

					if (parent != nullptr)
					{
						pos.x = pos.x * (1.f / parent->getScaleX());
						pos.y = pos.y * (1.f / parent->getScaleY());
					}

					action = MoveBy::create(time_action, pos);
				}
				else if (style == 2)
				{
					Vec2 delta_pos = value;
					Vec2 pos = Vec2(delta_pos.x * (GetGameSize().width / design_size.x),
						delta_pos.y * (GetGameSize().height / design_size.y));


					if (IsLayer)
					{
						pos.x -= (it_res->getContentSize().width / 2);
						pos.y -= (it_res->getContentSize().height / 2);
					}

					if (parent != nullptr)
					{
						pos.x = pos.x * (1.f / parent->getScaleX());
						pos.y = pos.y * (1.f / parent->getScaleY());
					}

					action = MoveBy::create(time_action, pos);
				}
				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_MOVE;
			}
			else if (name_action == "DELAY")
			{
				float value = value_action->GetDatafloat(name_action);
				//hook
				if (_hook_value && _hook_value->HaveDatafloat(name_action))
				{
					value = _hook_value->GetDatafloat(name_action);
				}
				action = DelayTime::create(value);
				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_DELAY;
			}
			else if (name_action == "BEZIER" )
			{
				Vec2 value_bezier = value_action->GetDataVector2(name_action);
				Vec2 value_bezier_p1 = value_action->GetDataVector2("BEZIER_P1");
				Vec2 value_bezier_p2 = value_action->GetDataVector2("BEZIER_P2");
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					value_bezier = _hook_value->GetDataVector2(name_action);
				}
				if (_hook_value && _hook_value->HaveDataVector2("BEZIER_P1"))
				{
					value_bezier_p1 = _hook_value->GetDataVector2("BEZIER_P1");
				}
				if (_hook_value && _hook_value->HaveDataVector2("BEZIER_P2"))
				{
					value_bezier_p2 = _hook_value->GetDataVector2("BEZIER_P2");
				}

				int style = value_action->GetDataInt("STYLE");

				Vec2 design_size = value_action->GetDataVector2("DESIGN");

				_ccBezierConfig be_zonfig;

				if (style == 0)
				{
					Vec2 pos_to = Vec2(value_bezier.x * (GetGameSize().width / design_size.x),
										value_bezier.y * (GetGameSize().height / design_size.y));
					Vec2 pos_p1 = Vec2(value_bezier_p1.x * (GetGameSize().width / design_size.x),
										value_bezier_p1.y * (GetGameSize().height / design_size.y));
					Vec2 pos_p2 = Vec2(value_bezier_p2.x * (GetGameSize().width / design_size.x),
										value_bezier_p2.y * (GetGameSize().height / design_size.y));

					if (IsLayer)
					{
						pos_to = ConvertPos1(pos_to);
						pos_to.x -= (it_res->getContentSize().width / 2);
						pos_to.y -= (it_res->getContentSize().height / 2);

						pos_p1 = ConvertPos1(pos_to);
						pos_p1.x -= (it_res->getContentSize().width / 2);
						pos_p1.y -= (it_res->getContentSize().height / 2);

						pos_p2 = ConvertPos1(pos_to);
						pos_p2.x -= (it_res->getContentSize().width / 2);
						pos_p2.y -= (it_res->getContentSize().height / 2);
					}

					be_zonfig.endPosition = pos_to;
					be_zonfig.controlPoint_1 = pos_p1;
					be_zonfig.controlPoint_2 = pos_p2;

					action = BezierTo::create(time_action, be_zonfig);
				}


				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_BENZIER;
			}
			else if (name_action == "JUMP_TO")
			{
				Vec2 value = value_action->GetDataVector2(name_action);
				unsigned int number_jump = value_action->GetDataInt("JUMP_NUM");
				float height_jump = value_action->GetDatafloat("JUMP_HEIGHT");
				//hook
				if (_hook_value && _hook_value->HaveDataVector2(name_action))
				{
					value = _hook_value->GetDataVector2(name_action);
				}
				if (_hook_value && _hook_value->HaveDataVector2("JUMP_NUM"))
				{
					number_jump = _hook_value->GetDataInt("JUMP_NUM");
				}
				if (_hook_value && _hook_value->HaveDataVector2("JUMP_HEIGHT"))
				{
					height_jump = _hook_value->GetDataInt("JUMP_HEIGHT");
				}

				int style = value_action->GetDataInt("STYLE");
				Vec2 design_size = value_action->GetDataVector2("DESIGN");


				if (style == 0)
				{
					Vec2 pos_to = Vec2(value.x * (GetGameSize().width / design_size.x),
						value.y * (GetGameSize().height / design_size.y));

					height_jump = height_jump * (GetGameSize().width / design_size.x);
					if (IsLayer)
					{
						pos_to = ConvertPos1(pos_to);
						pos_to.x -= (it_res->getContentSize().width / 2);
						pos_to.y -= (it_res->getContentSize().height / 2);

					}

					action = JumpTo::create(time_action, pos_to, height_jump, number_jump);
				}
				else if (style == 1)
				{
					Vec2 delta_pos = Vec2(value.x - (it_res)->getPosition().x, value.y - (it_res)->getPosition().y);
					Vec2 pos = Vec2(delta_pos.x * (GetGameSize().width / design_size.x),
						delta_pos.y * (GetGameSize().height / design_size.y));

					height_jump = height_jump * (GetGameSize().width / design_size.x);

					if (IsLayer)
					{
						pos = ConvertPos1(pos);
						pos.x -= (it_res->getContentSize().width / 2);
						pos.y -= (it_res->getContentSize().height / 2);

					}

					action = JumpBy::create(time_action, pos, height_jump, number_jump);
				}

				action_tag |= (int)UI_STATE_FADE_ACTION::FADE_BENZIER;
			}
			return action;
		}

		ActionCommand::ActionDetail * ActionCommand::ActionDetail::Clone()
		{
			ActionCommand::ActionDetail * p_clone = new ActionCommand::ActionDetail();
#ifdef OS_IOS
            memcpy(p_clone, this, sizeof(p_clone));
            memcpy(p_clone->value_action, this->value_action, sizeof(p_clone->value_action));
#else
            memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
			memcpy_s(p_clone->value_action, sizeof(p_clone->value_action), this->value_action, sizeof(this->value_action));
#endif
			return p_clone;
		}


		ActionCommand::ActionCommand()
		{
			p_list_actions.clear();
		}

		ActionCommand::~ActionCommand()
		{
			for (auto it = p_list_actions.begin(); it != p_list_actions.end(); it++)
			{
				delete p_list_actions.at(it->first);
			}
			p_list_actions.clear();
		}

		bool  ActionCommand::ReadJsonString(const char * jstr)
		{
			Json::Reader jreader;

			bool parsingSuccessful = jreader.parse(jstr, JRootValue);
			if (!parsingSuccessful)
			{
				// report to the user the failure and their locations in the document.
				PINFO((std::string("Failed to parse configuration : ") + jreader.getFormatedErrorMessages()).c_str());
				return false;
			}

			for (int i = 0; i < JRootValue.size(); i++)
			{
				RKString name_act = "act" + std::to_string(i);
				auto action_value = JRootValue[name_act.GetString()];
				auto list_action_each_act = ReadJsonAction(action_value);

				p_list_actions.insert(std::pair<RKString, ActionRepeat*>(name_act, list_action_each_act));
			}
			return true;
		}

		ActionCommand::ActionDetail* ActionCommand::ParseDetailJsonAction(const Json::Value value)
		{
			//PASSERT2(value.size() == 2, "the value must 2 vlue detail_action and time_action");

			ActionDetail* return_value = new ActionDetail();
			//get the first value
			for (auto it = value.begin(); it != value.end(); ++it)
			{
				RKString name_value = it.memberName();
				return_value->ParseActionDetail(value, name_value);			
			}

			return return_value;
		}

		ActionCommand::ActionRepeat * ActionCommand::ReadJsonAction(const Json::Value value)
		{
			ActionRepeat* list_action_each_act = new ActionRepeat();
			for (auto it = value.begin(); it != value.end(); ++it)
			{
				std::vector<ActionDetail*> list_detail_act;
				RKString name_value = it.memberName();
				bool dont_push = false;
				if (name_value.Contains("single"))
				{
					ActionDetail* act_detail = ParseDetailJsonAction(value[name_value.GetString()]);
					//act_detail->retain();
					//act_detail->autorelease();
					list_detail_act.push_back(act_detail);
				}
				else if (name_value.Contains("squence"))
				{
					auto value_squence = value[name_value.GetString()];
					for (auto it_squence = value_squence.begin(); it_squence != value_squence.end(); it_squence++)
					{
						int idx_value = it_squence.index();
						ActionDetail* act_detail = ParseDetailJsonAction(value_squence[idx_value]);
						//act_detail->retain();
						//act_detail->autorelease();
						list_detail_act.push_back(act_detail);
					}
				}
				else if (name_value == "repeat")
				{
					list_action_each_act->number_repeat = value[name_value.GetString()].asInt();
					dont_push = true;
				}

				if (!dont_push)
				{
					list_action_each_act->p_list_action_.push_back(list_detail_act);
				}
			}

			return list_action_each_act;
		}

		Sequence * ActionCommand::GetSquenceActionDetail(RKString name, int idx, unsigned int & action_tag, cocos2d::Node * it_res, cocos2d::Node * parent, bool IsLayer) const
		{
			auto list_action_detail = (p_list_actions.at(name)->p_list_action_).at(idx);
			Vector<FiniteTimeAction*> array_Action;
			for (int i = 0; i < list_action_detail.size(); i++)
			{
				auto action = list_action_detail.at(i)->GetAction(action_tag, it_res, parent, IsLayer);
				if (action)
				{
					array_Action.pushBack(action);
				}
			}
			//array_Action.pushBack(nullptr);
			auto squence = Sequence::create(array_Action);

			return squence;
		}

		ActionInterval * ActionCommand::GetSequenceActionData(unsigned int & action_tag, cocos2d::Node * it_res, cocos2d::Node * parent, int & flag, bool IsLayer) const
		{
			//
			Sequence * action = NULL;
			Vector<FiniteTimeAction*> array_final_Action;
			for (auto it = p_list_actions.begin(); it != p_list_actions.end(); it++)
			{
				Vector<FiniteTimeAction*> array_Action;
				for (int i = 0; i < it->second->p_list_action_.size(); i++)
				{
					auto squence_action = GetSquenceActionDetail(it->first, i, action_tag, it_res, parent, IsLayer);
					array_Action.pushBack(squence_action);
				}
				//array_Action.pushBack(nullptr);
				auto spawn_action = Spawn::create(array_Action);

				Sequence* action_continue = nullptr;
				if (it->second->number_repeat > 0)
				{
					Vector<FiniteTimeAction*> array_Action_repeat;
					int number_repeat = it->second->number_repeat;
					do
					{
						array_Action_repeat.pushBack(spawn_action);
						number_repeat--;
					} while (number_repeat > 0);
					flag += it->second->number_repeat;
					action_continue = Sequence::create(array_Action_repeat);
					action_tag *= it->second->number_repeat;
					array_final_Action.pushBack(action_continue);
				}
				else // repeate forever
				{
					RepeatForever * repeatAction = RepeatForever::create(spawn_action);
					flag = -1;
					return repeatAction;
				}
			}
			//array_final_Action.pushBack(nullptr);

			action = Sequence::create(array_final_Action);

			return action;
		}

		ActionCommand * ActionCommand::Clone()
		{
			ActionCommand * p_clone = new ActionCommand();
#ifdef OS_IOS
            memcpy(p_clone, this, sizeof(p_clone));
#else
            memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
#endif

			for (auto it = p_list_actions.begin(); it != p_list_actions.end(); it++)
			{
				auto list_1 = it->second;
				for (int i = 0; i < list_1->p_list_action_.size(); i++)
				{
					auto list_2 = list_1->p_list_action_[i];

					auto list_clone_value = p_clone->p_list_actions.at(it->first)->p_list_action_[i];
					for (int j = 0; j < list_2.size(); j++)
					{
						list_clone_value.erase(list_clone_value.begin() + j);
						list_clone_value.insert(list_clone_value.begin() + j , list_2.at(j)->Clone());
					}

					p_clone->p_list_actions.at(it->first)->p_list_action_[i] = list_2;
				}
				p_clone->p_list_actions.at(it->first) = list_1;
			}

			return p_clone;
					
		}

	}
}