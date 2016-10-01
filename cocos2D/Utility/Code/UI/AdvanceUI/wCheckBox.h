#ifndef __CHECK_BOX_WIDGET_H__
#define __CHECK_BOX_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class CheckBoxWidget : public WidgetEntity
		{
		private:
			xml::sound_click_dec *	ID_SOUND_CLICK;


		public:
			CheckBoxWidget();
			virtual ~CheckBoxWidget();

			virtual WidgetEntity * Clone();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			xml::sound_click_dec* GetIdSound() { return ID_SOUND_CLICK; }

			virtual void SetInteractive(bool value);

			virtual void ResizeAndPosChildWidget(WidgetEntity * child);


		};

	}
}

#endif //__CHECK_BOX_WIDGET_H__

