#include "BasicUnit.h"

BasicUnit::BasicUnit()
{
	p_type_unit = "";
}

BasicUnit::BasicUnit(RKString text)
{
	p_type_unit = text;
}

BasicUnit::BasicUnit(RKString text, int numberhold)
{
	p_type_unit = text;
}

BasicUnit::~BasicUnit()
{
}

void BasicUnit::Update(float dt)
{
	wBasicObject::Update(dt);
}

bool BasicUnit::OnFinishAction()
{
	return true;
}