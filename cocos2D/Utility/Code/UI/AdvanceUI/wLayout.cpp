#include "wLayout.h"
#include "LayerEntity.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		LayoutWidget::LayoutWidget()
		{
			p_typeUI = UI_TYPE::UI_LAYOUT;
			_eventListener = nullptr;
			p_isfocusEnable = false;
		}
		LayoutWidget::~LayoutWidget()
		{
			GetResource()->getEventDispatcher()->removeEventListener(_eventListener);
		}
	
		WidgetEntity * LayoutWidget::Clone()
		{
			LayoutWidget * p_clone = new LayoutWidget();
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
			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}
		/////////////////////////////////////////////////////////////

		void LayoutWidget::CreateLayout()
		{
			auto layout_widget = Layout::create();
			//layout_widget->setLayoutComponentEnabled(true);
			p_Resource = layout_widget;

		}

		void LayoutWidget::SetInitPos(xml::UIWidgetDec * xml_value)
		{
			auto pos_ratio = p_parent_layer ? xml_value->OriginValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->OriginValue->GetDataVector2("pos_ratio"));
			auto pos = Vec2(GetGameSize().width * pos_ratio.x, GetGameSize().height * pos_ratio.y);
			GetResource()->setAnchorPoint(Vec2(0.5, 0.5));

			SetPosition(pos);
			p_origin_value->ReplaceDataVector2("pos", pos);
		}

		void LayoutWidget::SetPosValueXML(xml::UIWidgetDec * xml_value)
		{
			auto originPosRatio = p_parent_layer ? xml_value->OriginValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->OriginValue->GetDataVector2("pos_ratio"));
			auto firstPosRatio = p_parent_layer ? xml_value->FirstValue->GetDataVector2("pos_ratio") : ConvertPos(xml_value->FirstValue->GetDataVector2("pos_ratio"));

			auto posOrigin = Vec2(GetGameSize().width * originPosRatio.x, GetGameSize().height * originPosRatio.y);
			auto posFirst = Vec2(GetGameSize().width * firstPosRatio.x, GetGameSize().height * firstPosRatio.y);

			PASSERT2(GetResource()->getContentSize().width != 0 || GetResource()->getContentSize().height != 0, "the content size of resource must bigger than 0");

			SetFirstPos(posFirst, xml_value->GeneralValue->GetDataInt("not_fade_pos") == 0);
		}

		void LayoutWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			CreateLayout();
			WidgetEntity::InitParam(name, xml_value);
			p_isfocusEnable = (xml_value->GeneralValue->GetDataInt("focus_enable") == 1);
			if (xml_value->GeneralValue->GetDataInt("clipping_enable") == 1)
			{
				static_cast<Layout*>(p_Resource)->setClippingEnabled(true);
			}
			else
			{
				static_cast<Layout*>(p_Resource)->setClippingEnabled(false);
			}

			int background_type = xml_value->GeneralValue->GetDataInt("background_type");

			if (background_type >= 0 && background_type <= 2)
			{
				static_cast<Layout*>(p_Resource)->setBackGroundColorType((ui::Layout::BackGroundColorType)background_type);

				if (background_type == 1)
				{
					Color4B color_layout = xml_value->GeneralValue->GetDataColor("tint");
					static_cast<Layout*>(p_Resource)->setBackGroundColor(Color3B(color_layout.r,color_layout.g,color_layout.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					static_cast<Layout*>(p_Resource)->setBackGroundColorOpacity(opacity_layout);
				}
				else if (background_type == 2)
				{
					Color4B color_layout_from = xml_value->GeneralValue->GetDataColor("tint");
					Color4B color_layout_to = xml_value->GeneralValue->GetDataColor("tint_to");
					static_cast<Layout*>(p_Resource)->setBackGroundColor(Color3B(color_layout_from.r, color_layout_from.g, color_layout_from.b) , 
						Color3B(color_layout_to.r, color_layout_to.g, color_layout_to.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					static_cast<Layout*>(p_Resource)->setBackGroundColorOpacity(opacity_layout);
				}
			}
			else if (background_type == 3)
			{
				RKString name_image = xml_value->GeneralValue->GetDataChar("source");
				bool IsUseAtlast = xml_value->GeneralValue->GetDataInt("use_atlas") == 1;

				auto t_sprite = TextureMgr->GetSpriteByName(name_image.GetString());
				Vec2 size_srpite = t_sprite->getContentSize();
				//p_Resource->setContentSize(Size(size_srpite.x, size_srpite.y));

				static_cast<Layout*>(p_Resource)->setBackGroundImage(name_image.GetString(), IsUseAtlast ? Widget::TextureResType::PLIST : Widget::TextureResType::LOCAL);
				bool IsScale9Enable = (xml_value->GeneralValue->GetDataInt("scale_9") == 1);

				p_isScale9Enable = IsScale9Enable;
				static_cast<Layout*>(p_Resource)->setBackGroundImageScale9Enabled(IsScale9Enable);
			
			}
			else
			{
				PWARN1("Wrong input !");
			}
		}
		
		void LayoutWidget::FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value)
		{
			for (int i = 0; i < xml_value->p_list_child_widget.size(); i++)
			{
				auto _widget_child = this->GetParentLayer()->InitWidgetEntity(xml_value->p_list_child_widget.at(i), this->GetParentLayer(),this);
				_widget_child->SetMenuParent(this->GetMenuParent());
				_widget_child->SetParentWidget(this);
				Vec2 scale_layout = this->GetScale();
				
				Vec2 scale_widget = _widget_child->GetScale();

				Vec2 additon_scale;

				additon_scale.x = scale_layout.x <= 1.f ? (1.f / scale_layout.x) : scale_layout.x;
				additon_scale.y = scale_layout.y <= 1.f ? (1.f / scale_layout.y) : scale_layout.y;

				scale_widget.x = scale_widget.x * additon_scale.x;
				scale_widget.y = scale_widget.y * additon_scale.y;

				//_widget_child->SetScale(scale_widget);
				InsertChildWidget(_widget_child);
			}
		}

		void LayoutWidget::createFocusEvent(std::function<void(ui::Widget*, ui::Widget*)> func_call_back)
		{
			_eventListener = EventListenerFocus::create();
			_eventListener->onFocusChanged = func_call_back;

			GetResource()->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListener, 1);
		}
	}
}