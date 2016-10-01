#ifndef __MENU_MAIN_H__
#define __MENU_MAIN_H__


#include "MenuManager.h"

using namespace Utility;
using namespace Utility::UI_Widget;

class MenuMain : public MenuEntityWrapper
{
private:
    
    ClippingNode * p_avatar_fb;
    
    int percent_ratio_appear_rating;
    
    bool p_isInitAds;

public:
	MenuMain();
	~MenuMain();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKUtils::RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnBeginFadeIn();
	virtual void OnFadeInComplete();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	void SetRankText(int idx_rank , int idx_score);
    
    void OnSetProfileFB();
    
    //callbackfunction
    static void CallBackFunction(void * p_Object, const char * str);
    virtual void OnProcess(const char * str);
};

#endif //__MENU_MAIN_H__

