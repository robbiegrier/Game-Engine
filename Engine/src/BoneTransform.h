#ifndef _BoneTransform
#define _BoneTransform

#include "MathEngine.h"

namespace Azul
{
	struct BoneTransform : public Align16
	{
		Vec3  T;
		Quat  Q;
		Vec3  S;
	};
}

#endif
