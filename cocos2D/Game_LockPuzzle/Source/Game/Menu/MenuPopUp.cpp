#include "MenuPopUp.h"
#include "UI/AdvanceUI/wText.h"
MenuPopUp::MenuPopUp()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;

	p_strCallback = "";
	p_ObjectCallBack = NULL;
	p_Function = NULL;
}

MenuPopUp::~MenuPopUp()
{
	p_strCallback = "";
	p_ObjectCallBack = NULL;
	p_Function = NULL;
}

int MenuPopUp::Init()
{
	RKString _menu = "popup";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "popup_ipad";
	}
	InitMenuWidgetEntity(_menu);
	return 1;
}

void MenuPopUp::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
}

void MenuPopUp::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuPopUp::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuPopUp::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	/*if (name == "back_btn")
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, IAP_MENU);
	}*/

	p_nameBtnProcess = name;
	OnHide();

}

void MenuPopUp::OnDeactiveCurrentMenu()
{
    MenuMgr->CloseCurrentMenu(POPUP_MENU);
	if (p_nameBtnProcess == "ok_btn" || (p_NumberBtn == 1 && this->p_Function != NULL))
	{
		if (this->p_Function != NULL)
		{
			callFunction(this->p_Function);
		}
	}

}

void MenuPopUp::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuPopUp::callFunction(void(*pt2Function)(void*, const char *))
{
	/* do something */
	pt2Function(p_ObjectCallBack, p_strCallback.GetString());  // make callback
	p_strCallback = "";
}

void MenuPopUp::OnShowPopUp(RKString title, RKString str, int numberBtn, const char * strCB, void * objectCallBack, void(*pt2Function)(void* pt2Object, const char * str))
{
    if(MenuMgr->GetCurrentMenuLayer() != POPUP_MENU)
    {
        OnFadeIn();
    }
	//
	((TextWidget *)GetWidgetChildByName("layer_popup.panel_PopUp.popup_message"))->SetText(str);
	((TextWidget *)GetWidgetChildByName("layer_popup.panel_PopUp.popup_title"))->SetText(title);
	if (numberBtn == 2)
	{
		GetWidgetChildByName("layer_popup.panel_PopUp.ok_btn")->Visible(true);
		GetWidgetChildByName("layer_popup.panel_PopUp.cancel_btn")->Visible(true);
	}
	else if (numberBtn == 1)
	{
		GetWidgetChildByName("layer_popup.panel_PopUp.ok_btn")->Visible(false);
		GetWidgetChildByName("layer_popup.panel_PopUp.cancel_btn")->Visible(true);
	}
	else
	{
		GetWidgetChildByName("layer_popup.panel_PopUp.ok_btn")->Visible(false);
		GetWidgetChildByName("layer_popup.panel_PopUp.cancel_btn")->Visible(false);
	}
	p_NumberBtn = numberBtn;
	p_strCallback = strCB;
	p_ObjectCallBack = objectCallBack;
	p_Function = pt2Function;
}