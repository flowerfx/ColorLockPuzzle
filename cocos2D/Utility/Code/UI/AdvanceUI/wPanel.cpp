#include "wPanel.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		PanelWidget::PanelWidget()
		{
			p_typeUI = UI_TYPE::UI_PANEL;

		}
		PanelWidget::~PanelWidget()
		{

		}
		
		WidgetEntity * PanelWidget::Clone()
		{
			PanelWidget * p_clone = new PanelWidget();
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
			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}

		/////////////////////////////////////////////////////////////
		void PanelWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
		}

		void PanelWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{
			auto source_texture = xml_value->GeneralValue->GetDataChar("source");
			bool IsUseAtlas = (xml_value->GeneralValue->GetDataInt("use_atlas") == 1);

			ImageView* panel = nullptr;
			if (IsUseAtlas)
			{
				panel = ImageView::create(source_texture,Widget::TextureResType::PLIST);
			}
			else
			{
				panel = ImageView::create(source_texture);
			}
			bool IsScale9Enable = (xml_value->GeneralValue->GetDataInt("scale_9") == 1);
			p_isScale9Enable = IsScale9Enable;
			panel->setScale9Enabled(IsScale9Enable);
			p_Resource = panel;
		}
		

	}
}