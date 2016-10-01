#include "wRadioBtn.h"
#include "MenuWidget.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		RadioBtnWidget::RadioBtnWidget()
		{
			p_typeUI = UI_TYPE::UI_RADIO_BTN;
			p_IsInteractive = true;
		}
		RadioBtnWidget::~RadioBtnWidget()
		{
			ID_SOUND_CLICK = nullptr; //not delete its
		}
		
		WidgetEntity * RadioBtnWidget::Clone()
		{
			RadioBtnWidget * p_clone = new RadioBtnWidget();
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
		void RadioBtnWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
			ID_SOUND_CLICK = xml_value->sound_click;
		}

		void RadioBtnWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{
			bool IsUseAtlas = (xml_value->GeneralValue->GetDataInt("use_atlas") == 1);

			RKString on_state = xml_value->GeneralValue->GetDataChar("on_state");
			RKString off_state = xml_value->GeneralValue->GetDataChar("off_state");

			auto p_button_refer = RadioButton::create(on_state.GetString(), off_state.GetString(), IsUseAtlas ? Widget::TextureResType::PLIST : Widget::TextureResType::LOCAL);

			p_Resource = p_button_refer;
		}
		

		void RadioBtnWidget::SetInteractive(bool value)
		{
			WidgetEntity::SetInteractive(value);
			//static_cast<RadioButton*>(p_Resource)->setPressedActionEnabled(value);
			p_IsInteractive = value;
		}


	}
}