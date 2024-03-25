#ifndef _AnimationSystem
#define _AnimationSystem

#include "ManagerBase.h"
#include "AnimController.h"
#include "CSAnimationMixer.h"
#include "CSAnimationWorld.h"

namespace Azul
{
	class AnimationSystem : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static AnimController* Add(AnimController* pInController);
		static void Dump();

		static void Update();

		static CSAnimationMixer& GetComputeShaderMixer();
		static CSAnimationWorld& GetComputeShaderWorld();

	protected:
		// Big four
		AnimationSystem(int initialReserved, int reserveGrow);
		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem& operator =(const AnimationSystem&) = delete;
		~AnimationSystem();

		// Manager
		virtual DLink* CreateNode() override;

		CSAnimationMixer computeShaderMixer;
		CSAnimationWorld computeShaderWorld;

		// Singleton
		static AnimationSystem& GetInstance();
		static AnimationSystem* pInstance;
	};
}

#endif