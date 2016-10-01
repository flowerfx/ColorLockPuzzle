#include "MenuIAP.h"

#include "FileManager.h"
#include "FXFlatform/XFlatform.h"
#include "Social/SocialManager.h"
#include "SoundManager.h"

#include "UI/AdvanceUI/wText.h"
MenuIAP::MenuIAP()
{
	p_menu_come_from = MENU_LAYER::MENU_NONE;
    p_list_iap_item = nullptr;
    
    iap_key_in_process = "";
    p_loadingIAPListComplete = false;
}

MenuIAP::~MenuIAP()
{
    p_list_iap_item = nullptr;
}

int MenuIAP::Init()
{
	RKString _menu = "iap";
	float cur_ratio = GetGameSize().width / GetGameSize().height;
	if (cur_ratio > 0.68f) //ipad ratio
	{
		_menu = "iap_ipad";
	}


	InitMenuWidgetEntity(_menu);

    p_list_iap_item = GetWidgetChildByName("layer_iap.layout_iap.scroll_iap");

    p_fadeInComplete = false;
    
	return 1;
}

void MenuIAP::UpdateMenu(float dt)
{
	onUpdateMenuWidget(dt);
    
    if (!p_fadeInComplete)
        return;
    
    bool res = IAPMgr->OnGetInventoryStore();
    
    if(res)
    {
        auto state_iap_load_item = IAPMgr->StateGetAllItemProcess();
        if(state_iap_load_item != IAP::IAP_PROCESSING && state_iap_load_item != IAP::IAP_NONE)
        {
            if(state_iap_load_item == IAP::IAP_SUCCEED)
            {
                MenuMgr->OnHidePopUp();
                
                ParseFormatPriceCurrency();
                
                p_loadingIAPListComplete = true;
            }
            else //fail
            {
                MenuMgr->OnShowPopUp("", "IAP LOAD FAIL...", 1, "load_iap_fail", (void*)this, MenuIAP::CallBackFunction);
            }
            
            IAPMgr->ClearStateLoadItemProcess();
        }
        else if (state_iap_load_item == IAP::IAP_NONE)
        {
            auto state_purchase_item = IAPMgr->StateGetPurchaseItemProcess();
            if(state_purchase_item != IAP::IAP_PROCESSING && state_purchase_item != IAP::IAP_NONE)
            {
                if(state_purchase_item == IAP::IAP_SUCCEED)
                {
                    MenuMgr->OnHidePopUp();
                    OnProcess(iap_key_in_process.GetString());
                }
                else //fail
                {
                    MenuMgr->OnShowPopUp("", "TRANSACTION FAILED ! \nTRY AGAIN LATER", 1);
                }
                IAPMgr->ClearStatePurchaseItemProcess();
            }
        }
    }
}

void MenuIAP::VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onVisitMenuWidget(renderer, transform, flags);
}

void MenuIAP::DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onDrawMenuWidget(renderer, transform, flags);
}

void MenuIAP::OnProcessWidgetChild(RKString name, UI_TYPE type_widget)
{
	if (name == "back_btn")
	{
		MenuMgr->SwitchToMenu(p_menu_come_from, IAP_MENU);
	}
	else if (name == "pack_1_btn" || name == "pack_2_btn" || name == "pack_3_btn")
	{
		MenuMgr->OnShowPopUp("", "Are you sure to want to \npurchase this pack ?", 2, name.GetString(), (void*)this, MenuIAP::CallBackFunction);
	}
	else if (name == "remove_ads_btn")
	{
		bool IsRemoveAds = File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_REMOVE_ADS) == "1";
		if (!IsRemoveAds)
		{
			MenuMgr->OnShowPopUp("", "Are you sure to want to \nRemove Ads ? \nIf you have removed ads before \nrestore purchase in setting menu!", 2, name.GetString(), (void*)this, MenuIAP::CallBackFunction);
		}
	}
}

void MenuIAP::OnDeactiveCurrentMenu()
{
	MenuMgr->CloseCurrentMenu(IAP_MENU);

    IAPMgr->ClearStateLoadItemProcess();
    IAPMgr->ClearStatePurchaseItemProcess();
    p_fadeInComplete = false;

    if (!MenuMgr->IsOnActionPhase())
	{
		MenuMgr->GetMenuUI(MAIN_MENU)->SetLayerInteractive(true);
	}
}

void MenuIAP::OnBeginFadeIn()
{
	int remain_hint = (int)File::SaveMgr->GetDataSave()->GetDataByName(SAVE_HINT_REMAIN);

	static_cast<TextWidget*>(GetWidgetChildByName("layer_iap.layout_iap.remain_hint_title"))->SetText("you have " + std::to_string(remain_hint) + " hints ");
    
    GetDataFromIAPMgr();
    
    if(IAPMgr->IsRemoveAds())
    {
        (p_list_iap_item->GetWidgetChildByName(".remove_ads_btn"))->Visible(false);
    }
}

void MenuIAP::OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status)
{
	OnAllProcessFinishFadeIdx(idx, p_max_idx_process_first, status);
}

void MenuIAP::CallBackFunction(void * p_Object, const char * str)
{
	MenuIAP * self = (MenuIAP*)p_Object;
	self->OnProcess(str);
}

void MenuIAP::OnAddHint(RKString name_hint)
{
	int remain_hint = (int)File::SaveMgr->GetDataSave()->GetDataByName(SAVE_HINT_REMAIN);

	remain_hint += IAPMgr->GetItem(name_hint)->p_GainValue;
	iap_key_in_process = "";

	File::SaveMgr->GetDataSave()->SetDataAtName(SAVE_HINT_REMAIN, remain_hint);
	File::SaveMgr->SaveTheGameSave();

	static_cast<TextWidget*>(GetWidgetChildByName("layer_iap.layout_iap.remain_hint_title"))->SetText("you have " + std::to_string(remain_hint) + " hints ");
}

void MenuIAP::OnProcess(const char * str)
{
    if (strcmp(str,"checking_full_fill_ment") == 0 || strcmp(str,"load_iap_fail") == 0)
    {
        MenuMgr->SwitchToMenu(p_menu_come_from, IAP_MENU);
        return;
    }
    if ((strcmp(str, "pack_1_btn") == 0 || strcmp(str, "pack_2_btn") == 0 || strcmp(str, "pack_3_btn") == 0 || strcmp(str, "remove_ads_btn") == 0)
        && IAPMgr->StateGetPurchaseItemProcess() == IAP::IAP_NONE)
	{
        if(strcmp(str, "pack_1_btn") == 0)
            iap_key_in_process = "10_hints_pack";
        if(strcmp(str, "pack_2_btn") == 0)
            iap_key_in_process = "40_hints_pack";
        if(strcmp(str, "pack_3_btn") == 0)
            iap_key_in_process = "120_hints_pack";
        if(strcmp(str, "remove_ads_btn") == 0)
            iap_key_in_process = "remove_ads_pack";
        
        //MenuMgr->OnHidePopUp();
        MenuMgr->OnShowPopUp("", "ON PROCESSING ITEM .. !", 0);
        IAPMgr->OnPurchaseItem(iap_key_in_process);
        return;
    }
    else if((strcmp(str, "10_hints_pack") == 0 || strcmp(str, "40_hints_pack") == 0 || strcmp(str, "120_hints_pack") == 0 || strcmp(str, "remove_ads_pack") == 0)
            && IAPMgr->StateGetPurchaseItemProcess() == IAP::IAP_SUCCEED)
    {
        
#if USE_CC_AUDIO
        GetSound->PlaySoundEffect("SFX_BUY_COIN", 0.1f, 1);
#endif
        IAPMgr->ClearStatePurchaseItemProcess();
        
        
        if (strcmp(str, "remove_ads_pack") == 0)
        {
            MenuMgr->SetDataFromSave(SAVE_STR_REMOVE_ADS, 0,"1");
            File::SaveMgr->SaveTheGameSave();
            IAPMgr->SetRemoveAds(true);
            return;
        }
        
		OnAddHint(iap_key_in_process);
		iap_key_in_process = "";

	}
}

void MenuIAP::GetDataFromIAPMgr()
{
    auto data__ = IAPMgr->GetItem("10_hints_pack");
    
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_1_btn.pack_1_title")))->SetText(data__->p_DisplayName);
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_1_btn.pack_1_cost")))->SetText(data__->p_cost);
    
    
    data__ = IAPMgr->GetItem("40_hints_pack");
    
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_2_btn.pack_2_title")))->SetText(data__->p_DisplayName);
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_2_btn.pack_2_cost")))->SetText(data__->p_cost);
    
    data__ = IAPMgr->GetItem("120_hints_pack");
    
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_3_btn.pack_3_title")))->SetText(data__->p_DisplayName);
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_3_btn.pack_3_cost")))->SetText(data__->p_cost);
    
    
    data__ = IAPMgr->GetItem("remove_ads_pack");
    
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".remove_ads_btn.remove_ads_title")))->SetText(data__->p_DisplayName);
    (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".remove_ads_btn.remove_ads_cost")))->SetText(data__->p_cost);
}

void MenuIAP::ParseFormatPriceCurrency()
{
    auto data__ = IAPMgr->GetItem("10_hints_pack");
    RKString id_item = data__->p_key;
    RKString _price_format = IAPMgr->GetPriceFormatWithID(id_item);
    if(_price_format != "")
        (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_1_btn.pack_1_cost")))->SetText(_price_format);
    
    data__ = IAPMgr->GetItem("40_hints_pack");
    id_item = data__->p_key;
    _price_format = IAPMgr->GetPriceFormatWithID(id_item);
    if(_price_format != "")
        (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_2_btn.pack_2_cost")))->SetText(_price_format);
    
    data__ = IAPMgr->GetItem("120_hints_pack");
    id_item = data__->p_key;
    _price_format = IAPMgr->GetPriceFormatWithID(id_item);
    if(_price_format != "")
        (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".pack_3_btn.pack_3_cost")))->SetText(_price_format);
    
    data__ = IAPMgr->GetItem("remove_ads_pack");
    id_item = data__->p_key;
    _price_format = IAPMgr->GetPriceFormatWithID(id_item);
    if(_price_format != "")
        (static_cast<TextWidget*>(p_list_iap_item->GetWidgetChildByName(".remove_ads_btn.remove_ads_cost")))->SetText(_price_format);
}

void MenuIAP::OnFadeInComplete()
{
    bool res = IAPMgr->OnGetInventoryStore();
    
    if (res) //check fulfillment aka restore purchase in ios complete
    {
        //
        if (!p_loadingIAPListComplete)
        {
            MenuMgr->OnShowPopUp("", "ON LOADING ITEM IAP...", 0, "", NULL);
        }
    }
    else
    {
        MenuMgr->OnShowPopUp("", "ON CHECK RESTORE ITEM...", 1, "checking_full_fill_ment", (void*)this, MenuIAP::CallBackFunction);
    }
    p_fadeInComplete = true;
}