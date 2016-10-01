#ifndef __MENU_IN_GAME_H__
#define __MENU_IN_GAME_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuInGame : public MenuEntityWrapper
{

public:
	MenuInGame();
	~MenuInGame();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	virtual void OnBeginFadeIn();

	//callbackfunction
	static void CallBackFunction(void * p_Object, const char * str);
	virtual void OnProcess(const char * str);
    
};

#endif //__MENU_IN_GAME_H__

