#ifndef __MENU_SELECTION_H__
#define __MENU_SELECTION_H__


#include "MenuManager.h"
#include "UI/AdvanceUI/wListView.h"
using namespace RKUtils;
using namespace Utility;
using namespace Utility::UI_Widget;


class MenuSelect : public MenuEntityWrapper
{
private:
	int p_number_level;

	void LoadLevelParam(TiXmlDocument * p);

	void LoadDataLevelFromXML();

	ListViewWidget * p_page_view;
    
   // int RecorrectHint(int cur_value, int size);
public:
	MenuSelect();
	~MenuSelect();

	virtual  int	Init();

	virtual void	UpdateMenu(float dt);
	virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void OnProcessWidgetChild(RKString name, UI_TYPE type_widget);

	virtual void OnDeactiveCurrentMenu();

	virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

	virtual void OnBeginFadeIn();

	void InitLevel();

	void UnlockLevelIdx(int level);
    
};

#endif //__MENU_SELECTION_H__

