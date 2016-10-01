#ifndef _PARTICLE_MANAGER_H_
#define  _PARTICLE_MANAGER_H_

#include "DisplayView.h"
#include "cocos2d.h"
#include "LoadDataXML.h"
USING_NS_CC;
#define UPDATE_PARTICLE_THREAD 1 //the update of the particle will be in thread to improve perf
namespace Utility
{
	namespace Particle
	{

		class Particle
		{
		protected:
			ParticleSystemQuad*    p_Emitter;
			
			int					   p_reduceEffectBaseHardware;
		public:
			Particle();
			virtual ~Particle();
			void InitParticle(RKString name_particle);
			/*
					draw
			*/
			void DrawParticle(Renderer *renderer, const Mat4& transform, uint32_t flags);
			void VisitParticle(Renderer *renderer, const Mat4& transform, uint32_t flags);
			/*
					update
			*/
			void UpdateParticle(float dt);
			//method in game
			void SetPosition(Vec2 pos);
			void SetAngle(float angle);
			//
			bool IsAlive();
			void RemoveParticle(float time);
		};

		class ParticleManager
		{
		private:
			std::map<RKString, Particle*> p_listParticle;
			std::vector<RKString> p_listParticleEndLife;
		public:
			ParticleManager();
			virtual ~ParticleManager();
			void onInit();
			/*
			draw
			*/
			void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);
			void onVisit(Renderer *renderer, const Mat4& transform, uint32_t flags);
			/*
			update
			*/
			void onUpdate(float dt);
			//
			void CheckAndRemoveEndLifeParticle();
			//
			void AddParticle(RKString name_particle, Vec2 Position, RKString name);
			//
			void KillParticle(RKString name, float time = 0.0f);
			//
			Particle* GetParticle(RKString name);

		};
	}
	#define EffectMgr Particle::ParticleManager::GetInstance() 
}
#endif//_PARTICLE_MANAGER_H_