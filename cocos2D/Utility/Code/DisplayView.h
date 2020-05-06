#ifndef _DISPLAY_VIEW_H_
#define _DISPLAY_VIEW_H_
#include "RKString_Code/RKString.h"
#include "cocos2d.h"
#include "RKBaseLog/debug.h"
#include "RKBaseLog/log.h"
#define USE_ANTIALIAS 0

#define USE_BLOOM_EFFECT 1
//simulate dx cor on opengl, no-need on directX, fix for design from photoshop
#define USE_DX_COR 1

#ifdef USE_ARK
#define USE_ARK_ARCHIVE 1
#endif
extern "C"
{
#ifdef OS_IOS
    __attribute__((dllimport)) bool g_IsUseArk ;
#else
	__declspec(dllimport) bool g_IsUseArk ;
#endif

}

#define USE_CC_AUDIO 1

#ifdef USE_CC_AUDIO
#define USE_AUDIO_ON_THREAD
#endif

#define USE_DYNAMIC_BG 1

#define USE_PARTICLE 1

#define USE_IAP 1

//#define USE_TEXT

#define USE_THREAD_BG


#ifdef USE_TEXT
#define LABEL Text
#else
#define LABEL Label
#endif

#define USE_SPRITE_FRAME 1

#ifdef OS_IOS
//#include "Init/../../iOS/IOSCode/DisplayView_IOS.h"
typedef unsigned int uint32;
#endif
using namespace cocos2d;


namespace Utility
{

	int GetRandom(int num1, int num2, int except_num = 0xfffff);
	Vec2 ConvertPos(Vec2 ps);
	Vec2 ConvertPos1(Vec2 ps);
	Vec2 GravityVector();
	Vec2 ConvertPTSCorToCCCor(Vec2 P);
	Vec2 GetViewPoint(Vec2 TopLeft, Vec2 CenterPos);

	void InitSizeGame();
	void InitValue();
	void LoadData();

	void ChangeToNewSize(int width, int height);

	Vec2 GetScreenCenter();
	Vec2 GetTextureCenterPos(MenuItemImage * m);

    cocos2d::Size  GetGameSize();
	Vec2  GetGameDesignSize();
	//
	float GetScaleFactor(bool Y = false);
	float GetOffsetSCale();
	//in milisecond
	float getTimeTick();
	//backkey
	bool IsBackKeyPress();
	void SetBackkey(bool b);
	//use in the editor
	//convert string
#if defined WP8 || defined OS_W8
	std::string CCUnicodeToUtf8(const wchar_t* pwszStr);
	std::wstring CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len/* = -1*/);
#endif
	//get platform
	std::string GetPlatForm();
	//get mempeak
	bool IsDeviceLowMem();
	unsigned int GetCpuCoreCount();
	//sync with fps
	float SyncWithFPS(float d);
	//convert to display time
	std::string convertToDisplayTime(int timePlay);

	//math
	float GetMulMul(float mNum, int num);
	float GetMulDiv(float mNum, int num);
}
//---------------------------------------------------------------------
//define path
//---------------------------------------------------------------------
#define TEXTURE_PATH	"Texture/"
#define XML_DIR			"xml/"
//---------------------------------------------------------------------
//define xml data
//---------------------------------------------------------------------
#define	TEXTURE_XML			"TextureObject.xml"
#define	UI_XML				"MenuUI.xml"
#define BG_DEC_XML			"BGObject.xml"
#define OBJECT_DEC_XML		"GamePlayObject.xml"
#define MATERIAL_DEC_XML	"MaterialDec.xml"
#define SOUND_DEC_XML		"SoundDec.xml"
#define EFFECT_DEC_XML		"Effect.xml"

#define ITEM_XML			"ItemInGame.xml"
#define	UI_WIDGET_XML		"MenuUIWidget.xml"
#define	W_OBJECT_DEC_XML	"GamePlayObject_widget.xml"
#define SCENE_3D_DEC_XML	"Scene3DDec.xml"
#define TEXTURE_PACKER_XML	"texturePacker.xml"
#endif //_DISPLAY_VIEW_H_