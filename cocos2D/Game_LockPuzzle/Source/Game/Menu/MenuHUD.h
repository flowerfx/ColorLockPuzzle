#ifndef __MENU_HUD_H__
#define __MENU_HUD_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuHUD : public MenuEntityWrapper
{
private:
	bool p_IsExitAP;

	WidgetEntity * p_current_hint_text;

	WidgetEntity * p_current_level_text;

	LayerEntity * p_layer_icon_diagonally;

	WidgetEntity * p_layer_target;

	bool p_have_move_diagonally;

protected:
	void SetNewTarget(int current_level, int & number_node_level);
public:
	MenuHUD();
	~MenuHUD();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	virtual void OnBeginFadeIn();
	virtual void OnFadeInComplete();

	bool IsExitAP() { return p_IsExitAP; }

	void MarkAsExitAP() { p_IsExitAP = true;  }

	void OnSetTargetPanel(int level, bool next_step = false);

	void ProcessNextLevel();

	void OnMakeTitleLevelZoom();
	void OnMakeBtnPauseZoom(CallFunc * func);

	void OnShowVideoAdsBtn();
	void OnShowDiagonalIcon(bool value);

	void SetNumberMoveDia(int number);

	Vec2 GetPosNodeTargetAtIdx(int idx, int size);

	Vec2 GetWorldPosHUDBtnMenu();
	//callbackfunction
	static void CallBackFunction(void * p_Object, const char * str);
	virtual void OnProcess(const char * str);
    
};

#endif //__MENU_HUD_H__

