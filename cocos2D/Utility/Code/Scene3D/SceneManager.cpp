#include "SceneManager.h"
#include "Scene3D.h"
namespace Utility
{
	namespace Scene
	{
		SceneManager::SceneManager()
		{
			p_current_scene = nullptr;
		}

		SceneManager::~SceneManager()
		{
			p_current_scene->release();
			p_current_scene = nullptr;
		}

		void SceneManager::CreateScene(RKString name)
		{
			return;
			if (p_current_scene)
			{
				ReleaseScene();
			}

			xml::Scene3D * _scene_dec_xml = XMLMgr->GetScene3DXMLByName(name);
			p_current_scene = new Scene3D();
			p_current_scene->Init(_scene_dec_xml);
			p_current_scene->setZOrder(-100);
			PushSceneToMainScene();
		}

		void SceneManager::ReleaseScene()
		{
			if (p_current_scene)
			{
				p_current_scene->autorelease();
				p_current_scene = nullptr;
				RemoveSceneFromMainScene();
			}
		}

		void SceneManager::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			return;
			if (p_current_scene)
			{
				p_current_scene->DrawScene(renderer, transform, flags);
			}
		}

		void SceneManager::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			return;
			if (p_current_scene)
			{
				p_current_scene->VisitScene(renderer, transform, flags);
			}
		}

		void SceneManager::Update(float dt)
		{
			return;
			if (p_current_scene)
			{
				p_current_scene->UpdateScene(dt);
			}
		}

		void SceneManager::PushSceneToMainScene()
		{
			cocos2d::Scene * scene = Director::getInstance()->getRunningScene();
			scene->addChild(p_current_scene);
		}

		void SceneManager::RemoveSceneFromMainScene()
		{
			cocos2d::Scene * scene = Director::getInstance()->getRunningScene();
			scene->removeChild(p_current_scene);
		}
	}
}