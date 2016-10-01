#ifndef __PANEL_WIDGET_H__
#define __PANEL_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class PanelWidget : public WidgetEntity
		{
		private:
			

		public:
			PanelWidget();
			virtual ~PanelWidget();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);
			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			virtual WidgetEntity * Clone();
		};

	}
}

#endif //__PANEL_WIDGET_H__

