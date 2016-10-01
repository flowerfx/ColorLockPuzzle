#include "MenuEntityWrapper.h"


MenuEntityWrapper::MenuEntityWrapper()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
    p_menu_show_next= MENU_LAYER::MENU_NONE;
}

MenuEntityWrapper::~MenuEntityWrapper()
{

}

int MenuEntityWrapper::Init()
{
	return 1;
}

void MenuEntityWrapper::UpdateMenu(float dt)
{

}

void MenuEntityWrapper::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{

}

void MenuEntityWrapper::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{

}

void MenuEntityWrapper::OnDeactiveCurrentMenu()
{
	
}


void MenuEntityWrapper::OnBeginFadeIn()
{

}
