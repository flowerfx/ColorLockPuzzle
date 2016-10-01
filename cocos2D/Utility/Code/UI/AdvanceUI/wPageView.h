#ifndef __PAGE_WIDGET_H__
#define __PAGE_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class LayoutWidget;
		class PageViewWidget : public WidgetEntity
		{
		private:
			LayoutWidget * p_refer_child;
		public:
			PageViewWidget();
			virtual ~PageViewWidget();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);
			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			virtual void SetSizeValueXML(xml::UIWidgetDec * xml_value);

			virtual void FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value);

			virtual WidgetEntity * Clone();

			WidgetEntity * CreateWidgetReferAtIdx(int idx);

			void PushBackItem(WidgetEntity * item);

			int GetCurrentPageIdx();

			void JumpToPageIdx(int idx);
		};

	}
}

#endif //__SCROLL_WIDGET_H__

