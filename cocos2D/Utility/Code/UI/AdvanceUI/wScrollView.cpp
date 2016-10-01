#include "wScrollView.h"
#include <string.h>
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		ScrollViewWidget::ScrollViewWidget()
		{
			p_typeUI = UI_TYPE::UI_SCROLLVIEW;

		}
		ScrollViewWidget::~ScrollViewWidget()
		{

		}

		WidgetEntity * ScrollViewWidget::Clone()
		{
			ScrollViewWidget * p_clone = new ScrollViewWidget();
			//copy all value (excep pointer)
#ifdef OS_IOS
            memcpy(p_clone,this,sizeof(p_clone));
#else
            memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
#endif
			//copy resource
			p_clone->p_Resource = p_Resource->clone();
			//remove child remain
			p_clone->p_Resource->removeAllChildrenWithCleanup(true);
			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}

		/////////////////////////////////////////////////////////////
		void ScrollViewWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
		}

		void ScrollViewWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{

			ScrollView* panel = ScrollView::create();

			panel->setScrollBarWidth(4);
			panel->setScrollBarPositionFromCorner(Vec2(2, 2));
			panel->setScrollBarColor(Color3B::WHITE);

			Vec2 inner_size = xml_value->GeneralValue->GetDataVector2("inner_size");

			inner_size.x = inner_size.x * GetGameSize().width / xml_value->p_menu_widget->DesignSize.x;
			inner_size.y = inner_size.y * GetGameSize().height / xml_value->p_menu_widget->DesignSize.y;
            panel->setInnerContainerSize(cocos2d::Size(inner_size.x, inner_size.y));
			panel->setBounceEnabled(true);

			ui::ScrollView::Direction direction = (ui::ScrollView::Direction)xml_value->GeneralValue->GetDataInt("direction");
			panel->setDirection(direction);

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
		
		void ScrollViewWidget::SetSizeValueXML(xml::UIWidgetDec * xml_value)
		{
			ActionProcess::SetSizeValueXML(xml_value);

			static_cast<ScrollView*>(p_Resource)->jumpToTop();
		}

	}
}