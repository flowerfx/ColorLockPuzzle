#ifndef __MENU_POPUP_H__
#define __MENU_POPUP_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuPopUp : public MenuEntityWrapper
{
private:
	RKString p_nameBtnProcess;
	int p_NumberBtn;
	//for the call back
	RKString p_strCallback;
	void* p_ObjectCallBack;
	void(*p_Function)(void * ptr, const char * str);
public:
	MenuPopUp();
	~MenuPopUp();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	//void functionCallBack
	void callFunction(void(*pt2Function)(void* pt2Object, const char * str_data));
	//call popup method
	void OnShowPopUp(RKString title = "", RKString str = "", int numberBtn = 2, const char * strCB = "", void * objectCallBack = NULL, void(*pt2Function)(void* pt2Object, const char * str) = NULL);
    
};

#endif //__MENU_POPUP_H__

