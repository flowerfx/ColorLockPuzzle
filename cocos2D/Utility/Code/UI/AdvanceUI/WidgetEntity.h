#ifndef __WIDGET_ENTITY_H__
#define __WIDGET_ENTITY_H__

#include "ActionProcess.h"

#include "RKBaseLog/Debug.h"
using namespace RKUtils;
using namespace ui;
namespace Utility
{
	namespace UI_Widget
	{
		class LayerEntity;
		class WidgetEntity : public ActionProcess
		{
		protected:
			//widget object
			Widget*		p_Resource;
			//list child widget
			std::vector<WidgetEntity*> p_listChildWidget;
			//
			bool p_isScale9Enable;
		public:
			virtual cocos2d::Node * GetResource() {
					return p_Resource;
			}
			virtual bool SetNewResource(cocos2d::Node * resource)
			{
				if (p_Resource)
				{
					//delete p_Resource;
					//p_Resource = nullptr;
					p_Resource = static_cast<Widget*>(resource);
				}
				else
				{
					return false;
				}

				return true;
			}

			virtual WidgetEntity * Clone() = 0;

			virtual void CloneChild(WidgetEntity * p_clone);

			WidgetEntity();
			virtual ~WidgetEntity();

			virtual bool Update(float dt);

			virtual void InitParamParent(RKString name, xml::UIWidgetDec * xml_value, LayerEntity * layer_parent , WidgetEntity * parent_widget);

			virtual void InitTexture(xml::UIWidgetDec * xml_value) = 0;

			virtual void SetPosValueXML(xml::UIWidgetDec * xml_value);

			virtual void FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value);

			virtual void InsertChildWidget(WidgetEntity* child_widget);

			unsigned int GetNumberChildWidget() const { return p_listChildWidget.size();  }

			bool ContainChildIdxAppear(unsigned int idx_appear);

			int SetProcessActionChildIdxAppear(STATUS_MENU fade_state, unsigned int current_idx);

			virtual WidgetEntity * GetWidgetChildByName(RKString name_path);
			virtual WidgetEntity * GetWidgetChildAtIdx(int idx);

			virtual void SetInteractive(bool value);

			virtual void ResizeAndPosChildWidget(WidgetEntity * child);

			virtual void RemoveAllChild();

			virtual void AddChildWidget(WidgetEntity * widget_);

			virtual void SetSize(Vec2 s);
			virtual Vec2 GetSize();

		};
	}
}

#endif //__WIDGET_ENTITY_H__

