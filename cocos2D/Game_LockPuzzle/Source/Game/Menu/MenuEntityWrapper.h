#ifndef __MENU_ENTITY_WRAPPER_H__
#define __MENU_ENTITY_WRAPPER_H__


//#include "UI/Entity/MenuEntity.h"

#include "UI/AdvanceUI/MenuWidget.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

enum MENU_LAYER
{
	MENU_NONE = 0,
	MAIN_MENU,
	SETTING_MENU,
	HUD_MENU,
	SELECTION_LEVEL_MENU,
	INGAME_MENU,
	POPUP_MENU,
	RESULT_MENU,
	//BOOST_MENU,
	IAP_MENU,
	TUTORIAL_MENU,
	MENU_COUNT
};

class MenuEntityWrapper : public MenuWidget
{
protected:

	MENU_LAYER p_menu_come_from;
    
    MENU_LAYER p_menu_show_next;
public:
	MenuEntityWrapper();
	~MenuEntityWrapper();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget) = 0;

	virtual void OnDeactiveCurrentMenu();

	virtual void OnBeginFadeIn();

	void SetMenuComFrom(MENU_LAYER menu){ p_menu_come_from = menu; }
	MENU_LAYER GetMenuComeFrom() { return p_menu_come_from; }
    
    void SetMenuShowNext(MENU_LAYER menu) {p_menu_show_next = menu; }
    MENU_LAYER GetMenuShowNext() { return p_menu_show_next; }
};

#endif //__MENU_ENTITY_WRAPPER_H__

