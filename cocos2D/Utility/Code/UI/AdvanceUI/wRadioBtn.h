#ifndef __BUTTON_WIDGET_H__
#define __BUTTON_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class RadioBtnWidget : public WidgetEntity
		{
		private:
			xml::sound_click_dec *	ID_SOUND_CLICK;
		public:
			RadioBtnWidget();
			virtual ~RadioBtnWidget();

			virtual WidgetEntity * Clone();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			xml::sound_click_dec* GetIdSound() { return ID_SOUND_CLICK; }

			virtual void SetInteractive(bool value);

		};

	}
}

#endif //__BUTTON_ENTITY_H__

