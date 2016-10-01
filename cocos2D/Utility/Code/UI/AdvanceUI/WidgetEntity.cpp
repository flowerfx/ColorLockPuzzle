#include "WidgetEntity.h"
#include "LayerEntity.h"
#include "MenuWidget.h"
namespace Utility
{
	namespace UI_Widget
	{
		WidgetEntity::WidgetEntity()
		{
			p_Resource = NULL;

			p_typeUI = UI_TYPE::UI_NONE;

			p_listChildWidget.clear();

			p_isScale9Enable = false;

		}

		WidgetEntity::~WidgetEntity()
		{
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				p_listChildWidget.at(i)->autorelease();
			}
			p_listChildWidget.clear();


			if (p_Resource)
			{
				p_Resource->autorelease();
			}
			p_Resource = nullptr;
			//CC_SAFE_RELEASE_NULL(p_Resource);
		}

		void WidgetEntity::CloneChild(WidgetEntity * p_clone)
		{
			//PASSERT2(p_clone->p_listChildWidget.size() == p_listChildWidget.size(), "number child of 2 object must be equal");
			//remove all child if exist 
			p_clone->GetResource()->removeAllChildren();
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				auto child_widget = p_listChildWidget.at(i)->Clone();
				p_clone->p_listChildWidget.push_back(child_widget);
				p_clone->GetResource()->setName(child_widget->GetName().GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
				p_clone->GetResource()->setZOrder(p_listChildWidget.size());
				p_clone->SetParentLayer(this->GetParentLayer());
				p_clone->GetResource()->addChild(child_widget->GetResource());
			}
		}

		bool WidgetEntity::Update(float dt)
		{
			p_Resource->update(dt);
			return true;
		}

		void WidgetEntity::SetPosValueXML(xml::UIWidgetDec * xml_value)
		{
			auto originPosRatio = (xml_value->OriginValue->GetDataVector2("pos_ratio"));
			auto firstPosRatio = (xml_value->FirstValue->GetDataVector2("pos_ratio"));

			auto posOrigin = Vec2(GetGameSize().width * originPosRatio.x, GetGameSize().height * originPosRatio.y);
			auto posFirst = Vec2(GetGameSize().width * firstPosRatio.x, GetGameSize().width * firstPosRatio.y);

			SetFirstPos(Vec2(GetGameSize().width * firstPosRatio.x, GetGameSize().height * firstPosRatio.y), xml_value->GeneralValue->GetDataInt("not_fade_pos") == 0);
		}


		void WidgetEntity::InsertChildWidget(WidgetEntity* child_widget)
		{
			p_listChildWidget.push_back(child_widget);
			child_widget->GetResource()->setName(child_widget->GetName().GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
			child_widget->GetResource()->setZOrder(p_listChildWidget.size());
			child_widget->SetParentLayer(this->GetParentLayer());
			GetResource()->addChild(child_widget->GetResource());
		}

		void WidgetEntity::FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value)
		{
			for (int i = 0; i < xml_value->p_list_child_widget.size(); i++)
			{
				auto _widget_child = this->GetParentLayer()->InitWidgetEntity(xml_value->p_list_child_widget.at(i), this->GetParentLayer(), this);

				ResizeAndPosChildWidget(_widget_child);

				_widget_child->SetMenuParent(this->GetMenuParent());
				_widget_child->SetParentWidget(this);
				InsertChildWidget(_widget_child);
			}
		}

		void WidgetEntity::InitParamParent(RKString name, xml::UIWidgetDec * xml_value, LayerEntity * layer_parent, WidgetEntity * widget_parent)
		{
			SetParentLayer(layer_parent);
			SetMenuParent(layer_parent->GetMenuParent());
			SetParentWidget(widget_parent);
			InitParam(name, xml_value);
		}

		bool WidgetEntity::ContainChildIdxAppear(unsigned int idx_appear)
		{
			if (GetIndexAppear() == idx_appear)
			{
				return true;
			}
			bool value = false;
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				value |= p_listChildWidget.at(i)->ContainChildIdxAppear(idx_appear);
			}
			return value;
		}

		WidgetEntity * WidgetEntity::GetWidgetChildAtIdx(int idx)
		{
			if (idx < p_listChildWidget.size() && p_listChildWidget.size() > 0)
			{
				return p_listChildWidget.at(idx);
			}
			return nullptr;
		}

		WidgetEntity * WidgetEntity::GetWidgetChildByName(RKString name_path)
		{
			auto name_list = name_path.Split(".");
			int value_i = name_path.FindFirst(".", 0, 1);
			if (value_i == 0)
			{
				for (int i = 0; i < p_listChildWidget.size(); i++)
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

					auto value = p_listChildWidget.at(i)->GetWidgetChildByName(sub_name);
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
					for (int i = 0; i < p_listChildWidget.size(); i++)
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
						auto value = p_listChildWidget.at(i)->GetWidgetChildByName(sub_name);
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

		int WidgetEntity::SetProcessActionChildIdxAppear(STATUS_MENU fade_state, unsigned int idx_appear)
		{
			if (GetIndexAppear() == idx_appear)
			{
				if (SetActionProcess(fade_state))
				{
					return 1;
				}
			}
			int value = 0;
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				value += p_listChildWidget.at(i)->SetProcessActionChildIdxAppear(fade_state, idx_appear);
			}

			return value;
		}

		void WidgetEntity::SetInteractive(bool value)
		{
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				p_listChildWidget.at(i)->SetInteractive(value);
			}
			p_IsInteractive = value;
		}

		void WidgetEntity::RemoveAllChild()
		{
			for (int i = 0; i < p_listChildWidget.size(); i++)
			{
				p_listChildWidget.at(i)->autorelease();
			}

			p_listChildWidget.clear();
		}

		void WidgetEntity::AddChildWidget(WidgetEntity * object)
		{
			if (object)
			{
				this->GetResource()->addChild(object->GetResource());
				p_listChildWidget.push_back(object);
				object->GetResource()->setName(object->GetName().GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
				object->GetResource()->setZOrder(p_listChildWidget.size());
				object->SetParentLayer(this->GetParentLayer());
			}
		}

		void WidgetEntity::ResizeAndPosChildWidget(WidgetEntity * child)
		{
			//set new size for widget child
			float scale_parent_widget_x = this->GetSize().x / this->GetResource()->getContentSize().width;
			float scale_parent_widget_y = this->GetSize().y / this->GetResource()->getContentSize().height;

			Vec2 design_size = child->GetMenuParent()->GetDesignSize();

			Vec2 first_pos = Vec2((child->GetPosition().x / GetGameSize().width) * design_size.x, (child->GetPosition().y / GetGameSize().height) * design_size.y);
			//child->SetPosition(first_pos);

			Vec2 first_size = Vec2(this->GetSize().x * design_size.x / GetGameSize().width, this->GetSize().y * design_size.y / GetGameSize().height);


			Vec2 pos_ratio = Vec2(first_pos.x / first_size.x, first_pos.y / first_size.y);
			//if (scale_parent_widget_x != 1.0f)
			{
				//child->GetResource()->setScaleX((1 / scale_parent_widget_x) * child->GetResource()->getScaleX());
				//child->GetResource()->setScaleY((1 / scale_parent_widget_x) * child->GetResource()->getScaleY());

				child->SetPosition(Vec2((1 / scale_parent_widget_x) * this->GetSize().x *pos_ratio.x,
					(1 / scale_parent_widget_y) * this->GetSize().y *pos_ratio.y));
			}
		}

		void WidgetEntity::SetSize(Vec2 s)
		{
			if (p_isScale9Enable)
			{
				p_Resource->setSize(cocos2d::Size(s.x, s.y));
			}
			else
			{
				ActionProcess::SetSize(s);
			}
		}

		Vec2 WidgetEntity::GetSize()
		{
			if (p_isScale9Enable)
			{
				return Vec2(p_Resource->getSize().width, p_Resource->getSize().height);
			}
			else
			{
				return ActionProcess::GetSize();
			}
		}
	}
}