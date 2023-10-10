//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"
#include "MathApp.h"

//-----------------------------------------------------------------------------
// Mutates result to be a slerp between the source and target by the
// specified factor.
// For a factor of 0.0, result == source.
// For a factor of 1.0, result == target.
//-----------------------------------------------------------------------------

namespace Azul
{
	void QuatApp::Slerp(Quat& result, const Quat& source, const Quat& target, const float slerpFactor)
	{
		float tmpCos = source.dot(target);
		Quat tmpTarget = target;

		if (tmpCos < 0.f)
		{
			tmpCos = -tmpCos;
			tmpTarget = -tmpTarget;
		}

		const float theta = Trig::acos(tmpCos);
		const float sinTheta = Trig::sin(theta);

		if (Util::isZero(sinTheta, 0.0000001f))
		{
			result = Quat();
		}
		else
		{
			Quat a = source * ((Trig::sin((MATH_ONE - slerpFactor) * theta) / sinTheta));
			Quat b = tmpTarget * ((Trig::sin(slerpFactor * theta) / sinTheta));
			result = a + b;
		}
	}

	void QuatApp::SlerpArray(Quat* result, const Quat* source, const Quat* target, const float slerpFactor, const int numQuats)
	{
		for (int i = 0; i < numQuats; i++)
		{
			Quat* placement = &result[i];
			Slerp(*placement, source[i], target[i], slerpFactor);
		}
	}
}

// ---  End of File ---------------