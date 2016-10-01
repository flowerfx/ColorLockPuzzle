#ifndef __CONTROLLABLE_UNIT_H__
#define __CONTROLLABLE_UNIT_H__

#include "UI/Entity/TextEntity.h"
#include "BasicUnit.h"
#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace Utility;
using namespace Utility::wObject;

class ControllableUnit : public BasicUnit
{
private:
	Vec2 index_on_table;
	//

	////for the lock
	int p_number_step_to_lock;
	int p_current_step_lock_remain;

	//the object is on lock or not
	bool p_IsLock;

	//the object is on active or not
	bool p_IsActive;

	//the object is on select or not
	bool p_IsOnControl;
    
    wBasicObject * p_object_hl;
    wBasicObject * p_object_sl;

	wBasicObject * p_text_note;

public:
	ControllableUnit();
	~ControllableUnit();

	virtual void Init();
	virtual void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void Update(float dt);

	void SetIdxOnTable(int i, int j);
	Vec2 GetIdxOnTable();

	int GetStepLock() { return p_number_step_to_lock; }
	void InitStepLock(int number_lock);
	void ClearStepLockStatus();
	void SetstepLockRemain(bool reset = false);
	bool IsLock()  { return p_IsLock; }

	void SetOnControl(bool val , float delay_time = 0.f);
	bool IsOnControl();
	bool IsActive();
	void SetActive(bool act);

	virtual void SetPosition(Vec2 pos);
	virtual void SetColor(Color4B color);

	void SetColorBG(Color4B color);

	virtual bool OnFinishAction();

	void SetTextNode(RKString text);

	wBasicObject * GetObjectSL() {
		return p_object_sl;
	}
};

#endif //__CONTROLLABLE_UNIT_H__

