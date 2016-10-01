#ifndef __SCROLL_WIDGET_H__
#define __SCROLL_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class ScrollViewWidget : public WidgetEntity
		{
		private:
			

		public:
			ScrollViewWidget();
			virtual ~ScrollViewWidget();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);
			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			virtual void SetSizeValueXML(xml::UIWidgetDec * xml_value);

			virtual WidgetEntity * Clone();
		};

	}
}

#endif //__SCROLL_WIDGET_H__

