#ifndef __MENU_IAP_H__
#define __MENU_IAP_H__


#include "MenuManager.h"

using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;

class MenuIAP : public MenuEntityWrapper
{
private:
    WidgetEntity * p_list_iap_item;
    
    void GetDataFromIAPMgr();
    RKString iap_key_in_process;
    
    bool p_fadeInComplete;
    
    bool p_loadingIAPListComplete;
    
    void ParseFormatPriceCurrency();

	void OnAddHint(RKString name_hint);
public:
	MenuIAP();
	~MenuIAP();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnBeginFadeIn();
    
    virtual void OnFadeInComplete();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);
    
	//callbackfunction
	static void CallBackFunction(void * p_Object, const char * str);

	virtual void OnProcess(const char * str);
};

#endif //__MENU_IAP_H__

