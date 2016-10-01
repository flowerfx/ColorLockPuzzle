#ifndef __W_OBJECT_H__
#define __W_OBJECT_H__

#include "../LoadDataXML.h"
#include "../UI/AdvanceUI/ActionCommand.h"
#include "cocostudio/CocoStudio.h"

#include "cocos2d.h"

using namespace cocos2d;
using namespace cocostudio;

namespace Utility
{
	using namespace UI_Widget;
	namespace wObject
	{

		enum TYPE_OBJECT
		{
			OBJ_NONE,
			OBJ_SPRITE,
			OBJ_ANIMATION,
			OBJ_ARMATURE,
			OBJ_LABEL,
			OBJ_PROCESS_TIMER,
			OBJ_COUNT
		};

		enum STATUS_ACTION
		{
			STATUS_IDLE = 0,
			STATUS_ACTIVE
		};


		struct ActionQueue : public Ref
		{
			RKString action_name;
			unsigned int p_action_tag;
			cocos2d::Sequence * p_action;

			ActionQueue(unsigned int tag, cocos2d::Sequence * action , RKString name)
				:p_action_tag(tag),
				p_action(action),
				action_name(name)
			{

			}

			virtual ~ActionQueue()
			{
				p_action_tag = 0;
				CC_SAFE_DELETE(p_action);
			}
		};

		class wBasicObject : public Ref
		{
		private:
			RKString		   p_name_cotain;
			cocos2d::Node	*  p_Resource;
			//
			TYPE_OBJECT			p_TypeObject;
			//
			std::map<RKString, ActionCommand *>	p_list_command;
			//
			xml::BasicDec		* p_valueContain;
			//
			wBasicObject * p_parent_object;
			Vector<wBasicObject *> p_listChildObject;

			//
			bool p_OnActionLock;

			//
			Vector<ActionQueue*> p_ActionListQueue;

			//
			Vec2 design_size;

		protected:

			void FreeObject();

			virtual void LoadObjectFromSprite(Sprite * p, bool UseAntiAlias = false);
			virtual void LoadAnimateFromEditor(xml::AnimatureDec * animature_dec);
			virtual void LoadFontDecFromXML(xml::FontDec * font_dec);
			virtual void LoadAnimateFromXML(xml::AnimateDec * animation_dec);
			virtual void LoadValueBasicObject(xml::Wide_ObjectDec * object_dec);
			virtual void LoadFirstValue(xml::Wide_ObjectDec * object_dec);
			virtual void SetCommandValueXML(xml::Wide_ObjectDec * xml_value);

			virtual void ResizeAndPosChildWidget(wBasicObject * child);
			virtual void UpdateActionProcess(float dt);

			virtual bool OnInitWideObjectXML(xml::Wide_ObjectDec * objec, bool UseAntiAlias = false);

		public:
			//
			cocos2d::Node*  GetResource();

			void SetBlendFunc(BlendFunc blend);

			wBasicObject();
			virtual ~wBasicObject();

			virtual void Flip(bool UseY, bool b);

			virtual void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)  ;
			virtual void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags) ;
			virtual void Update(float dt);

			virtual bool OnInitWideObject(RKString name, bool UseAntiAlias = false);
			virtual bool SetActionCommandWithName(RKString name, CallFunc * call_back = nullptr);

			virtual bool SetActionCustomWithName(RKString name, cocos2d::Sequence* action_custom, CallFunc * call_back = nullptr);
			
			virtual wBasicObject * OnLoadChildObject(wBasicObject * parent, xml::Wide_ObjectDec * objec);

			RKString GetName() { return p_name_cotain; }

			virtual wBasicObject * GetObjectByName(RKString name_path);

			virtual bool OnFinishAction(); //return is remove its or not
			virtual void EventActionFinish();

			virtual bool SetHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail, xml::BasicDec  value, RKString name_value);
			virtual bool ClearHookCommand(RKString name_command, RKString idx_act, int idx_squence, int idx_detail);

			void StopCurrentActionWithName(RKString name);
			/*

			GET / SET method of basic object

			*/
#pragma region normal_method
			virtual void SetSize(Vec2 Size);
			virtual Vec2 GetSize();

			virtual void SetScale(Vec2 scale);
			virtual Vec2 GetScale();

			virtual void SetOpacity(RKUtils::BYTE opacity);
			virtual RKUtils::BYTE GetOpacity();

			virtual void SetAngle(float angle);
			virtual float GetAngle();

			virtual void SetColor(Color4B color);
			virtual Color4B GetColor();

			virtual void SetPosition(Vec2 pos);
			virtual Vec2 GetPosition();

			cocos2d::Rect GetBound();

			bool IsVisible();
			void SetVisible(bool val);

			void SetText(RKString str){
				if (p_TypeObject == TYPE_OBJECT::OBJ_LABEL)
				{
					static_cast<Label*>(p_Resource)->setString(str.GetString());
				}
			}

			xml::BasicDec		* GetValueContain(){ return p_valueContain; }
#pragma endregion normal_method
		};
	}
}
#endif //__W_OBJECT_H__

