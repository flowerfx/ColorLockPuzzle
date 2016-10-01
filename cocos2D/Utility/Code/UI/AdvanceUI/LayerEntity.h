#ifndef __LAYER_ENTITY_H__
#define __LAYER_ENTITY_H__

#include "WidgetEntity.h"
#include "../../Factory.h"
using namespace RKUtils;
using namespace ui;
namespace Utility
{
	namespace UI_Widget
	{
		class LayerEntity : public Layer, public ActionProcess
		{
		private:

			std::vector<WidgetEntity* > p_listWidgetChild;

			std::vector<LayerEntity* >	p_listLayerChild;

			Factory::FactoryWithIndex<WidgetEntity>	p_factory_ui_widget;

		protected:

			int p_current_idx_appear;

			int p_number_child_have_action;

			void RegisterUIWidget();

		protected:

			std::function<void(MenuWidget&, RKString, UI_TYPE)> func_call_widget;

			void AddWidgetWithName(Widget * element, RKString name);

			virtual void OnTouchEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, WidgetEntity* _widget);
			virtual void OnSelectListItem(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type, WidgetEntity* _widget);
			virtual void OnTextFieldEvent(cocos2d::Ref* sender, TextField::EventType type, WidgetEntity* _widget);
			virtual void OnLayoutFocusChangeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, WidgetEntity* _widget);
			virtual void SetInitPos(xml::UIWidgetDec * xml_value);
		public:

			virtual cocos2d::Node * GetResource() 
			{
					return this;
			}

			virtual bool SetNewResource(cocos2d::Node * resource)
			{
				return false;
			}

			Widget * GetWidgetElementByName(RKString name)
			{
				return static_cast<Widget*>(getChildByName(name.GetString()));
			}

			WidgetEntity * GetWidgetEntityByName(RKString name)
			{
				for (unsigned int i = 0; i < p_listWidgetChild.size(); i++)
				{
					if (p_listWidgetChild.at(i)->GetName() == name)
					{
						return p_listWidgetChild.at(i);
					}
				}
				return 0;
			}

			//LayerEntity * GetLayerChildByName(RKString name)
			//{
			//	//return static_cast<LayerEntity*>(getChildByName(name.GetString()));

			//	for (int i = 0; i < p_listLayerChild.size(); i++)
			//	{
			//		if (p_listLayerChild.at(i)->GetName() == name)
			//		{
			//			return p_listLayerChild.at(i);
			//		}
			//	}
			//	return 0;
			//}

			void AddLayerEntity(LayerEntity * layer);

			void AddWidgetEntity(WidgetEntity * layer);

			LayerEntity();
			virtual ~LayerEntity();

			virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
			virtual void visit(Renderer *renderer, const Mat4& transform, uint32_t flags);

			virtual bool Update(float dt);

			virtual void SetPosValueXML(xml::UIWidgetDec * xml_value);

			virtual bool IsAllChildFinishAction();

			WidgetEntity * InitWidgetEntity(xml::UILayerWidgetDec * widget, LayerEntity * parent_layer, WidgetEntity * parent_widget);

			void SetCallBack(std::function<void(MenuWidget&, RKString, UI_TYPE)> func);

			void SetActionProcessForAllChild(STATUS_MENU fade_state, unsigned int current_idx);

			void SetIdxProcessGame(int idx) { p_current_idx_appear = idx; }

			std::vector<LayerEntity* > GetListLayerChild() { return p_listLayerChild; }

			void HaveChildFinishAction() { 
				p_number_child_have_action--;

				if (p_number_child_have_action < 0)
				{
					p_number_child_have_action = 0;
				}
			}

			bool UpdateActionProcessChild(float dt, int current_idx);

			bool HaveChildWithIdxAppear(int checked_idx_appear);

			virtual WidgetEntity * GetWidgetChildAtIdx(int idx);
			virtual WidgetEntity * GetWidgetChildByName(RKString name_path);
			virtual LayerEntity	 * GetLayerChildByName(RKString name_str);

			virtual void SetInteractive(bool value);
		};
	}
}

#endif //__LAYER_ENTITY_H__

