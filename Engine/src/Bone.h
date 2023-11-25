#ifndef _Bone
#define _Bone

#include "MathEngine.h"

namespace Azul
{
	struct Bone : public Align16
	{
		Vec3  T;
		Quat  Q;
		Vec3  S;
	};
}

#endif
