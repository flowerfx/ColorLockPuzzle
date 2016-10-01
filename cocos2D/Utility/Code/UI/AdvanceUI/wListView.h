#ifndef __LIST_WIDGET_H__
#define __LIST_WIDGET_H__

#include "WidgetEntity.h"
#include "RKString_Code/RKString.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class LayoutWidget;
		class ListViewWidget : public WidgetEntity
		{
		private:
			LayoutWidget * p_refer_child;
			int current_idx_selected;
		public:
			ListViewWidget();
			virtual ~ListViewWidget();

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);
			virtual void InitTexture(xml::UIWidgetDec * xml_value);

			virtual void SetSizeValueXML(xml::UIWidgetDec * xml_value);

			virtual void FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value);

			virtual void InsertChildWidget(WidgetEntity* child_widget);

			WidgetEntity * CreateWidgetRefer(int idx);

			void PushBackItem(WidgetEntity * item);

			virtual WidgetEntity * Clone();

			int GetCurrentIdxSelected() { return current_idx_selected; }
			void SetCurrentIdxSelected(int idx) { current_idx_selected = idx; }

			void JumpToItemIdx(int idx);
		};

	}
}

#endif //__LIST_WIDGET_H__

