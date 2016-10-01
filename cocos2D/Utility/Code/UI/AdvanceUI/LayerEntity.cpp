#include "LayerEntity.h"

#include "wButton.h"
#include "wLayout.h"
#include "wText.h"
#include "wPanel.h"
#include "wListView.h"
#include "wScrollView.h"
#include "wPageView.h"
#include "wTextField.h"
#include "wCheckBox.h"
#include "../../SoundManager.h"
#include "MenuWidget.h"
namespace Utility
{
	namespace UI_Widget
	{
		LayerEntity::LayerEntity()
		{
			p_listWidgetChild.clear();
			p_listLayerChild.clear();

			setTouchEnabled(true);

			p_current_idx_appear = -1;
			p_number_child_have_action = 0;

			p_typeUI = UI_TYPE::UI_LAYER;
		
			RegisterUIWidget();
		}

		LayerEntity::~LayerEntity()
		{
			p_listWidgetChild.clear();
			p_listLayerChild.clear();

			p_current_idx_appear = -1;
			p_number_child_have_action = 0;

		}


		void LayerEntity::AddWidgetWithName(Widget * element, RKString name)
		{
			if (element)
			{
				element->setName(name.GetString());
				element->setZOrder(p_listWidgetChild.size());
				this->addChild(element);
			}

		}

		void LayerEntity::AddLayerEntity(LayerEntity * layer)
		{
			if (layer)
			{
				p_listLayerChild.push_back(layer);

				layer->setName(layer->GetName().GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
				layer->setZOrder(p_listLayerChild.size());
				layer->SetParentLayer(this);
				this->addChild(layer);
			}
		}

		void LayerEntity::AddWidgetEntity(WidgetEntity * layer)
		{
			if (layer)
			{
				//layer->SetParentLayer(this);
				p_listWidgetChild.push_back(layer);
				AddWidgetWithName((Widget*)layer->GetResource(), layer->GetName());
			}
		}

		void LayerEntity::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			if (p_Visible)
			{
				Layer::draw(renderer, transform, flags);
			}
		}

		void LayerEntity::visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			if (p_Visible)
			{
				Layer::visit(renderer, transform, flags);
			}
		}

		bool LayerEntity::Update(float dt)
		{
			return true;
		}

		WidgetEntity * LayerEntity::InitWidgetEntity(xml::UILayerWidgetDec * widget, LayerEntity * parent_layer, WidgetEntity * parent_widget)
		{
			if (widget)
			{
				xml::UIWidgetDec* _widget = static_cast<xml::UIWidgetDec*>(widget);

				if (widget->typeValue == xml::TYPE_UI_WIDGET::WIDGET_LAYER)
				{
					PASSERT2(false, "ERROR : Dont init layer in this function");
					return nullptr;
				}
				//Widget::enableDpadNavigation(true);
				auto wEntity = p_factory_ui_widget.CreateNewProduct(widget->typeValue);
				if (wEntity)
				{
					wEntity->InitParamParent(widget->NameWidget, _widget, parent_layer, parent_widget);
					wEntity->SetValueFromXML(_widget);
					wEntity->FindAndInsertChildWidget(widget);
				}


				switch (widget->typeValue)
				{
					case xml::TYPE_UI_WIDGET::WIDGET_BUTTON:
					{
						((Button*)(wEntity->GetResource()))->addTouchEventListener(CC_CALLBACK_2(LayerEntity::OnTouchEvent, this, wEntity));
						break;
					}
					case xml::TYPE_UI_WIDGET::WIDGET_LIST_VIEW:
					{
						((ListView*)(wEntity->GetResource()))->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(LayerEntity::OnSelectListItem, this, wEntity));
						break;
					}
					case xml::TYPE_UI_WIDGET::WIDGET_TEXT_FIELD:
					{
						((TextField*)(wEntity->GetResource()))->addEventListener(CC_CALLBACK_2(LayerEntity::OnTextFieldEvent, this, wEntity));
						break;
					}
					case xml::TYPE_UI_WIDGET::WIDGET_LAYOUT:
					{
						//if (static_cast<LayoutWidget*>(wEntity)->IsFocusEnable())
						//{
							//((Widget*)(wEntity->GetResource()))->addTouchEventListener(CC_CALLBACK_2(LayerEntity::OnLayoutFocusChangeEvent, this, wEntity));
						//}
						break;
					}
				}

				return wEntity;
			}

			return nullptr;
		}
	
		void LayerEntity::OnTextFieldEvent(cocos2d::Ref* sender, TextField::EventType type, WidgetEntity* _widget)
		{
			TextField* textField = dynamic_cast<TextField*>(sender);
			switch (type)
			{
				
				case TextField::EventType::ATTACH_WITH_IME:
				{
					textField->setPlaceHolder("");
					break;
				}
				case TextField::EventType::DETACH_WITH_IME:
				{
					textField->setPlaceHolder(static_cast<TextFieldWidget*>(_widget)->GetDefaultPlaceHolder().GetString());
					if (_widget->IsInteractive())
					{
						if (menu_point_to)
						{
							func_call_widget(*menu_point_to, _widget->GetName(), _widget->getUIWidgetType());
						}
					}
					break;
				}
				case TextField::EventType::INSERT_TEXT:
				{
					break;
				}
				case TextField::EventType::DELETE_BACKWARD:
				{
					textField->setPlaceHolder(static_cast<TextFieldWidget*>(_widget)->GetDefaultPlaceHolder().GetString());
					break;
				}
			}
		}
		
		void LayerEntity::OnLayoutFocusChangeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, WidgetEntity* _widget)
		{
				if (_widget->IsInteractive())
				{
					if (menu_point_to)
					{
						func_call_widget(*menu_point_to, _widget->GetName(), _widget->getUIWidgetType());
					}
				}
		}

		void LayerEntity::OnSelectListItem(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type, WidgetEntity* _widget)
		{
			switch (type)
			{
				case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
				{
					break;
				}
				case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
				{
					ListViewWidget* listView = static_cast<ListViewWidget*>(_widget);
					CC_UNUSED_PARAM(listView);
					int current_index_item = static_cast<ListView*>(listView->GetResource())->getCurSelectedIndex();
					listView->SetCurrentIdxSelected(current_index_item);
					break;
				}
				default:
					break;
			}
		}

		void LayerEntity::OnTouchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, WidgetEntity* _widget)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				switch (_widget->getUIWidgetType())
				{
				case UI_TYPE::UI_BUTTON:
				{
					auto btn = ((ButtonWidget*)_widget);
					if (btn->IsInteractive())
					{
						GetSound->PlaySoundEffect(btn->GetIdSound()->id_click, btn->GetIdSound()->delayTime, btn->GetIdSound()->loop);
					}
					break;
				}
				default:
					break;
				}
				break;

			case Widget::TouchEventType::MOVED:

				break;

			case Widget::TouchEventType::ENDED:
			{
				//auto btn = ((ButtonWidget*)_widget);
				if (_widget->IsInteractive())
				{
					if (menu_point_to)
					{
						func_call_widget(*menu_point_to, _widget->GetName(), _widget->getUIWidgetType());
					}
				}
			}
			break;

			case Widget::TouchEventType::CANCELED:

				break;

			default:
				break;
			}
		}

		void LayerEntity::SetCallBack( std::function<void(MenuWidget&, RKString, UI_TYPE)> func)
		{
			func_call_widget = func;
		}

		//the postion of layer not place at the center , it place at left bottom
		void LayerEntity::SetPosValueXML(xml::UIWidgetDec * xml_value)
		{
			auto originPosRatio = p_parent_layer ? xml_value->OriginValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->OriginValue->GetDataVector2("pos_ratio"));
			auto firstPosRatio = p_parent_layer ? xml_value->FirstValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->FirstValue->GetDataVector2("pos_ratio"));

			auto posOrigin = Vec2(GetGameSize().width * originPosRatio.x, GetGameSize().height * originPosRatio.y);
			auto posFirst = Vec2(GetGameSize().width * firstPosRatio.x, GetGameSize().height * firstPosRatio.y);

			PASSERT2(GetResource()->getContentSize().width != 0 || GetResource()->getContentSize().height != 0, "the content size of resource must bigger than 0");

			posFirst.x -= (GetResource()->getContentSize().width / 2);
			posFirst.y -= (GetResource()->getContentSize().height / 2);

			SetFirstPos(posFirst, xml_value->GeneralValue->GetDataInt("not_fade_pos") == 0);
		}

		void LayerEntity::SetInitPos(xml::UIWidgetDec * xml_value)
		{
			auto pos_ratio = p_parent_layer ? xml_value->OriginValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->OriginValue->GetDataVector2("pos_ratio"));
			auto pos = Vec2(GetGameSize().width * pos_ratio.x, GetGameSize().height * pos_ratio.y);

			pos.x -= (GetResource()->getContentSize().width / 2);
			pos.y -= (GetResource()->getContentSize().height / 2);

			SetPosition(pos);
			p_origin_value->ReplaceDataVector2("pos", pos);
		}

		void LayerEntity::SetActionProcessForAllChild(STATUS_MENU fade_state, unsigned int current_idx)
		{
			for (int i = 0; i < p_listWidgetChild.size(); i++)
			{
				auto child = p_listWidgetChild.at(i);
				if (child->GetIndexAppear() != current_idx)
				{
					if (child->ContainChildIdxAppear(current_idx))
					{
						p_number_child_have_action += child->SetProcessActionChildIdxAppear(fade_state, current_idx);
					}

					continue;
				}
				bool res = child->SetActionProcess(fade_state);
				if (res)
				{
					p_number_child_have_action++;
				}
			}

			for (int j = 0; j < p_listLayerChild.size(); j++)
			{
				auto child = p_listLayerChild.at(j);

				if (child->GetIndexAppear() != current_idx)
				{

					child->SetActionProcessForAllChild(fade_state, current_idx);
					continue;
				}
				bool res = child->SetActionProcess(fade_state);
				if (res)
				{
					p_number_child_have_action++;
				}
			}
		}

		bool LayerEntity::IsAllChildFinishAction()
		{
			return (p_number_child_have_action <= 0);
		}

		WidgetEntity * LayerEntity::GetWidgetChildAtIdx(int idx)
		{
			if (idx < p_listWidgetChild.size() && p_listWidgetChild.size() > 0)
			{
				return p_listWidgetChild.at(idx);
			}
			return nullptr;
		}

		WidgetEntity * LayerEntity::GetWidgetChildByName(RKString name_path)
		{
			auto name_list = name_path.Split(".");

			auto layer_contain = GetLayerChildByName(name_list.GetAt(0));
			if (layer_contain)
			{
				RKString sub_name = "";
				for (int i = 1; i < name_list.Size(); i++)
				{
					sub_name += name_list.GetAt(i);
					if (i != name_list.Size() - 1)
					{
						sub_name += ".";
					}
				}

				return layer_contain->GetWidgetChildByName(sub_name);
			}
			else
			{
				for (int i = 0; i < p_listWidgetChild.size(); i++)
				{
					auto widget_contain = p_listWidgetChild.at(i)->GetWidgetChildByName(name_path);
					if (widget_contain)
						return widget_contain;
				}
			}
			return nullptr;
		}

		LayerEntity	 * LayerEntity::GetLayerChildByName(RKString name_path)
		{
			auto name_list = name_path.Split(".");

			for (unsigned int i = 0; i < p_listLayerChild.size(); i++)
			{
				if (p_listLayerChild.at(i)->GetName() == name_list.GetAt(0))
				{
					if (name_list.Size() == 1 || name_list.GetAt(1) == "")
					{
						return p_listLayerChild.at(i);
					}
					else
					{
						RKString sub_name = "";
						for (unsigned int j = 1; j < name_list.Size(); j++)
						{
							sub_name += name_list.GetAt(j);
							if (j != name_list.Size() - 1)
							{
								sub_name += ".";
							}
						}
						return p_listLayerChild.at(i)->GetLayerChildByName(sub_name);
					}
				}
			}

			return 0;
		}

		bool LayerEntity::HaveChildWithIdxAppear(int checked_idx_appear)
		{
			for (int i = 0; i < p_listWidgetChild.size(); i++)
			{
				auto child = p_listWidgetChild.at(i);
				if (child->GetIndexAppear() == checked_idx_appear)
				{
					return true;
				}
				else
				{
					if (child->ContainChildIdxAppear(checked_idx_appear) == true) { return true; }
				}
			}

			for (int j = 0; j < p_listLayerChild.size(); j++)
			{
				auto child = p_listLayerChild.at(j);

				if (child->GetIndexAppear() != checked_idx_appear)
				{
					for (int i = 0; i < child->GetListLayerChild().size(); i++)
					{
						bool value = child->GetListLayerChild().at(i)->HaveChildWithIdxAppear(checked_idx_appear);
						if (value)
							return value;
					}
				}
				else
				{
					return true;
				}
			}

			return false;
		}

		bool LayerEntity::UpdateActionProcessChild(float dt, int current_idx)
		{
			bool IsAllfinish = true;
			for (int i = 0; i < p_listWidgetChild.size(); i++)
			{
				auto child = p_listWidgetChild.at(i);
				if (child->GetIndexAppear() != current_idx)
				{
					continue;
				}
				IsAllfinish &= child->UpdateActionProcess(dt);
			}

			for (int j = 0; j < p_listLayerChild.size(); j++)
			{
				auto child = p_listLayerChild.at(j);

				if (child->GetIndexAppear() != current_idx)
				{
					for (int i = 0; i < child->GetListLayerChild().size(); i++)
					{
						IsAllfinish &= child->GetListLayerChild().at(i)->UpdateActionProcessChild(dt, current_idx);
					}
					continue;
				}
				IsAllfinish &= child->UpdateActionProcess(dt);
			}

			return IsAllfinish;
		}
		
		void LayerEntity::RegisterUIWidget()
		{
			p_factory_ui_widget.RegisterProduct<ButtonWidget>(xml::TYPE_UI_WIDGET::WIDGET_BUTTON);
			p_factory_ui_widget.RegisterProduct<LayoutWidget>(xml::TYPE_UI_WIDGET::WIDGET_LAYOUT);
			p_factory_ui_widget.RegisterProduct<TextWidget>(xml::TYPE_UI_WIDGET::WIDGET_FONT);
			p_factory_ui_widget.RegisterProduct<PanelWidget>(xml::TYPE_UI_WIDGET::WIDGET_PANEL);
			p_factory_ui_widget.RegisterProduct<ScrollViewWidget>(xml::TYPE_UI_WIDGET::WIDGET_SCROLL_VIEW);
			p_factory_ui_widget.RegisterProduct<ListViewWidget>(xml::TYPE_UI_WIDGET::WIDGET_LIST_VIEW);
			p_factory_ui_widget.RegisterProduct<PageViewWidget>(xml::TYPE_UI_WIDGET::WIDGET_PAGE_VIEW);
			p_factory_ui_widget.RegisterProduct<TextFieldWidget>(xml::TYPE_UI_WIDGET::WIDGET_TEXT_FIELD);
			p_factory_ui_widget.RegisterProduct<CheckBoxWidget>(xml::TYPE_UI_WIDGET::WIDGET_CHECK_BOX);
		}

		void LayerEntity::SetInteractive(bool value)
		{
			for (int i = 0; i < p_listWidgetChild.size(); i++)
			{
				p_listWidgetChild.at(i)->SetInteractive(value);
			}
			
			for (int i = 0; i < p_listLayerChild.size(); i++)
			{
				p_listLayerChild.at(i)->SetInteractive(value);
			}
            p_IsInteractive = value;
		}
	}
}