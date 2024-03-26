#ifndef _AnimationSystem
#define _AnimationSystem

#include "CSAnimationMixer.h"
#include "CSAnimationWorld.h"

namespace Azul
{
	class AnimationSystem
	{
	public:
		static void Create();
		static void Destroy();
		static void Dump();

		static CSAnimationMixer& GetComputeShaderMixer();
		static CSAnimationWorld& GetComputeShaderWorld();

	protected:
		// Big four
		AnimationSystem();
		AnimationSystem(const AnimationSystem&) = delete;
		AnimationSystem& operator =(const AnimationSystem&) = delete;
		~AnimationSystem();

		CSAnimationMixer computeShaderMixer;
		CSAnimationWorld computeShaderWorld;

		// Singleton
		static AnimationSystem& GetInstance();
		static AnimationSystem* pInstance;
	};
}

#endif