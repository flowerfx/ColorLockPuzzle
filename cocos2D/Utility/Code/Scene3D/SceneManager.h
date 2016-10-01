#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "../DisplayView.h"
#include "../LoadDataXML.h"
#include "cocos2d.h"
#include "RKString_Code/RKString.h"
USING_NS_CC;
namespace Utility
{
	namespace Scene
	{
		class Scene3D;
		class SceneManager 
		{
		private:
			/*
			we dont use list scene because can lead to low perf and high mem , use one scene at a time 
			and if need to load other, we reload it !
			*/

			Scene3D * p_current_scene; 

		public:
			SceneManager();
			virtual ~SceneManager();

			void CreateScene(RKString name);
			void ReleaseScene();

			 void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
			 void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags);
			 void Update(float dt);

			 void PushSceneToMainScene();
			 void RemoveSceneFromMainScene();
		};
		
	}
}
#endif //__SCENE_MANAGER_H__

