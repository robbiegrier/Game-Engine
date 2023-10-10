//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Scale::Scale()
		: Mat4(Special::Identity)
	{
		privSetScaleHint();
	}

	Scale::Scale(const float sx, const float sy, const float sz)
	{
		set(sx, sy, sz);
	}

	Scale::Scale(const Vec3& vScale)
	{
		set(vScale);
	}

	void Scale::set(const float sx, const float sy, const float sz)
	{
		_v0.set(sx, MATH_ZERO, MATH_ZERO, MATH_ZERO);
		_v1.set(MATH_ZERO, sy, MATH_ZERO, MATH_ZERO);
		_v2.set(MATH_ZERO, MATH_ZERO, sz, MATH_ZERO);
		_v3.set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
		privSetScaleHint();
	}

	void Scale::set(const Vec3& vScale)
	{
		set(vScale[x], vScale[y], vScale[z]);
	}

	Mat4 Scale::operator*(const Mat4& A) const
	{
		return static_cast<Mat4>(*this) * A;
	}

	Mat4 Scale::operator*(const Quat& q) const
	{
		return *this * Mat4(q);
	}

	Scale Scale::operator*(const Scale& A) const
	{
		return Scale(_m0 * A._m0, _m5 * A._m5, _m10 * A._m10);
	}

	Scale& Scale::operator*=(const Scale& A)
	{
		_m0 *= A._m0;
		_m5 *= A._m5;
		_m10 *= A._m10;
		return *this;
	}

	Mat4 Scale::operator*(const Rot& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}

	Mat4 Scale::operator*(const Trans& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}
}

// ---  End of File ---