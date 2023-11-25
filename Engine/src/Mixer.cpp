#include "Mixer.h"
#include "MathApp.h"

namespace Azul
{
	void Mixer::Blend(Bone* pResult, const Bone* pA, const Bone* pB, const float tS, int numBones)
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