#ifndef __MENU_TUTORIAL_H__
#define __MENU_TUTORIAL_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuTutorial : public MenuEntityWrapper
{
private:

	int p_Step_tutorial;

	WidgetEntity * p_text_tutorial;
	WidgetEntity * p_hand_tutorial;
    
    void OnFinishTutorialStep();
public:
	MenuTutorial();
	~MenuTutorial();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);
	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	virtual void OnBeginFadeIn();
	virtual void OnFadeInComplete();

	void OnShowTutorialStep(int step);

	int GetCurrentStepTutorial() { return p_Step_tutorial; }
    
};

#endif //__MENU_SELECTION_H__

