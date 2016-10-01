#ifndef __BASIC_UNIT_H__
#define __BASIC_UNIT_H__

#include "Object/wObject.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace Utility;
using namespace Utility::wObject;

class BasicUnit : public wBasicObject
{
private:
	RKString p_type_unit;
public:
	BasicUnit();
	BasicUnit(RKString text);
	BasicUnit(RKString text, int numberhold);
	virtual ~BasicUnit();

	virtual void Update(float dt);

	RKString GetType() { return p_type_unit; }

	virtual bool OnFinishAction();
};

#endif //__BASIC_UNIT_H__

