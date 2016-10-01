#include "Scene3D.h"

namespace Utility
{
	namespace Scene
	{
		Scene3D::Scene3D()
		{
			_skyBox = nullptr;
			_camera = nullptr;
		}

		Scene3D::~Scene3D()
		{
			_skyBox->release();
			_skyBox = nullptr;

			_camera->release();
			_camera = nullptr;
		}

		void Scene3D::InitSkyBox(xml::Scene3D * scene_xml)
		{
			_skyBox = Skybox::create();
			_skyBox->retain();

			cocos2d::TextureCube* _textureCube = TextureCube::create(
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("LEFT")).GetString(),
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("RIGHT")).GetString(),
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("UP")).GetString(),
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("DOWN")).GetString(),
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("FRONT")).GetString(),
				Utility::File::ResolveNamePath(scene_xml->skybox_tex->GetDataChar("BACK")).GetString());

			Texture2D::TexParams tRepeatParams;
			tRepeatParams.magFilter = GL_LINEAR;
			tRepeatParams.minFilter = GL_LINEAR;
			tRepeatParams.wrapS = GL_CLAMP_TO_EDGE;
			tRepeatParams.wrapT = GL_CLAMP_TO_EDGE;
			_textureCube->setTexParameters(tRepeatParams);

			_skyBox->setTexture(_textureCube);
			this->addChild(_skyBox);
		}

		void Scene3D::InitTerrain(xml::Scene3D * scene_xml)
		{
			Terrain::DetailMap 
				r(Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_1")).GetString()),
				g(Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_2")).GetString()),
				b(Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_3")).GetString()),
				a(Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_4")).GetString());

			Terrain::TerrainData 
				data
				(
					Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_height_map")).GetString(),
					Utility::File::ResolveNamePath(scene_xml->terrain_tex->GetDataChar("tex_alpha_map")).GetString(),
					r, g, b, a,
					cocos2d::Size(
						scene_xml->terrain_tex->GetDataVector2("chunk_size").x, 
						scene_xml->terrain_tex->GetDataVector2("chunk_size").y
						),
					scene_xml->terrain_tex->GetDatafloat("map_height"),
					scene_xml->terrain_tex->GetDatafloat("map_scale")
				);


			_terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
			_terrain->setLODDistance(6.4f, 12.8f, 19.2f);
			_terrain->setMaxDetailMapAmount(4);
			_terrain->setCameraMask(2);

			_terrain->setPositionY(_terrain->getPositionY() + 10);
			this->addChild(_terrain);
		}

		void Scene3D::Init(xml::Scene3D * scene_xml)
		{
			PASSERT2(scene_xml != nullptr, "null scene dec !");

			InitSkyBox(scene_xml);

			InitTerrain(scene_xml);

			cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
			_camera = Camera::createPerspective(45, visibleSize.width / visibleSize.height, 10, 1000);
			_camera->setPosition3D(Vec3(0.f, -10.f, -50.f));
			_camera->setCameraFlag(CameraFlag::USER1);

			auto listener = EventListenerTouchAllAtOnce::create();
			listener->onTouchesMoved = CC_CALLBACK_2(Scene3D::onTouchesMoved, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

			addChild(_camera);
			setCameraMask(2);


		}

		void Scene3D::DrawScene(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			//draw(renderer, transform, flags);
		}

		void Scene3D::VisitScene(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			//visit(renderer, transform, flags);
		}

		void Scene3D::UpdateScene(float dt)
		{
			update(dt);
		}

		void Scene3D::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
		{
			if (touches.size())
			{
				auto touch = touches[0];
				auto delta = touch->getDelta();

				static float _angle = 0.f;
				_angle -= CC_DEGREES_TO_RADIANS(delta.x);
				_camera->setPosition3D(Vec3(50.0f * sinf(_angle), 0.0f, 50.0f * cosf(_angle)));
				_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
			}
		}
	}
}