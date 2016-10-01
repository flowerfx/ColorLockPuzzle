#ifndef __MENU_WIDGET_H__
#define __MENU_WIDGET_H__

#pragma once

#include "LayerEntity.h"

#include "cocos2d.h"
using namespace RKUtils;
namespace Utility
{
	namespace UI_Widget
	{
		class MenuWidget;
		//
		struct UIDecs
		{
			STATUS_MENU p_curStatus;
			bool		p_IsActive;
			RKString	p_SoundFadeIn;
			RKString	p_SoundFadeOut;
			float		p_delayTime;
			int			p_loop;
		public:
			UIDecs()
			{
				p_curStatus = STATUS_MENU::M_IDLE;
				p_IsActive = false;
				p_SoundFadeIn = "";
				p_SoundFadeOut = "";

				p_delayTime = 0;
				p_loop = 0;

			}

			UIDecs(RKString sou_1, RKString sou_2, bool IsActive)
			{
				p_curStatus = STATUS_MENU::M_IDLE;
				p_IsActive = IsActive;
				p_SoundFadeIn = sou_1;
				p_SoundFadeOut = sou_2;

				p_delayTime = 0;
				p_loop = 0;
			}

			UIDecs(RKString sou_1, RKString sou_2, bool IsActive, float delayTime, int loop)
			{
				p_curStatus = STATUS_MENU::M_IDLE;
				p_IsActive = IsActive;
				p_SoundFadeIn = sou_1;
				p_SoundFadeOut = sou_2;

				p_delayTime = delayTime;
				p_loop = loop;
			}
		};

		class MenuWidget : public Ref
		{
		protected:

			std::vector<LayerEntity*>		p_listLayerWidget;

			std::vector<UIDecs *>		p_ListStatus;

			float p_DeltaMoveTime;

			int p_IdxMenuData;

			int p_IdxUITagAppear;

			LayerEntity * InitTheChildElement(RKString str, LayerEntity * parent_layer, xml::UILayerWidgetDec* re);
			void InitMenuWidgetEntity(RKString str);

			float p_time_process;

			int p_max_idx_process_first;

			RKString name_ui;

			bool p_IsCanPressButton;

			bool p_IsMenuInteractive;

			Vec2 p_design_size;

		protected:

			void onDrawMenuWidget(Renderer *renderer, const Mat4& transform, uint32_t flags);
			void onVisitMenuWidget(Renderer *renderer, const Mat4& transform, uint32_t flags);
			void onUpdateMenuWidget(float dt);
			//
			void RemoveActionFadeAllChild(int idx);
			//
			virtual void OnProcessWhenEndOrBegin(STATUS_MENU cur_status);
			virtual void OnProcessWhenTagIdxMulti(int max_idx, int idx, STATUS_MENU cur_status, STATUS_MENU status);
		public:
			RKString GetNameUI() { return name_ui; }
			void SetIdxMenuData(int idx) { p_IdxMenuData = idx; }

			MenuWidget();
			virtual ~MenuWidget();
			//
			virtual int	Init() = 0;
			virtual void	UpdateMenu(float dt) = 0;
			virtual void	VisitMenu(Renderer *renderer, const Mat4& transform, uint32_t flags) = 0;
			virtual void	DrawMenu(Renderer *renderer, const Mat4& transform, uint32_t flags) = 0;
			//
			virtual void OnFadeIn() ;
			virtual void OnFadeOut() ;
			//
			virtual void OnShow() ;
			virtual void OnHide() ;
			//
			virtual void OnProcessWidgetChild(RKString name , UI_TYPE type_widget) = 0;
			void OnCheckTouchEvent(RKString name, UI_TYPE type_widget);

			virtual void OnDeactiveCurrentMenu() = 0 ;

			virtual void OnFinishFadeAtCurrentIdx(int idx, STATUS_MENU status);

			STATUS_MENU GetCurrentStatus(int idx = 0);
			void SetStatusAtIdx(STATUS_MENU status, int idx);
			//
			bool GetActiveStatusAtidx(int idx);
			void SetActiveStatusAtIdx(bool active, int idx);
			//
			virtual void OnReloadString();
			
			//
			bool OnSetActionFadeAllResource(STATUS_MENU stateFade, unsigned int idx_tag_appear = 0);
			//
			// return the fade process finish or not
			bool OnUpdateFadeProcessChildByTagIdx(unsigned int idx_tag_appear, float process_time, float dt, STATUS_MENU stateFade);

			virtual WidgetEntity * GetWidgetChildByName(RKString name_path);
			virtual LayerEntity	 * GetLayerChildByName(RKString name_str);

			//
			virtual bool OnCallProcessOfChild(RKString name_child_path, RKString command_str, int type = 0);

			//use this to call any idx target to appear or disappear except at 0
			virtual void OnShowOrHideIdxTarget(int idx_target);

			//if UI have more 1 idx target call this function
			virtual void OnAllProcessFinishFadeIdx(int idx, int max_idx, STATUS_MENU status);

			//happen when begin fade in 
			virtual void OnBeginFadeIn();

			//happen when fade in complete
			virtual void OnFadeInComplete();

			//happen when begin fade out 
			virtual void OnBeginFadeOut();

			//happen when fade out complete
			virtual void OnFadeOutComplete();

			//happend when have touch on the menu
			virtual void OnUpdateTouchMenu();
			virtual void OnTouchMenuBegin(cocos2d::Point p);
			virtual void OnTouchMenuEnd(cocos2d::Point p);

			virtual void onSpecificSizeChange(float cur_width, float cur_height);
			void ResetSizeAndPosMenuEntity(float cur_width, float cur_height);

			int GetMaxIdxFirst() { return p_max_idx_process_first; }

			//method with cocos2dx
			void push_layer_to_main_scene();
			void remove_layer_from_main_scene();

			//
			void SetLayerInteractive(bool value);

			void SetChildInteractive(RKString name_path, bool value);

			bool IsInteractive() { return p_IsMenuInteractive; }

			Vec2 GetDesignSize() { return p_design_size; }
		};

	}
}

#endif //__MENU_WIDGET_H__

