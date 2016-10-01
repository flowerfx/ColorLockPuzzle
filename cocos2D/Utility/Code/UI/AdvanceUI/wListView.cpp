#include "wListView.h"
#include "LayerEntity.h"
#include "wLayout.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		ListViewWidget::ListViewWidget()
		{
			p_typeUI = UI_TYPE::UI_LISTVIEW;
			p_refer_child = nullptr;
			current_idx_selected = -1;

		}
		ListViewWidget::~ListViewWidget()
		{
			p_refer_child = nullptr;
		}

		WidgetEntity * ListViewWidget::Clone()
		{
			ListViewWidget * p_clone = new ListViewWidget();
			//copy all value (excep pointer)
#ifdef OS_IOS
			memcpy(p_clone, this, sizeof(p_clone));
#else
			memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
#endif
			//copy resource
			p_clone->p_Resource = p_Resource->clone();
			//remove child remain
			p_clone->p_Resource->removeAllChildrenWithCleanup(true);
			//copy refer
			p_clone->p_refer_child = static_cast<LayoutWidget*>(p_refer_child->Clone());
			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}

		/////////////////////////////////////////////////////////////
		void ListViewWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
		}

		void ListViewWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{

			ListView* panel = ListView::create();

			panel->setScrollBarWidth(1);
			panel->setScrollBarPositionFromCorner(Vec2(0, 2));
			panel->setScrollBarColor(Color3B::WHITE);

			Vec2 inner_size = xml_value->GeneralValue->GetDataVector2("inner_size");

			inner_size.x = inner_size.x * GetGameSize().width / xml_value->p_menu_widget->DesignSize.x;
			inner_size.y = inner_size.y * GetGameSize().height / xml_value->p_menu_widget->DesignSize.y;
			panel->setInnerContainerSize(cocos2d::Size(inner_size.x, inner_size.y));
			panel->setBounceEnabled(true);

			ui::ScrollView::Direction direction = (ui::ScrollView::Direction)xml_value->GeneralValue->GetDataInt("direction");
			panel->setDirection(direction);

			//// set all items layout gravity
			panel->setGravity(ListView::Gravity::CENTER_VERTICAL);
			//
			//// set items margin
			//panel->setItemsMargin(3.0f);

			int background_type = xml_value->GeneralValue->GetDataInt("background_type");

			if (background_type >= 0 && background_type <= 2)
			{
				panel->setBackGroundColorType((ui::Layout::BackGroundColorType)background_type);

				if (background_type == 1)
				{
					Color4B color_layout = xml_value->GeneralValue->GetDataColor("tint");
					panel->setBackGroundColor(Color3B(color_layout.r, color_layout.g, color_layout.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					panel->setBackGroundColorOpacity(opacity_layout);
				}
				else if (background_type == 2)
				{
					Color4B color_layout_from = xml_value->GeneralValue->GetDataColor("tint");
					Color4B color_layout_to = xml_value->GeneralValue->GetDataColor("tint_to");
					panel->setBackGroundColor(Color3B(color_layout_from.r, color_layout_from.g, color_layout_from.b),
						Color3B(color_layout_to.r, color_layout_to.g, color_layout_to.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					panel->setBackGroundColorOpacity(opacity_layout);
				}
			}
			else if (background_type == 3)
			{
				RKString name_image = xml_value->GeneralValue->GetDataChar("source");
				bool IsUseAtlast = xml_value->GeneralValue->GetDataInt("use_atlas") == 1;

				auto t_sprite = TextureMgr->GetSpriteByName(name_image.GetString());
				Vec2 size_srpite = t_sprite->getContentSize();
				//p_Resource->setContentSize(Size(size_srpite.x, size_srpite.y));

				panel->setBackGroundImage(name_image.GetString(), IsUseAtlast ? Widget::TextureResType::PLIST : Widget::TextureResType::LOCAL);
				
				bool IsScale9Enable = (xml_value->GeneralValue->GetDataInt("scale_9") == 1);
				p_isScale9Enable = IsScale9Enable;
				panel->setBackGroundImageScale9Enabled(IsScale9Enable);
			}
			else
			{
				PWARN1("Wrong input !");
			}
			//panel->setInnerContainerPosition(panel->getSize());
			p_Resource = panel;
		}

		void ListViewWidget::SetSizeValueXML(xml::UIWidgetDec * xml_value)
		{
			ActionProcess::SetSizeValueXML(xml_value);

			static_cast<ListView*>(p_Resource)->jumpToTop();
		}

		void ListViewWidget::FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value)
		{
			//PASSERT2(xml_value->p_list_child_widget.size() == 1, "the sample list must have 1 value to refer");
			for (int i = 0; i < xml_value->p_list_child_widget.size(); i++)
			{
				auto _widget_child = this->GetParentLayer()->InitWidgetEntity(xml_value->p_list_child_widget.at(i), this->GetParentLayer(), this);
				_widget_child->SetMenuParent(this->GetMenuParent());
				_widget_child->SetParentWidget(this);

				if (_widget_child->getUIWidgetType() != UI_TYPE::UI_LAYOUT
					||
					xml_value->p_list_child_widget.at(i)->GeneralValue->GetDataInt("not_default_layout") == 1
					)
				{
					int idx = static_cast<ListView*>(p_Resource)->getChildrenCount();
					static_cast<ListView*>(p_Resource)->pushBackCustomItem(static_cast<Widget*>(_widget_child->GetResource()));

					_widget_child->SetName(_widget_child->GetName() + std::to_string(idx));
					p_listChildWidget.push_back(_widget_child);
					_widget_child->GetResource()->setName((_widget_child->GetName() + std::to_string(idx)).GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
					_widget_child->GetResource()->setZOrder(p_listChildWidget.size());
					_widget_child->SetParentLayer(this->GetParentLayer());
				}
				else
				{
					p_refer_child = static_cast<LayoutWidget *>(_widget_child);
				}
			}
			//static_cast<ListView*>(p_Resource)->setItemModel(static_cast<Widget*>(_widget_child->GetResource()));
			//InsertChildWidget(_widget_child);
		}

		void ListViewWidget::InsertChildWidget(WidgetEntity* child_widget)
		{
			//p_listChildWidget.push_back(child_widget);
			//child_widget->GetResource()->setName(child_widget->GetName().GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
			//child_widget->GetResource()->setZOrder(p_listChildWidget.size());
			//child_widget->SetParentLayer(this->GetParentLayer());
			//GetResource()->addChild(child_widget->GetResource());
		}

		WidgetEntity * ListViewWidget::CreateWidgetRefer(int idx)
		{
			auto list_view = static_cast<ListView*>(p_Resource);

			LayoutWidget * _layout_widget = static_cast<LayoutWidget*>(p_refer_child->Clone());

			list_view->pushBackCustomItem(static_cast<Widget*>(_layout_widget->GetResource()));

			_layout_widget->SetName(_layout_widget->GetName() + std::to_string(idx));
			p_listChildWidget.push_back(_layout_widget);
			_layout_widget->GetResource()->setName((_layout_widget->GetName() + std::to_string(idx)).GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
			_layout_widget->GetResource()->setZOrder(p_listChildWidget.size());
			_layout_widget->SetParentLayer(this->GetParentLayer());

			return _layout_widget;

		}

		void ListViewWidget::PushBackItem(WidgetEntity * item)
		{
			auto list_view = static_cast<ListView*>(p_Resource);
			list_view->pushBackCustomItem(static_cast<Widget*>(item->GetResource()));
		}

		void ListViewWidget::JumpToItemIdx(int idx)
		{
			static_cast<ListView*>(p_Resource)->jumpToItem(idx, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
		}
	}
}