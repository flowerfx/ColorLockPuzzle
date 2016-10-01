#include "ControllableUnit.h"
#include "DisplayView.h"
#include "SoundManager.h"
ControllableUnit::ControllableUnit()
{
	p_IsOnControl = false;

	index_on_table = Vec2(0, 0);

	p_number_step_to_lock = -1;
	p_current_step_lock_remain = -1;

	p_IsLock = false;

	p_IsActive = true;
    
    p_object_hl = nullptr;
    p_object_sl = nullptr;

	p_text_note = nullptr;

}

ControllableUnit::~ControllableUnit()
{

	p_number_step_to_lock = -1;
	p_current_step_lock_remain = p_number_step_to_lock;
    
    p_object_hl = nullptr;
    p_object_sl = nullptr;
	p_text_note = nullptr;
}


void ControllableUnit::Init()
{
	//new this first

	bool res = OnInitWideObject("object_node_bg");
	//
    
	CCASSERT(res, "init object failed !");
    
    p_object_sl = GetObjectByName(".object_node_select");
    p_object_hl = GetObjectByName(".hight_light_obj");
    
	p_text_note = GetObjectByName(".develope_text");
}



void ControllableUnit::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (!p_IsActive)
		return;

	wBasicObject::Draw(renderer, transform, flags);
}

void ControllableUnit::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (!p_IsActive)
		return;

	wBasicObject::Visit(renderer, transform, flags);
}

void ControllableUnit::Update(float dt)
{
	if (!p_IsActive)
		return;

	BasicUnit::Update(dt);
}

void ControllableUnit::SetIdxOnTable(int i, int j)
{
	index_on_table.x = i;
	index_on_table.y = j;
}

Vec2 ControllableUnit::GetIdxOnTable()
{
	return index_on_table;
}

void ControllableUnit::ClearStepLockStatus()
{
	p_number_step_to_lock = -1;
	p_current_step_lock_remain = -1;
}

void ControllableUnit::SetstepLockRemain( bool reset)
{
	if (p_current_step_lock_remain < 0)
		return;

	p_IsLock = false;
	if (reset)
	{
		p_current_step_lock_remain = p_number_step_to_lock;
		GetObjectByName(".lock_off")->SetVisible(true);
		GetObjectByName(".lock_on")->SetVisible(false);
	}
	else
	{
		p_current_step_lock_remain--;
		
		GetObjectByName(".lock_off")->SetVisible(true);
		GetObjectByName(".lock_on")->SetVisible(false);
		//
		if (p_current_step_lock_remain <= 0)
		{
			GetObjectByName(".lock_on")->SetVisible(true);
			GetObjectByName(".lock_off")->SetVisible(false);
			p_current_step_lock_remain = 0;
			p_IsLock = true;
		}
		else if (p_current_step_lock_remain >= p_number_step_to_lock)
		{
			p_current_step_lock_remain = p_number_step_to_lock;
		}
	}
	p_text_note->SetText(std::to_string(p_current_step_lock_remain));
}


void ControllableUnit::SetPosition(Vec2 pos)
{
	wBasicObject::SetPosition(pos);
}

void ControllableUnit::SetColor(Color4B color)
{
    if(p_object_sl != nullptr)
    {
        p_object_sl->SetColor(color);
        p_object_hl->SetColor(color);
    }
    else
    {
        wBasicObject::SetColor(color);
    }
}

void ControllableUnit::SetColorBG(Color4B color)
{
	wBasicObject::SetColor(color);
}

bool ControllableUnit::OnFinishAction()
{
	if (!p_IsOnControl)
	{
		p_object_sl->SetVisible(false);
		p_object_hl->SetVisible(false);
	}

	return true;
}

void ControllableUnit::SetOnControl(bool val, float delay_time) {
	p_IsOnControl = val; 
	//
	if (p_object_sl)
	{
		p_object_sl->SetVisible(true);
		if (p_IsOnControl)
		{
			p_object_sl->SetActionCommandWithName("ZOOM_ACTION_2");			
		}
		else
		{
			p_object_sl->SetActionCommandWithName("ZOOM_ACTION_1");
		}
	}
    
	if (p_object_hl)
	{
		p_object_hl->SetVisible(true);
		if (p_IsOnControl)
		{
			p_object_hl->SetActionCommandWithName("ZOOM_ACTION");
		}
		else
		{
			p_object_hl->SetActionCommandWithName("ZOOM_ACTION_1");
		}
	}
}
bool ControllableUnit::IsOnControl() { 
	return p_IsOnControl; 
	//
}

bool ControllableUnit::IsActive()
{
	return p_IsActive; 
}
void ControllableUnit::SetActive(bool act)
{
	p_IsActive = act; 
}

void ControllableUnit::SetTextNode(RKString text)
{
	p_text_note->SetVisible(true);
	p_text_note->SetText(text);
}

void ControllableUnit::InitStepLock(int number_lock){
	p_number_step_to_lock = number_lock;
	p_current_step_lock_remain = p_number_step_to_lock;
	p_text_note->SetVisible(true);
	GetObjectByName(".lock_off")->SetVisible(true);
	GetObjectByName(".lock_on")->SetVisible(false);
	p_text_note->SetText(std::to_string(number_lock));
}