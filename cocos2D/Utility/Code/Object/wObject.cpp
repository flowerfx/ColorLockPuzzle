#include "wObject.h"
#include "../ObjectTexture.h"
#include "../DisplayView.h"

namespace Utility
{
	namespace wObject
	{
		wBasicObject::wBasicObject()
		{
			p_Resource = nullptr;

			p_TypeObject = TYPE_OBJECT::OBJ_NONE;

			p_list_command.clear();

			p_valueContain = nullptr;

			p_name_cotain = "";

			p_OnActionLock = false;

			p_parent_object = nullptr;
			p_listChildObject.clear();

			p_valueContain = new xml::BasicDec();

			p_ActionListQueue.clear();

			design_size = Vec2(0, 0);

		}

		wBasicObject::~wBasicObject()
		{
			FreeObject();
		}

		void wBasicObject::FreeObject()
		{
			//
			if (p_Resource)
			{
				p_Resource->getActionManager()->removeAllActionsFromTarget(p_Resource);
				p_Resource->removeAllChildrenWithCleanup(true);
				//CC_SAFE_DELETE(p_Resource);
				p_Resource->release();
			}
			//
			for (auto it = p_list_command.begin(); it != p_list_command.end(); it++)
			{
				delete p_list_command.at(it->first);
			}

			p_list_command.clear();
			CC_SAFE_DELETE(p_valueContain);
			//dont delete parent
			p_parent_object = nullptr;

			for (int i = 0; i < p_listChildObject.size(); i++)
			{
				p_listChildObject.at(i)->autorelease();
			}
			p_listChildObject.clear();

			for (int i = 0; i < p_ActionListQueue.size(); i++)
			{
				p_ActionListQueue.at(i)->autorelease();
			}
			p_ActionListQueue.clear();

		}

		void wBasicObject::Flip(bool UseY, bool b)
		{
			CCASSERT(GetResource(), "object sprite invalid");

			switch (p_TypeObject)
			{
			case Utility::wObject::OBJ_NONE:
				break;
			case Utility::wObject::OBJ_ARMATURE:
			case Utility::wObject::OBJ_LABEL:
			{
				if (!UseY)
				{
					float cur_scale = (p_Resource)->getScaleX();
					(p_Resource)->setScaleX(b ? -abs(cur_scale) : abs(cur_scale));
				}
				else
				{
					float cur_scale = (p_Resource)->getScaleY();
					(p_Resource)->setScaleY(b ? -abs(cur_scale) : abs(cur_scale));
				}
				break;
			}
			case Utility::wObject::OBJ_ANIMATION:
			case Utility::wObject::OBJ_SPRITE:
			case Utility::wObject::OBJ_PROCESS_TIMER:
			{
				if (!UseY)
				{
					static_cast<Sprite*>(p_Resource)->setFlippedX(b);
				}
				else
				{
					static_cast<Sprite*>(p_Resource)->setFlippedX(b);
				}
				break;
			}
			case Utility::wObject::OBJ_COUNT:
				break;
			default:
				break;
			}
		}

		void wBasicObject::UpdateActionProcess(float dt)
		{
			if (p_ActionListQueue.size() > 0 && !p_OnActionLock) //check the queue
			{
				if (p_ActionListQueue.at(0)->p_action_tag > 0)
				{
					auto action = p_Resource->getActionByTag(p_ActionListQueue.at(0)->p_action_tag);
					if (action)
					{
						action->step(dt);
					}
				}
			}

			if (p_OnActionLock)
			{
				p_ActionListQueue.at(0)->autorelease();
				p_ActionListQueue.erase(0);

				if (p_ActionListQueue.size() > 0)
				{
					GetResource()->runAction(p_ActionListQueue.at(0)->p_action);
				}
				p_OnActionLock = false;
			}

			for (int i = 0; i < p_listChildObject.size(); i++)
			{
				p_listChildObject.at(i)->UpdateActionProcess(dt);
			}

			return;
		}

		void wBasicObject::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			GetResource()->draw(renderer, transform, flags);
		}

		void wBasicObject::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			GetResource()->visit(renderer, transform, flags);
		}

		void wBasicObject::Update(float dt)
		{
			UpdateActionProcess(dt);
		}

#pragma region init_method
		bool wBasicObject::OnInitWideObject(RKString name, bool UseAntiAlias)
		{
			auto object_dec = XMLMgr->GetWideObjectByName(name);

			bool res = OnInitWideObjectXML(object_dec, UseAntiAlias);

			return res;
		}

		bool wBasicObject::OnInitWideObjectXML(xml::Wide_ObjectDec * object_dec, bool UseAntiAlias)
		{
			if (!object_dec)
			{
				CCASSERT(false, "no object loaded");
				return false;
			}
			p_name_cotain = object_dec->name_object;
			//
			design_size = object_dec->design_size;

			switch (object_dec->type_object)
			{
			case 0:
			case 4:
			{
				Sprite * p = TextureMgr->GetSpriteByName(object_dec->name_source);
				if (object_dec->type_object == 0)
				{
					LoadObjectFromSprite(p, UseAntiAlias);
				}
				else
				{
					auto p_ObjectTimer = ProgressTimer::create(p);
					p_TypeObject = TYPE_OBJECT::OBJ_SPRITE;
					p_Resource = p_ObjectTimer;
				}

				break;
			}
			case 1:
			{
				LoadAnimateFromEditor(object_dec->animature_dec);
				break;
			}
			case 2:
			{
				LoadAnimateFromXML(object_dec->animation_dec);
				break;
			}
			case 3:
			{
				LoadFontDecFromXML(object_dec->text_dec);
				break;
			}
			default:
				PASSERT2(false, "no object loaded");
				return false;
				break;
			}

			//
			LoadValueBasicObject(object_dec);
			//
			p_Resource->retain();
			//
			return true;
		}

		void wBasicObject::LoadObjectFromSprite(Sprite * p, bool UseAntiAlias)
		{
#ifdef USE_SPRITE_FRAME
			p_Resource = p;
#else
			p_Resource = p->createWithTexture(p->getTexture(), p->getTextureRect());
#endif
#if USE_ANTIALIAS
			if (UseAntiAlias)
			{
				p_Resource->getTexture()->generateMipmap();
				Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
				p_Resource->getTexture()->setTexParameters(&texParams);
			}
#endif
			p_TypeObject = TYPE_OBJECT::OBJ_SPRITE;
			//p_Resource->retain();

		}

		void wBasicObject::LoadAnimateFromXML(xml::AnimateDec * animation_dec)
		{
			if (animation_dec)
			{
				//CCASSERT(p_Object != NULL, "object not init");

				Vector<SpriteFrame*> animFrames;

				for (size_t i = 0; i < animation_dec->GetNumberSprite(); i++)
				{
					Sprite * temp = TextureMgr->GetSpriteByName(animation_dec->GetSpriteFrameNameAtIdx(i));
					auto frame = SpriteFrame::createWithTexture(temp->getTexture(), temp->getTextureRect());
					animFrames.pushBack(frame);
				}
				Animation * p_Animate = CCAnimation::createWithSpriteFrames(animFrames, animation_dec->time_delay, animation_dec->loop);
				auto t = CCAnimate::create(p_Animate);
				t->setTag(animation_dec->tag_action);

				Sprite * p = TextureMgr->GetSpriteByName(animation_dec->source_sprite);

				Sprite * p_Object = nullptr;
#ifdef USE_SPRITE_FRAME
				p_Object = p;
#else
				p_Object = p->createWithTexture(p->getTexture(), p->getTextureRect());
#endif
				p_Object->runAction(t);

				p_TypeObject = TYPE_OBJECT::OBJ_ANIMATION;

				p_Resource = p_Object;
			}
		}

		void wBasicObject::LoadFontDecFromXML(xml::FontDec * font_dec)
		{
			if (font_dec)
			{
				auto label_temp = TextureMgr->GetFontByIdx(font_dec->idx);
				TTFConfig config(label_temp->getTTFConfig().fontFilePath.c_str(), font_dec->d_size * GetGameSize().width, font_dec->IsUnicode ? GlyphCollection::ASCII : GlyphCollection::DYNAMIC);
				auto p_textLabel = label_temp->createWithTTF(config, "0");

				p_TypeObject = TYPE_OBJECT::OBJ_LABEL;
				p_Resource = p_textLabel;

				SetColor(Color4B(font_dec->d_color[0], font_dec->d_color[1], font_dec->d_color[2], font_dec->d_color[3]));
			}
		}

		void wBasicObject::LoadAnimateFromEditor(xml::AnimatureDec * animature_dec)
		{
			if (animature_dec)
			{
				bool IsUseArk = false;
#ifdef USE_ARK_ARCHIVE
				IsUseArk = true;
				g_IsUseArk = IsUseArk;
#endif
				//async loading export file
				ArmatureDataManager::getInstance()->addArmatureFileInfo(
					File::ResolveNamePath(animature_dec->pathTexture.GetString()).GetString(),
					File::ResolveNamePath(animature_dec->pathPlist.GetString()).GetString(),
					File::ResolveNamePath(animature_dec->PathJson.GetString()).GetString()
					);

				//Creat armature by name
				auto p_Armature = CCArmature::create(animature_dec->name.GetString());

				//retain
				//p_Armature->retain();

				//play animation by animation index
				p_Armature->getAnimation()->playByIndex(animature_dec->idxPlay);

				p_Armature->getAnimation()->setSpeedScale(animature_dec->SpeedScale);

#ifdef USE_ARK_ARCHIVE
				IsUseArk = false;
				g_IsUseArk = IsUseArk;
#endif
				p_Resource = p_Armature;

				p_TypeObject = TYPE_OBJECT::OBJ_ARMATURE;
			}
		}

		void wBasicObject::LoadValueBasicObject(xml::Wide_ObjectDec * object_dec)
		{
			if (object_dec)
			{
				/*
					set the origin value of the object
					*/

				bool IsVisible = (object_dec->CommonValue->GetDataInt("visible") == 1);
				SetVisible(IsVisible);

				bool IsStickScreen = (object_dec->CommonValue->GetDataInt("stick_screen") != 0);

				//set size of object
				if (IsStickScreen)
				{
					Vec2 size_object = Vec2(
						object_dec->OriginValue->GetDataVector2("scale_ratio").x * GetGameSize().width,
						object_dec->OriginValue->GetDataVector2("scale_ratio").y * GetGameSize().height);
					SetSize(size_object);
					p_valueContain->InsertDataVector2("size_origin", size_object);
				}
				else
				{
					Vec2 size_object = Vec2(
						object_dec->OriginValue->GetDataVector2("scale_ratio").x * GetGameSize().width,
						object_dec->OriginValue->GetDataVector2("scale_ratio").y * GetGameSize().width);
					SetSize(size_object);
					p_valueContain->InsertDataVector2("size_origin", size_object);
				}

				SetPosition(Vec2(object_dec->OriginValue->GetDataVector2("pos_ratio").x * GetGameSize().width,
					object_dec->OriginValue->GetDataVector2("pos_ratio").y * GetGameSize().height));

				p_valueContain->InsertDataVector2("pos_origin", Vec2(object_dec->OriginValue->GetDataVector2("pos_ratio").x * GetGameSize().width,
					object_dec->OriginValue->GetDataVector2("pos_ratio").y * GetGameSize().height));

				//set opacity of object
				SetOpacity(object_dec->OriginValue->GetDataInt("opacity"));
				p_valueContain->InsertDataInt("opacity_origin", object_dec->OriginValue->GetDataInt("opacity"));

				//set color of object
				SetColor(object_dec->OriginValue->GetDataColor("tint"));
				p_valueContain->InsertDataColor("tint_origin", object_dec->OriginValue->GetDataColor("tint"));

				//set angle object
				SetAngle(object_dec->OriginValue->GetDatafloat("rotate"));
				p_valueContain->InsertDatafloat("rotate_origin", object_dec->OriginValue->GetDatafloat("rotate"));

				//set command
				SetCommandValueXML(object_dec);

				//set child

				for (int i = 0; i < object_dec->list_child_object.size(); i++)
				{
					auto child_object = OnLoadChildObject(this, object_dec->list_child_object[i]);

					child_object->p_Resource->setZOrder(i);
					child_object->p_Resource->setName(child_object->GetName().GetString());
					this->ResizeAndPosChildWidget(child_object);
					this->p_Resource->addChild(child_object->GetResource());
					this->p_listChildObject.pushBack(child_object);
				}

			//	GetResource()->setCascadeOpacityEnabled(true);
			//	GetResource()->setCascadeColorEnabled(true);

				LoadFirstValue(object_dec);
			}
		}

		void wBasicObject::LoadFirstValue(xml::Wide_ObjectDec * object_dec)
		{
			if (object_dec)
			{
				/*
				set the origin value of the object
				*/

				bool IsStickScreen = (object_dec->CommonValue->GetDataInt("stick_screen") != 0);

				//set size of object
				if (IsStickScreen)
				{
					Vec2 size_object = Vec2(
						object_dec->FirstValue->GetDataVector2("scale_ratio").x * GetGameSize().width,
						object_dec->FirstValue->GetDataVector2("scale_ratio").y * GetGameSize().height);
					SetSize(size_object);
				}
				else
				{
					Vec2 size_object = Vec2(
						object_dec->FirstValue->GetDataVector2("scale_ratio").x * GetGameSize().width,
						object_dec->FirstValue->GetDataVector2("scale_ratio").y * GetGameSize().width);
					SetSize(size_object);
				}

				SetPosition(Vec2(object_dec->FirstValue->GetDataVector2("pos_ratio").x * GetGameSize().width,
					object_dec->FirstValue->GetDataVector2("pos_ratio").y * GetGameSize().height)
					);

				//set opacity of object
				SetOpacity(object_dec->FirstValue->GetDataInt("opacity"));

				//set color of object
				SetColor(object_dec->FirstValue->GetDataColor("tint"));

				//set angle object
				SetAngle(object_dec->FirstValue->GetDatafloat("rotate"));
			}
		}

		void wBasicObject::SetCommandValueXML(xml::Wide_ObjectDec * xml_value)
		{
			if (xml_value->GetCommandSize() <= 0)
				return;

			for (int i = 0; i < xml_value->GetCommandSize(); i++)
			{
				ActionCommand * p_command = new ActionCommand();

				auto cmd_pair = xml_value->GetIterCommandAtIdx(i);

				if (cmd_pair.first != "" && p_command->ReadJsonString(cmd_pair.second.GetString()))
				{
					p_list_command.insert(std::pair<RKString, ActionCommand*>(cmd_pair.first, p_command));
				}
			}
		}

		bool wBasicObject::SetActionCommandWithName(RKString name, CallFunc * call_back)
		{
			if (p_list_command.size() < 0 || p_list_command.find(name) == p_list_command.end())
			{
				return false;
			}

			//check the action already added

			if (p_ActionListQueue.size() > 0)
			{
				for (int i = 0; i < p_ActionListQueue.size(); i++)
				{
					if (p_ActionListQueue.at(i)->action_name == name)
					{
						return false;
					}
				}
			}

			//get the action command with name 

			unsigned int p_action_process = 0;

			ActionCommand* action_command = p_list_command.at(name);

			int flag = 0;
			auto action_squence = action_command->GetSequenceActionData(p_action_process, this->GetResource(), this->p_parent_object ? this->p_parent_object->GetResource() : nullptr, flag);

			auto action = Sequence::create(action_squence, 
				call_back ? call_back : CallFunc::create(CC_CALLBACK_0(wBasicObject::EventActionFinish, this)),
				nullptr);

			action->setTag(p_action_process);
			action->retain();

			p_ActionListQueue.pushBack(new ActionQueue(p_action_process, action, name));

			if (p_ActionListQueue.size() == 1)
			{
				GetResource()->runAction(action);
				p_OnActionLock = false;
			}



			return true;
		}

		bool wBasicObject::ClearHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail)
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

		void wBasicObject::StopCurrentActionWithName(RKString name)
		{
			int idx_name = -1;
			if (p_ActionListQueue.size() > 0)
			{
				for (int i = 0; i < p_ActionListQueue.size(); i++)
				{
					if (p_ActionListQueue.at(i)->action_name == name)
					{
						idx_name = i;
						break;
					}
				}
			}

			if (idx_name >= 0)
			{
				GetResource()->stopAllActionsByTag(p_ActionListQueue.at(idx_name)->p_action_tag);
				p_ActionListQueue.erase(idx_name);
			}
		}

		bool wBasicObject::SetActionCustomWithName(RKString name, cocos2d::Sequence* action_custom, CallFunc * call_back )
		{
			//check the action already added

			if (p_ActionListQueue.size() > 0)
			{
				for (int i = 0; i < p_ActionListQueue.size(); i++)
				{
					if (p_ActionListQueue.at(i)->action_name == name)
					{
						return false;
					}
				}
			}

			auto action = Sequence::create(action_custom,
				call_back ? call_back : CallFunc::create(CC_CALLBACK_0(wBasicObject::EventActionFinish, this)),
				nullptr);

			action->setTag(action_custom->getTag());
			action->retain();

			p_ActionListQueue.pushBack(new ActionQueue(action_custom->getTag(), action, name));

			if (p_ActionListQueue.size() == 1)
			{
				GetResource()->runAction(action);
				p_OnActionLock = false;
			}

			return true;
		}

		void wBasicObject::ResizeAndPosChildWidget(wBasicObject * child)
		{
			//set new size for widget child
			float scale_parent_widget_x = this->GetSize().x / this->GetResource()->getContentSize().width;
			float scale_parent_widget_y = this->GetSize().y / this->GetResource()->getContentSize().height;

			Vec2 design_size = child->design_size;

			Vec2 first_pos = Vec2((child->GetPosition().x / GetGameSize().width) * design_size.x, (child->GetPosition().y / GetGameSize().height) * design_size.y);
			//child->SetPosition(first_pos);

			Vec2 first_size = Vec2(this->GetSize().x * design_size.x / GetGameSize().width, this->GetSize().y * design_size.x / GetGameSize().width);


			Vec2 pos_ratio = Vec2(first_pos.x / first_size.x, first_pos.y / first_size.y);
			//if (scale_parent_widget_x != 1.0f)
			{
				child->GetResource()->setScaleX((1 / scale_parent_widget_x) * child->GetResource()->getScaleX());
				child->GetResource()->setScaleY((1 / scale_parent_widget_x) * child->GetResource()->getScaleY());

				child->SetPosition(Vec2((1 / scale_parent_widget_x) * this->GetSize().x *pos_ratio.x,
					(1 / scale_parent_widget_y) * this->GetSize().y *pos_ratio.y));
			}
		}

		wBasicObject * wBasicObject::OnLoadChildObject(wBasicObject * parent, xml::Wide_ObjectDec * objec)
		{
			if (objec)
			{
				wBasicObject * _child = new wBasicObject();

				_child->OnInitWideObjectXML(objec);

				_child->p_parent_object = parent;

				return _child;
			}
			return nullptr;
		}
#pragma endregion init_method


#pragma region normal_method
		cocos2d::Node*  wBasicObject::GetResource()
		{
			return p_Resource;
		}

		void wBasicObject::SetSize(Vec2 Size)
		{
			CCASSERT(GetResource(), "object sprite invaid");
			Vec2 p_Size(1, 1);
			//
			p_Size.x = Size.x / GetResource()->getContentSize().width;
			p_Size.y = Size.y / GetResource()->getContentSize().height;
			if (p_TypeObject == TYPE_OBJECT::OBJ_LABEL)
			{
				GetResource()->setScaleX(p_Size.x);
				GetResource()->setScaleY(p_Size.x);
			}
			else
			{
				GetResource()->setScaleX(p_Size.x);
				GetResource()->setScaleY(p_Size.y);
			}

		}

		Vec2 wBasicObject::GetSize()
		{
			CCASSERT(GetResource(), "object sprite invaid");
			Vec2 p_Size(1, 1);
			//
			p_Size.x = GetResource()->getContentSize().width * GetResource()->getScaleX();
			p_Size.y = GetResource()->getContentSize().height * GetResource()->getScaleY();
			//
			return p_Size;
		}

		void wBasicObject::SetScale(Vec2 scale)
		{
			Vec2 p_scale(1, 1);
			int mipmap = TextureMgr->GetLevelMipMap();

			p_scale.x = GetMulMul(scale.x, mipmap);
			p_scale.y = GetMulMul(scale.y, mipmap);


			CCASSERT(GetResource(), "object sprite invaid");
			GetResource()->setScaleX(p_scale.x);
			GetResource()->setScaleY(p_scale.y);
		}

		Vec2 wBasicObject::GetScale()
		{
			Vec2 p_scale(1, 1);
			int mipmap = TextureMgr->GetLevelMipMap();

			CCASSERT(GetResource(), "object sprite invaid");
			p_scale.x = GetMulDiv(GetResource()->getScaleX(), mipmap);
			p_scale.y = GetMulDiv(GetResource()->getScaleY(), mipmap);

			return p_scale;
		}

        void wBasicObject::SetOpacity(RKUtils::BYTE opacity)
		{
			CCASSERT(GetResource(), "object sprite invaid");
			GetResource()->setOpacity(opacity);
		}

		RKUtils::BYTE  wBasicObject::GetOpacity()
		{
			CCASSERT(GetResource(), "object sprite invaid");
			return GetResource()->getOpacity();
		}

		void wBasicObject::SetAngle(float angle)
		{
			CCASSERT(GetResource(), "object sprite invaid");
			GetResource()->setRotation(angle);
		}

		float wBasicObject::GetAngle()
		{
			CCASSERT(GetResource(), "object sprite invaid");
			return GetResource()->getRotation();
		}

		void wBasicObject::SetColor(Color4B color)
		{
			CCASSERT(GetResource(), "object sprite invaid");
			if (p_TypeObject == TYPE_OBJECT::OBJ_LABEL)
			{
				static_cast<Label*>(GetResource())->setTextColor(color);
			}
			else
			{
				GetResource()->setColor(Color3B(color.r, color.g, color.b));
			}
		}

		Color4B wBasicObject::GetColor()
		{
			CCASSERT(GetResource(), "object sprite invaid");
			if (p_TypeObject == TYPE_OBJECT::OBJ_LABEL)
			{
				return static_cast<Label*>(GetResource())->getTextColor();
			}
			else
			{
				Color3B color = GetResource()->getColor();
				return Color4B(color.r, color.g, color.b, GetOpacity());
			}
		}

		void wBasicObject::SetPosition(Vec2 pos)
		{
			CCASSERT(GetResource(), "object sprite invaid");
			GetResource()->setPosition(pos);
		}

		Vec2 wBasicObject::GetPosition()
		{
			CCASSERT(GetResource(), "object sprite invaid");
			return GetResource()->getPosition();
		}

        cocos2d::Rect wBasicObject::GetBound()
		{
			Vec2 pos = GetPosition();
			Vec2 size = GetSize();
            cocos2d::Rect rec(
				pos.x - size.x / 2,
				pos.y - size.y / 2,
				size.x,
				size.y
				);
			return rec;
		}

		void wBasicObject::SetBlendFunc(BlendFunc blend)
		{
			if (p_TypeObject == TYPE_OBJECT::OBJ_LABEL)
			{
				static_cast<Label*>(p_Resource)->setBlendFunc(blend);
				return;
			}
			else if (p_TypeObject == TYPE_OBJECT::OBJ_SPRITE)
			{
				static_cast<Sprite*>(p_Resource)->setBlendFunc(blend);
			}
		}

		bool wBasicObject::IsVisible() 
		{ 
			return p_Resource->isVisible(); 
		}

		void wBasicObject::SetVisible(bool val) 
		{
			p_Resource->setVisible(val);
		}

#pragma endregion normal_method

		bool wBasicObject::OnFinishAction()
		{
			//abtrast class dont implement this function.....
			return true;
		}

		void wBasicObject::EventActionFinish()
		{
			p_Resource->getActionManager()->removeAllActionsFromTarget(p_Resource);
			p_OnActionLock = true;
			OnFinishAction();
			//abtrast class dont implement this function...
		}

		wBasicObject * wBasicObject::GetObjectByName(RKString name_path)
		{
			if (name_path != "")
			{
				auto name_list = name_path.Split(".");
				int value_i = name_path.FindFirst(".", 0, 1);
				if (value_i == 0)
				{
					for (int i = 0; i < p_listChildObject.size(); i++)
					{

						RKString sub_name = "";
						for (int j = 0; j < name_list.Size(); j++)
						{
							sub_name += name_list.GetAt(j);
							if (j != name_list.Size() - 1)
							{
								sub_name += ".";
							}
						}
						if (name_list.Size() == 1)
						{
							sub_name = name_list.GetAt(0);
						}

						auto value = p_listChildObject.at(i)->GetObjectByName(sub_name);
						if (value)
						{
							return value;
						}
					}
				}
				else
				{
					if ((GetName() == name_list.GetAt(0)) && name_list.Size() > 1)
					{
						for (int i = 0; i < p_listChildObject.size(); i++)
						{

							RKString sub_name = "";
							for (int j = 1; j < name_list.Size(); j++)
							{
								sub_name += name_list.GetAt(j);
								if (j != name_list.Size() - 1)
								{
									sub_name += ".";
								}
							}
							auto value = p_listChildObject.at(i)->GetObjectByName(sub_name);
							if (value)
							{
								return value;
							}
						}
					}
					else if ((GetName() == name_list.GetAt(0)) && name_list.Size() <= 1)
					{
						return this;
					}
				}

				return nullptr;
			}
			return nullptr;
		}
	
		bool wBasicObject::SetHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail, xml::BasicDec value, RKString name_value)
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
	}
}