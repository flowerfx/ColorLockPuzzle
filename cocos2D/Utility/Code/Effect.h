#ifndef _EFFECT_H_
#define  _EFFECT_H_

#include "DisplayView.h"
#include "cocos2d.h"
#include "Background/BGObject.h"
USING_NS_CC;
namespace Utility
{
	namespace Effect
	{
		enum EFFECT_TYPE
		{
			EFFECT_NONE = 0,
			EFFECT_BLOOM,
			EFFECT_BLUR,
			EFFECT_OUTLINE,
			EFFECT_EDGE
		};
		enum FLATFORM
		{
			HLSL = 0,
			GLSL
		};

		void AddEffectWithMaterial(cocos2d::Node * object, EFFECT_TYPE type);

		void AddEffectWithShader(cocos2d::Node * object, EFFECT_TYPE type, FLATFORM fl);

		
	}
}
#endif//_EFFECT_H_