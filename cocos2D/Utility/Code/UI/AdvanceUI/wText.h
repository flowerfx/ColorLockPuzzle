#ifndef __TEXT_WIDGET_H__
#define __TEXT_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class TextWidget : public WidgetEntity
		{
		private:
			RKString	p_current_text;
			int			p_border_width;
			Color4B		p_border_color;
			int			p_min_size_scale;
		public:
			TextWidget();
			virtual ~TextWidget();

			virtual void SetSize(Vec2 s);

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			virtual void SetColor(Color3B p);

			RKString GetText();

			void SetText(RKString text);

			virtual WidgetEntity * Clone();
		};

	}
}

#endif //__TEXT_WIDGET_H__

