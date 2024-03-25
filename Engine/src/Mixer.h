#ifndef _Mixer
#define _Mixer

#include "AnimTimer.h"
#include "BoneTransform.h"

namespace Azul
{
	class Mixer
	{
	public:
		static void BlendCPU_Deprecated(BoneTransform* pResult, BoneTransform* pA, BoneTransform* pB, const float tS, const int numBones);
	};
}

#endif
