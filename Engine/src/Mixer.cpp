#include "Mixer.h"
#include "MathApp.h"
#include "AnimationSystem.h"

namespace Azul
{
	void Mixer::BlendCPU_Deprecated(BoneTransform* pResult, BoneTransform* pA, BoneTransform* pB, const float tS, const int numBones)
	{
		for (int i = 0; i < numBones; i++)
		{
			Vec3App::Lerp(pResult->T, pA->T, pB->T, tS);
			QuatApp::Slerp(pResult->Q, pA->Q, pB->Q, tS);
			Vec3App::Lerp(pResult->S, pA->S, pB->S, tS);

			pA++;
			pB++;
			pResult++;
		}
	}
}