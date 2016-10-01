#ifndef __SCENE_3D_H__
#define __SCENE_3D_H__

#include "../DisplayView.h"
#include "../LoadDataXML.h"
#include "cocos2d.h"
#include "RKString_Code/RKString.h"
USING_NS_CC;
namespace Utility
{
	namespace Scene
	{
		class Scene3D : public cocos2d::Layer
		{
		private:
			cocos2d::Skybox*	_skyBox;
			cocos2d::Terrain*	_terrain;

			cocos2d::Camera *	_camera;
		protected:
			void InitSkyBox(xml::Scene3D * scene_xml);
			void InitTerrain(xml::Scene3D * scene_xml);
		public:
			Scene3D();
			virtual ~Scene3D();


			virtual void Init(xml::Scene3D * scene_xml);
			virtual void DrawScene(Renderer *renderer, const Mat4& transform, uint32_t flags);
			virtual void VisitScene(Renderer *renderer, const Mat4& transform, uint32_t flags);
			virtual void UpdateScene(float dt);

			void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
		};
		
	}
}
#endif //__SCENE_3D_H__

