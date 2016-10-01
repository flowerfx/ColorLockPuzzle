#ifndef __LAYOUT_WIDGET_H__
#define __LAYOUT_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class LayoutWidget : public WidgetEntity
		{
		private:
			cocos2d::EventListenerFocus* _eventListener;
			bool p_isfocusEnable;
		protected:

			void CreateLayout();

		public:
			LayoutWidget();
			virtual ~LayoutWidget();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual void InitTexture(xml::UIWidgetDec * xml_value) final {} ;

			virtual void SetInitPos(xml::UIWidgetDec * xml_value);

			virtual void SetPosValueXML(xml::UIWidgetDec * xml_value);

			virtual void FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value);

			virtual WidgetEntity * Clone();

			void createFocusEvent(std::function<void(ui::Widget*, ui::Widget*)> func_call_back);

			bool IsFocusEnable() { return p_isfocusEnable; }
		};

	}
}

#endif //__LAYOUT_WIDGET_H__

