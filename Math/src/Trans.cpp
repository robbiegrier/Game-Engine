//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Trans::Trans()
		: Mat4(Special::Identity)
	{
		privSetTransHint();
	}

	Trans::Trans(const float tx, const float ty, const float tz)
	{
		set(tx, ty, tz);
	}

	Trans::Trans(const Vec3& vTrans)
	{
		set(vTrans);
	}

	void Trans::set(const float tx, const float ty, const float tz)
	{
		_v0.set(MATH_ONE, MATH_ZERO, MATH_ZERO, MATH_ZERO);
		_v1.set(MATH_ZERO, MATH_ONE, MATH_ZERO, MATH_ZERO);
		_v2.set(MATH_ZERO, MATH_ZERO, MATH_ONE, MATH_ZERO);
		_v3.set(tx, ty, tz, MATH_ONE);
		privSetTransHint();
	}

	void Trans::set(const Vec3& vTrans)
	{
		set(vTrans[x], vTrans[y], vTrans[z]);
	}

	Mat4 Trans::operator*(const Mat4& A) const
	{
		return static_cast<Mat4>(*this) * A;
	}

	Mat4 Trans::operator*(const Quat& q) const
	{
		return *this * Mat4(q);
	}

	Mat4 Trans::operator*(const Scale& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}

	Mat4 Trans::operator*(const Rot& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}

	Trans Trans::operator*(const Trans& A) const
	{
		return Trans(_m12 + A._m12, _m13 + A._m13, _m14 + A._m14);
	}

	Trans& Trans::operator*=(const Trans& A)
	{
		_m12 += A._m12;
		_m13 += A._m13;
		_m14 += A._m14;
		return *this;
	}
}

// ---  End of File ---