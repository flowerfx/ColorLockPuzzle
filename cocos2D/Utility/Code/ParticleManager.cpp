#include "ParicleManager.h"
#include "ObjectTexture.h"
namespace Utility
{
	namespace Particle
	{
		/********************************************************
		*
		Particle
		*
		*********************************************************/
		Particle::Particle()
		{
			p_reduceEffectBaseHardware = 1;
			if (XMLMgr->GetHarwardProfile() == xml::HARDWARE_MED)
			{
				p_reduceEffectBaseHardware = 2;
			}
			else if (XMLMgr->GetHarwardProfile() == xml::HARDWARE_LOW)
			{
				p_reduceEffectBaseHardware = 4;
			}
		}
		Particle::~Particle()
		{
			CC_SAFE_DELETE(p_Emitter);
		}

		void Particle::InitParticle(RKString name_particle)
		{
		
			auto par_xml = XMLMgr->GetEffectDecByName(name_particle);

            RKString name_path_url = File::ResolveNamePath(par_xml->name_source_plist);
			p_Emitter = ParticleSystemQuad::create(name_path_url.GetString());
			//SetTexture
			auto te = TextureMgr->GetSpriteByName(par_xml->name_source_tex);
			p_Emitter->setTextureWithRect(te->getTexture(), te->getTextureRect());

			//reset size
			p_Emitter->setStartSize(p_Emitter->getStartSize() * GetGameSize().width / 480);
			p_Emitter->setStartSizeVar(p_Emitter->getStartSizeVar()* GetGameSize().width / 480);
			p_Emitter->setEndSize(p_Emitter->getEndSize()* GetGameSize().width / 480);
			p_Emitter->setEndSizeVar(p_Emitter->getEndSizeVar()* GetGameSize().width / 480);

			p_Emitter->setBlendAdditive(true);
			p_Emitter->setEmissionRate(150.f);
			p_Emitter->retain();
		}
		/*
		draw
		*/
		void Particle::DrawParticle(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			CCASSERT(p_Emitter, "object not invalid");
			if (!IsAlive())
				return;
			p_Emitter->draw(renderer, transform, flags);
		}
		void Particle::VisitParticle(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			CCASSERT(p_Emitter, "object not invalid");
			if (!IsAlive())
				return;
			p_Emitter->visit(renderer, transform, flags);
		}
		/*
		update
		*/
		void Particle::UpdateParticle(float dt)
		{
			CCASSERT(p_Emitter, "object not invalid");
			p_Emitter->update(dt);
		}
		//method in game
		void Particle::SetPosition(cocos2d::Point pos)
		{
			CCASSERT(p_Emitter, "object not invalid");
			p_Emitter->setSourcePosition(pos);
		}

		void Particle::SetAngle(float angle)
		{
			CCASSERT(p_Emitter, "object not invalid");
			p_Emitter->setAngle(angle);
		}
		//
		bool Particle::IsAlive()
		{
			CCASSERT(p_Emitter, "object not invalid");
			return p_Emitter->isActive();
		}
		void Particle::RemoveParticle(float time)
		{
			CCASSERT(p_Emitter, "object not invalid");
			return p_Emitter->setDuration(time);
		}

		/********************************************************
		*
		ParticleManager
		*
		*********************************************************/

		ParticleManager::ParticleManager()
		{
			p_listParticle.clear();
			p_listParticleEndLife.clear();
		}
		ParticleManager::~ParticleManager()
		{
			p_listParticle.clear();
			p_listParticleEndLife.clear();
		}

		void ParticleManager::onInit()
		{
			//nothing here
		}
		/*
		draw
		*/
		void ParticleManager::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			if (p_listParticle.size() <= 0)
				return;

			for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
			{
				if (it->second->IsAlive())
				{
					(it->second)->DrawParticle(renderer, transform, flags);
				}
			}
		}

		void ParticleManager::onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags)
		{
			if (p_listParticle.size() <= 0)
				return;

			for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
			{
				if (it->second->IsAlive())
				{
					(it->second)->VisitParticle(renderer, transform, flags);
				}
			}
		}
		/*
		update
		*/
		void ParticleManager::onUpdate(float dt)
		{
			if (p_listParticle.size() <= 0)
				return;
			//
			//update
			for (auto it = p_listParticle.begin(); it != p_listParticle.end(); ++it)
			{
				if ((it->second)->IsAlive())
				{
					(it->second)->UpdateParticle(dt);
					//
					if (!(it->second)->IsAlive())
					{
						p_listParticleEndLife.push_back(it->first);
					}
				}
			}
		}

		void ParticleManager::AddParticle(RKString name_particle, Vec2 Position, RKString name)
		{
			//remove particle if it exist;
			if (p_listParticle.size() > 0 && p_listParticle.find(name) != p_listParticle.end())
			{
				p_listParticle.erase(name);
			}
			auto par = new Particle();
			par->InitParticle(name_particle);
			par->SetPosition(Position);
			//
			p_listParticle.insert(std::pair<RKString, Particle*>(name, par));
		}

		void ParticleManager::KillParticle(RKString name, float time)
		{
			if (p_listParticle.size() > 0 && p_listParticle.find(name) != p_listParticle.end())
			{
				p_listParticle.at(name)->RemoveParticle(time);
			}
		}

		Particle * ParticleManager::GetParticle(RKString name)
		{
			if (p_listParticle.size() > 0 && p_listParticle.find(name) != p_listParticle.end())
			{
				return p_listParticle.at(name);
			}
			return NULL;
		}

		void ParticleManager::CheckAndRemoveEndLifeParticle()
		{
			//remove endlife particle
			for (int i = 0; i < p_listParticleEndLife.size(); i++)
			{
				p_listParticle.erase(p_listParticleEndLife.at(i));
			}
			p_listParticleEndLife.clear();
		}
	}
}