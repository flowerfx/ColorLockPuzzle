#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
namespace Utility
{
	namespace Control
	{
		enum CONTROL_TYPE
		{
			NONE = 0,
			MOVE_FORWARD,
			MOVE_BACKWARD,
			JUMP,
			BACK_KEY,
			CANCEL_SELECT,
			ROTATE_FORWARD,
			ROTATE_BACKWARD,
			TOGGLE_ROTATE,
			TOUCH_BEGAN,
			TOUCH_MOVED,
			TOUCH_ENDED,
			TOUCH_CANCELLED,
			COUNT
		};

		enum STATE_TOUCH
		{
			STATE_NONE = 0,
			STATE_PRESS,
			STATE_HOLD,
			STATE_RELEASE,
			STATE_COUNT
		};

		class TouchPoint : public Ref
		{
		public:
			TouchPoint(cocos2d::Point p) : pos(p){}
			TouchPoint() : pos(0, 0){}
			~TouchPoint(){}
			cocos2d::Point pos;
		};
		class InputManager : public Layer
		{
		private:
			static InputManager * m_Instace;
			CONTROL_TYPE m_currentControl;
			vector<EventKeyboard::KeyCode> heldKeys;

			bool HaveEventInput;
			Map<int, TouchPoint*> m_TouchList;
			Map<int, TouchPoint*> m_TouchReleaseList;
			std::map<CONTROL_TYPE, bool> m_StatusList;

			CONTROL_TYPE convertKeyToControl(EventKeyboard::KeyCode k)
			{
				if (k == EventKeyboard::KeyCode::KEY_ESCAPE)
					return CONTROL_TYPE::BACK_KEY;
				else if (k == EventKeyboard::KeyCode::KEY_SPACE)
					return CONTROL_TYPE::JUMP;
				else if (k == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
					return CONTROL_TYPE::MOVE_FORWARD;
				else if (k == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
					return CONTROL_TYPE::MOVE_BACKWARD;
				else if (k == EventKeyboard::KeyCode::KEY_DELETE)
					return CONTROL_TYPE::CANCEL_SELECT;
				else if (k == EventKeyboard::KeyCode::KEY_UP_ARROW)
					return CONTROL_TYPE::ROTATE_FORWARD;
				else if (k == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
					return CONTROL_TYPE::ROTATE_BACKWARD;
				else if (k == EventKeyboard::KeyCode::KEY_R)
					return CONTROL_TYPE::TOGGLE_ROTATE;
				return CONTROL_TYPE::NONE;
			}


			STATE_TOUCH p_isTouchOnScreen;
		public:
			static Scene* createScene();
			static InputManager * GetInstance()
			{
				if (!m_Instace)
				{
					m_Instace = new InputManager();
				}
				return m_Instace;
			}
			CONTROL_TYPE GetCurrentControlType()
			{
				return m_currentControl;
			}
			cocos2d::Point GetPosAtId(int idx)
			{
				if (idx >= m_TouchList.size())
					return cocos2d::Point(0, 0);
				else
					return m_TouchList.at(idx)->pos;
			}
			cocos2d::Point GetPosAtIdRelease(int idx)
			{
				if (idx >= m_TouchReleaseList.size())
					return cocos2d::Point(0, 0);
				else
					return m_TouchReleaseList.at(idx)->pos;
			}
			int GetNumberTouch()
			{
				return (int)m_TouchList.size();
			}

			bool IsPressOnBound(cocos2d::Rect re)
			{
				if (re.containsPoint(m_TouchList.at(0)->pos))
					return true;
				return false;
			}
			bool IsReleaseOnbutton(cocos2d::Rect re)
			{
				if (re.containsPoint(m_TouchReleaseList.at(0)->pos))
					return true;
				return false;
			}
			virtual void update(float dt);
			virtual bool init();

			void onKeyHold(float interval);
			void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
			void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

			virtual void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event);
			virtual void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event);
			virtual void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event);
			virtual void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);

			void OnClearTouchReleaseList() { m_TouchReleaseList.clear(); }

			bool IsTouchOnScreen() { return p_isTouchOnScreen == STATE_TOUCH::STATE_PRESS; }
			STATE_TOUCH GetStateTouchOnScreen() { return p_isTouchOnScreen ; }

			void EndTouchOnScreen() { 
				p_isTouchOnScreen = STATE_TOUCH::STATE_NONE;
			}

			bool IsThisButtonPress(CONTROL_TYPE c)
			{
				return false;
				//return InputManager::GetInstance()->m_StatusList.find(c)->second;
			}

			CREATE_FUNC(InputManager);

			InputManager();
			virtual ~InputManager();
		};
	}
	#define InputMgr Control::InputManager::GetInstance()
}

#endif//_INPUT_MANAGER_H_