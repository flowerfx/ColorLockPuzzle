#include "wCheckBox.h"
#include "MenuWidget.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		CheckBoxWidget::CheckBoxWidget()
		{
			p_typeUI = UI_TYPE::UI_CHECK_BOX;
			p_IsInteractive = true;
		}
		CheckBoxWidget::~CheckBoxWidget()
		{
			ID_SOUND_CLICK = nullptr; //not delete its
		}

		WidgetEntity * CheckBoxWidget::Clone()
		{
			CheckBoxWidget * p_clone = new CheckBoxWidget();
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
			//copy sound click
			p_clone->ID_SOUND_CLICK = new xml::sound_click_dec();
			memcpy(p_clone->ID_SOUND_CLICK, ID_SOUND_CLICK, sizeof(ID_SOUND_CLICK));
			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;

		}

		/////////////////////////////////////////////////////////////
		void CheckBoxWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
			ID_SOUND_CLICK = xml_value->sound_click;
		}

		void CheckBoxWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{
			bool IsUseAtlas = (xml_value->GeneralValue->GetDataInt("use_atlas") == 1);

			RKString background = xml_value->GeneralValue->GetDataChar("source");
			RKString select = xml_value->GeneralValue->GetDataChar("select");
			RKString cross = xml_value->GeneralValue->GetDataChar("cross");
			RKString disable = xml_value->GeneralValue->GetDataChar("backgrounddisable");
			RKString cross_disable = xml_value->GeneralValue->GetDataChar("frontCrossDisabled");



			ui::CheckBox * _widget = nullptr;

			_widget = CheckBox::create(
				background.GetString(), 
				select.GetString(), 
				cross.GetString(),
				disable.GetString(),
				cross_disable.GetString(),
					IsUseAtlas ? Widget::TextureResType::PLIST : Widget::TextureResType::LOCAL);



			p_Resource = _widget;
		}


		void CheckBoxWidget::SetInteractive(bool value)
		{
			WidgetEntity::SetInteractive(value);
			static_cast<ui::CheckBox *>(p_Resource)->setEnabled(value);
			p_IsInteractive = value;
		}


		void CheckBoxWidget::ResizeAndPosChildWidget(WidgetEntity * child)
		{
			//set new size for widget child
			float scale_parent_widget_x = this->GetSize().x / this->GetResource()->getContentSize().width;
			float scale_parent_widget_y = this->GetSize().y / this->GetResource()->getContentSize().height;

			Vec2 design_size = child->GetMenuParent()->GetDesignSize();

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
	}
}