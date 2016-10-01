#ifndef __MENU_END_GAME_H__
#define __MENU_END_GAME_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuEndGame : public MenuEntityWrapper
{

public:
	MenuEndGame();
	~MenuEndGame();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFadeInComplete();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);
    
};

#endif //__MENU_END_GAME_H__

