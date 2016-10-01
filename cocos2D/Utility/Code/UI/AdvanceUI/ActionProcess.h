#ifndef __ACTION_PROCESS_H__
#define __ACTION_PROCESS_H__

#include "../../DisplayView.h"
#include "../../ObjectTexture.h"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#include "ActionCommand.h"
using namespace RKUtils;
using namespace ui;
namespace Utility
{
	namespace UI_Widget
	{
		enum STATUS_MENU
		{
			M_IDLE = 0,
			M_FADEIN,
			M_FADEOUT
		};

		enum UI_TYPE
		{
			UI_NONE = 0,
			UI_BUTTON,
			UI_PANEL,
			UI_TEXT,
			UI_LOADING_BAR,
			UI_TEXT_FIELD,
			UI_CHECK_BOX,
			UI_SLIDER,
			UI_LISTVIEW,
			UI_SCROLLVIEW,
			UI_PAGEVIEW,
			UI_LAYOUT,
			UI_LAYER,
			UI_RADIO_BTN,
			UI_COUNT
		};
		class MenuWidget;
		class LayerEntity;
		class WidgetEntity;
		class ActionProcess : public Ref
		{
		protected:

			RKString	p_Name;
			UI_TYPE		p_typeUI;
			//action process in game , the fade action process with the parent element
			unsigned int p_action_process;
			unsigned int p_count_action_process; //use to know wherever all the action fade finish

			float		 p_timeAction;
			bool		 p_Visible;

			xml::BasicDec	*p_origin_value;
			xml::BasicDec   *p_first_value;

			//
			STATUS_MENU						p_currentStatusMenu;
			RKList<UI_STATE_FADE_ACTION>	p_l_fd;
			//
			unsigned int					p_index_appear;

			MenuWidget *					menu_point_to;
			LayerEntity *					p_parent_layer;
			WidgetEntity*					p_parent_widget;

			std::map<RKString,ActionCommand *>	p_list_command;

			

			bool p_IsInteractive;
		protected:
			Sequence * GetSequenceActionFromList(STATUS_MENU stateFade, RKList<UI_STATE_FADE_ACTION> l_fd, float time);
			//
			void AddActionFade(UI_STATE_FADE_ACTION fd)
			{
				p_action_process |= fd;
				p_count_action_process = p_action_process;
			}

			void RemoveActionFade(UI_STATE_FADE_ACTION fd)
			{
				p_action_process ^= fd;
				p_count_action_process = p_action_process;
			}

			void ResetActionFade()
			{
				p_action_process = 0;
				p_count_action_process = 0;
			}

			bool ContainActionFade(UI_STATE_FADE_ACTION fd)
			{
				return ( (p_action_process & fd) == fd);
			}

			virtual void SetSizeValueXML(xml::UIWidgetDec * xml_value);

			virtual void SetPosValueXML(xml::UIWidgetDec * xml_value);

			virtual void SetInitPos(xml::UIWidgetDec * xml_value);
			virtual void SetInitSize(xml::UIWidgetDec * xml_value);
			virtual void SetInitRotate(xml::UIWidgetDec * xml_value);

			virtual void SetCommandValueXML(xml::UIWidgetDec * xml_value);

			virtual void CloneThis(ActionProcess * p_clone);

		public:
            void ForceFinishAction();
            
			void SetActionFade(unsigned int fd)
			{
				p_action_process = fd;
				p_count_action_process = p_action_process;
			}

			void InitSetFade(RKList<UI_STATE_FADE_ACTION> l_fd, float time)
			{
				p_timeAction = time;
				p_l_fd = l_fd;
			}

			RKString GetName()
			{ 
				return p_Name; 
			}

			void SetName(RKString name)
			{
				p_Name = name;
			}

			virtual cocos2d::Node * GetResource() = 0;

			virtual bool SetNewResource(cocos2d::Node * new_resource) = 0;

			virtual void InitParam(RKString name, xml::UIWidgetDec * xml_value);

			virtual bool SetActionCommandWithName(RKString name , CallFunc * func_callback = nullptr);

			virtual bool SetHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail, xml::BasicDec  value, RKString name_value);
		
			virtual bool ClearHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail);

		public:

#pragma region normal_method
			ActionProcess();
			virtual ~ActionProcess();
			
			virtual bool Visible();
			virtual void Visible(bool b);

			virtual void SetScale(Vec2 s);
			virtual Vec2 GetScale();

			virtual Vec2 GetSize();
			virtual void SetSize(Vec2 s);

			virtual void SetRotate(float r);
			virtual float GetRotate();

			virtual void SetPosition(Vec2 p);
			virtual Vec2 GetPosition();

			virtual void SetOpacity(RKUtils::BYTE p);
			virtual RKUtils::BYTE GetOpacity();

			virtual void SetColor(Color3B p);
			virtual Color3B GetColor();

			unsigned int GetIndexAppear();
			void SetIndexAppear(unsigned int idx);

			Vec2 GetOriginPos(){ return p_origin_value->GetDataVector2("pos"); }
			Vec2 GetFirstPos() { return p_first_value->GetDataVector2("pos"); }
			void SetFirstPos(Vec2 p, bool force = true) { 
				p_first_value->ReplaceDataVector2("pos", p);
				if (force)
				{
					SetPosition(p);
				}
			}
			//
			float GetOriginAngle(){ return p_origin_value->GetDatafloat("rotate"); }
			float GetFirstAngle() { return p_first_value->GetDatafloat("rotate"); }
			void SetFirstAngle(float p, bool force = true) { 
				p_first_value->ReplaceDatafloat("rotate",p);
				if (force)
				{
					SetRotate(p);
				}
			}
			//
			RKUtils::BYTE GetOriginAlpha(){ return (RKUtils::BYTE)p_origin_value->GetDataInt("alpha"); }
			void SetOriginAlpha(RKUtils::BYTE p, bool force = false) {
				p_origin_value->ReplaceDataInt("alpha",(int)p);
				if (force)
				{
					SetOpacity(p);
				}
			}
			RKUtils::BYTE GetFirstAlpha() { return (RKUtils::BYTE)p_first_value->GetDataInt("alpha"); }
			void SetFirstAlpha(RKUtils::BYTE p, bool force = true) {
				p_first_value->ReplaceDataInt("alpha", (int)p);
				if (force)
				{
					SetOpacity(p);
				}
			}
			//
			Color3B GetOriginColor(){
				auto t = p_origin_value->GetDataColor("tint");
				return Color3B(t.r, t.g, t.b);
			}
			void SetOriginColor(Color3B p, bool force = false) {
				p_origin_value->ReplaceDataColor("tint", Color4B(p.r, p.g, p.b, 255));
				if (force)
				{
					SetColor(p);
				}
			}
			Color3B GetFirstColor() { 
				auto t = p_first_value->GetDataColor("tint");
				return Color3B(t.r, t.g, t.b);
			}
			void SetFirstColor(Color3B p, bool force = true) {
				p_first_value->ReplaceDataColor("tint", Color4B(p.r, p.g, p.b, 255));
				if (force)
				{
					SetColor(p);
				}
			}
			//
			float GetOriginScale(){ return p_origin_value->GetDatafloat("scale"); }
			float GetFirstScale() { return p_first_value->GetDatafloat("scale"); }
			void SetFirstSize(Vec2 p, bool force = true) {
				if (force)
				{
					SetSize(p);
				}
				p_first_value->ReplaceDatafloat("scale", GetResource()->getScaleX());
			}
			void SetFirstScale(float p, bool force = true) {	
				p_origin_value->ReplaceDatafloat("scale",  p);
					if (force)
					{
						SetScale(Vec2(p, p));
					}
			}

			virtual void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
			virtual void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags);

			virtual bool Update(float dt) = 0;

			LayerEntity * GetParentLayer() { return p_parent_layer; }

			void SetParentLayer(LayerEntity * parent);

			UI_TYPE getUIWidgetType(){ return p_typeUI; }

			void SetMenuParent(MenuWidget * menu) { menu_point_to = menu; }

			void SetParentWidget(WidgetEntity * widget) { p_parent_widget = widget; }

			MenuWidget * GetMenuParent() { return menu_point_to; }
#pragma endregion normal_method

#pragma region action_process_method
			bool SetActionProcess(STATUS_MENU stateFade, bool force_only = false);

			void ActionProcessFinish(UI_STATE_FADE_ACTION current_fd);
			void SetResourceRunAction(UI_STATE_FADE_ACTION action, int loop, bool reserve, float time = 0.f);
			void SetResourceRunManyAction(std::vector<Vector<FiniteTimeAction*>> list_action_);

			Vector<FiniteTimeAction*> CreateListActionFromSingleAction(UI_STATE_FADE_ACTION action, int loop, bool reserve, float time = 0.f);

			Sequence * CreateActionFromList(Vector<ActionInterval*> list_action);

			bool HaveActionNotFade();

			bool UpdateActionProcess(float dt);

			void RemoveActionByTag(bool IsAll = false);
#pragma endregion action_process_method

			virtual void SetValueFromXML(xml::UIWidgetDec * xml_value);

			void ResetSizeAndPos(int cur_width, int cur_height);

			virtual void FinishRunAction();

			virtual bool IsAllChildFinishAction() { return true; }

			virtual void SetInteractive(bool value){ p_IsInteractive = value; }

			virtual bool IsInteractive() { return p_IsInteractive; }
		};
	}
}

#endif //__ACTION_PROCESS_H__

