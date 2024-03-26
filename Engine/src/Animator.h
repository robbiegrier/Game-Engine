#ifndef _Animator
#define _Animator

#include "Component.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{
	class TimerController;

	class Animator : public Component
	{
	public:
		Animator();
		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;
		virtual ~Animator();

		virtual void Update(float deltaTime) override;

		void SetActiveClip(Clip::Name inClipName);
		void SetSkeleton(Skeleton::Name inSkeletonName);

		Clip* GetActiveClip() const { return pActiveClip; }
		Skeleton* GetSkeleton() const { return pSkeleton; }

		void SetAnimationSpeed(float inSpeed) { animationSpeed = inSpeed; }

		virtual void Serialize(json& out) override;
		virtual void Deserialize(const json& in) override;

	private:
		TimerController* pTimerController;
		Clip* pActiveClip;
		Skeleton* pSkeleton;
		float animationSpeed = 1.f;

		static constexpr float ANIM_SPEED_CONST = 20.f;
	};
}

#endif