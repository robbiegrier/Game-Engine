#ifndef _AnimController
#define _AnimController

#include "TimerController.h"
#include "Clip.h"
#include "Skeleton.h"
#include "DLink.h"
#include "AnimatedObjectHierarchy.h"
#include "Mesh.h"
#include "TextureObject.h"

namespace Azul
{
	class GameObjectBasic;
	struct BoneTransform;

	class AnimController : public DLink
	{
	public:
		AnimController(AnimTime delta, Clip::Name pInClipName, Skeleton::Name pInSkeletonName, Mesh::Name inMeshName, TextureObject::Name inTextureName, AnimMode inMode);
		AnimController(const AnimController&) = delete;
		AnimController& operator = (const AnimController&) = delete;
		~AnimController();

		void Update();
		GameObjectBasic* GetObjectPivot() const;

		TimerController* GetTimerController() const { return pTimerControl; }

		void SetClip(Clip::Name inClipName);

		Clip* GetClip() const;

	private:
		TimerController* pTimerControl;
		Clip* pClip;
		BoneTransform* pBoneResult;
		AnimatedObjectHierarchy* pAnimatedObjectHierarchy;
		int numBones;

		void Wash() override;
		bool Compare(DLink* pTargetNode) override;
	};
}

#endif
