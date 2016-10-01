#ifndef __MENU_SETTING_H__
#define __MENU_SETTING_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuSetting : public MenuEntityWrapper
{
private:
    WidgetEntity * p_option_scroll;

public:
	MenuSetting();
	~MenuSetting();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);
    
    void SetInfoGameLogin(RKString name_login, int type);

    void OnSetProfileFB();
};

#endif //__MENU_SETTING_H__

