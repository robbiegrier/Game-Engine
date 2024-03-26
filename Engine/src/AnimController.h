#ifndef _AnimController
#define _AnimController

#include "TimerController.h"
#include "Clip.h"
#include "Skeleton.h"
#include "DLink.h"
#include "Mesh.h"
#include "TextureObject.h"

namespace Azul
{
	struct BoneTransform;
	class Skeleton;
	class GameObject;

	class AnimController : public DLink
	{
	public:
		AnimController(AnimTime delta, Clip::Name pInClipName, Skeleton::Name pInSkeletonName, Mesh::Name inMeshName, TextureObject::Name inTextureName);
		AnimController(const AnimController&) = delete;
		AnimController& operator = (const AnimController&) = delete;
		~AnimController();

		void Update();
		GameObject* GetGameObject() const;

		TimerController* GetTimerController() const { return pTimerControl; }

		void SetClip(Clip::Name inClipName);
		Clip* GetClip() const;

		Skeleton* GetSkeleton() const { return pSkeleton; }

	private:
		TimerController* pTimerControl;
		Clip* pClip;
		//AnimatedObjectHierarchy* pAnimatedObjectHierarchy;
		int numBones;

		GameObject* pGameObject;
		Skeleton* pSkeleton;

		void Wash() override;
		bool Compare(DLink* pTargetNode) override;
	};
}

#endif
