#ifndef __BUTTON_WIDGET_H__
#define __BUTTON_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class ButtonWidget : public WidgetEntity
		{
		private:
			xml::sound_click_dec *	ID_SOUND_CLICK;
			bool p_is_only_text;
			Color3B p_color_text;
			int		p_opacity_text;
		public:
			ButtonWidget();
			virtual ~ButtonWidget();

			virtual WidgetEntity * Clone();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			xml::sound_click_dec* GetIdSound() { return ID_SOUND_CLICK; }

			virtual void SetInteractive(bool value);

			virtual void ResizeAndPosChildWidget(WidgetEntity * child);

			virtual void SetColor(Color3B p);
			virtual void SetSize(Vec2 s);

		};

	}
}

#endif //__BUTTON_ENTITY_H__

