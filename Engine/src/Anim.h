#ifndef _Anim
#define _Anim

#include "AnimTimer.h"
#include "MathEngine.h"
#include "Clip.h"

namespace Azul
{
#define NUM_BONES 12

	class GameObjectAnimSkin;
	class GameObjectAnimSkeleton;
	class Skeleton;
	struct Bone;

	class Anim
	{
	public:
		Anim(Clip* pInClip);
		Anim(const Anim&) = delete;
		Anim& operator = (const Anim&) = delete;
		~Anim();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();
		static void PoseSkeleton(GameObjectAnimSkeleton* node);
		Skeleton* GetSkeleton() { return pSkeleton; }

		void SetClip(Clip::Name inClipName);

	private:
		Clip* pClip;
		Bone* pBoneResult;
		Skeleton* pSkeleton;
		int numBones;
	};
}

#endif
