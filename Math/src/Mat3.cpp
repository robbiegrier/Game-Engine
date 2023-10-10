//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Mat3::Mat3()
		: _m0(MATH_ZERO), _m1(MATH_ZERO), _m2(MATH_ZERO), _m3(MATH_ZERO),
		_m4(MATH_ZERO), _m5(MATH_ZERO), _m6(MATH_ZERO), _m7(MATH_ZERO),
		_m8(MATH_ZERO), _m9(MATH_ZERO), _m10(MATH_ZERO), _m11(MATH_ZERO),
		_m12(MATH_ZERO), _m13(MATH_ZERO), _m14(MATH_ZERO), _m15(MATH_ZERO)
	{
	}

	Mat3& Mat3::operator=(const Mat3& A)
	{
		_v0 = A._v0;
		_v1 = A._v1;
		_v2 = A._v2;
		_v3 = A._v3;
		return *this;
	}

	Mat3::Mat3(const Mat3& tM)
		: _m0(tM._m0), _m1(tM._m1), _m2(tM._m2), _m3(tM._m3),
		_m4(tM._m4), _m5(tM._m5), _m6(tM._m6), _m7(tM._m7),
		_m8(tM._m8), _m9(tM._m9), _m10(tM._m10), _m11(tM._m11),
		_m12(tM._m12), _m13(tM._m13), _m14(tM._m14), _m15(tM._m15)
	{
	}

	Mat3::~Mat3()
	{
		// Do Nothing
	}

	Mat3::Mat3(const Vec3& tV0, const Vec3& tV1, const Vec3& tV2)
		: _v0(tV0, MATH_ZERO),
		_v1(tV1, MATH_ZERO),
		_v2(tV2, MATH_ZERO),
		_v3(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ZERO)
	{
	}

	Mat3::Mat3(const Mat4& tM)
		: _m0(tM._m0), _m1(tM._m1), _m2(tM._m2), _m3(tM._m3),
		_m4(tM._m4), _m5(tM._m5), _m6(tM._m6), _m7(tM._m7),
		_m8(tM._m8), _m9(tM._m9), _m10(tM._m10), _m11(tM._m11),
		_m12(tM._m12), _m13(tM._m13), _m14(tM._m14), _m15(tM._m15)
	{
	}

	Mat3::Mat3(const Special type)
	{
		set(type);
	}

	Vec3 Mat3::get(const Row3 type) const
	{
		return Vec3(_rows[static_cast<int>(type)]);
	}

	void Mat3::set(const Mat3& mIn)
	{
		*this = mIn;
	}

	void Mat3::set(const Special type)
	{
		switch (type)
		{
		case Special::Zero:
			_m0 = MATH_ZERO; _m1 = MATH_ZERO; _m2 = MATH_ZERO; _m3 = MATH_ZERO;
			_m4 = MATH_ZERO; _m5 = MATH_ZERO; _m6 = MATH_ZERO; _m7 = MATH_ZERO;
			_m8 = MATH_ZERO; _m9 = MATH_ZERO; _m10 = MATH_ZERO; _m11 = MATH_ZERO;
			_m12 = MATH_ZERO; _m13 = MATH_ZERO; _m14 = MATH_ZERO; _m15 = MATH_ZERO;
			break;
		case Special::Identity:
			_m0 = MATH_ONE; _m1 = MATH_ZERO; _m2 = MATH_ZERO; _m3 = MATH_ZERO;
			_m4 = MATH_ZERO; _m5 = MATH_ONE; _m6 = MATH_ZERO; _m7 = MATH_ZERO;
			_m8 = MATH_ZERO; _m9 = MATH_ZERO; _m10 = MATH_ONE; _m11 = MATH_ZERO;
			_m12 = MATH_ZERO; _m13 = MATH_ZERO; _m14 = MATH_ZERO; _m15 = MATH_ONE;
			break;
		default:
			assert(false);
		}
	}

	void Mat3::set(const Row3 type, const Vec3& V)
	{
		_rows[static_cast<int>(type)].set(V, MATH_ZERO);
	}

	void Mat3::set(const Vec3& V0, const Vec3& V1, const Vec3& V2)
	{
		_rows[0].set(V0, MATH_ZERO);
		_rows[1].set(V1, MATH_ZERO);
		_rows[2].set(V2, MATH_ZERO);
	}

	float& Mat3::operator[] (const enum m0_enum)
	{
		return _m0;
	}

	float& Mat3::operator[] (const enum m1_enum)
	{
		return _m1;
	}

	float& Mat3::operator[] (const enum m2_enum)
	{
		return _m2;
	}

	float& Mat3::operator[] (const enum m4_enum)
	{
		return _m4;
	}

	float& Mat3::operator[] (const enum m5_enum)
	{
		return _m5;
	}

	float& Mat3::operator[] (const enum m6_enum)
	{
		return _m6;
	}

	float& Mat3::operator[] (const enum m8_enum)
	{
		return _m8;
	}

	float& Mat3::operator[] (const enum m9_enum)
	{
		return _m9;
	}

	float& Mat3::operator[] (const enum m10_enum)
	{
		return _m10;
	}

	float Mat3::operator[](const m0_enum) const
	{
		return _m0;
	}

	float Mat3::operator[](const m1_enum) const
	{
		return _m1;
	}

	float Mat3::operator[](const m2_enum) const
	{
		return _m2;
	}

	float Mat3::operator[](const m4_enum) const
	{
		return _m4;
	}

	float Mat3::operator[](const m5_enum) const
	{
		return _m5;
	}

	float Mat3::operator[](const m6_enum) const
	{
		return _m6;
	}

	float Mat3::operator[](const m8_enum) const
	{
		return _m8;
	}

	float Mat3::operator[](const m9_enum) const
	{
		return _m9;
	}

	float Mat3::operator[](const m10_enum) const
	{
		return _m10;
	}

	void Mat3::m0(const float v)
	{
		_m0 = v;
	}

	void Mat3::m1(const float v)
	{
		_m1 = v;
	}

	void Mat3::m2(const float v)
	{
		_m2 = v;
	}

	void Mat3::m4(const float v)
	{
		_m4 = v;
	}

	void Mat3::m5(const float v)
	{
		_m5 = v;
	}

	void Mat3::m6(const float v)
	{
		_m6 = v;
	}

	void Mat3::m8(const float v)
	{
		_m8 = v;
	}

	void Mat3::m9(const float v)
	{
		_m9 = v;
	}

	void Mat3::m10(const float v)
	{
		_m10 = v;
	}

	float Mat3::m0() const
	{
		return _m0;
	}

	float Mat3::m1() const
	{
		return _m1;
	}

	float Mat3::m2() const
	{
		return _m2;
	}

	float Mat3::m4() const
	{
		return _m4;
	}

	float Mat3::m5() const
	{
		return _m5;
	}

	float Mat3::m6() const
	{
		return _m6;
	}

	float Mat3::m8() const
	{
		return _m8;
	}

	float Mat3::m9() const
	{
		return _m9;
	}

	float Mat3::m10() const
	{
		return _m10;
	}

	float Mat3::det() const
	{
		const float a = _m0 * ((_m5 * _m10) - (_m6 * _m9));
		const float b = _m1 * ((_m4 * _m10) - (_m6 * _m8));
		const float c = _m2 * ((_m4 * _m9) - (_m5 * _m8));

		return a - b + c;
	}

	Mat3& Mat3::T(void)
	{
		float tmp = _m1; _m1 = _m4; _m4 = tmp;
		tmp = _m2; _m2 = _m8; _m8 = tmp;
		tmp = _m6; _m6 = _m9; _m9 = tmp;
		return *this;
	}

	Mat3 Mat3::getT(void) const
	{
		return Mat3(
			Vec3(_m0, _m4, _m8),
			Vec3(_m1, _m5, _m9),
			Vec3(_m2, _m6, _m10)
		);
	}

	Mat3 Mat3::getInv(void) const
	{
		Mat3 m;

		const float a = (_m5 * _m10) - (_m6 * _m9);
		const float b = (_m4 * _m10) - (_m6 * _m8);
		const float c = (_m4 * _m9) - (_m5 * _m8);

		const float determinant = (_m0 * a) - (_m1 * b) + (_m2 * c);
		const float invDet = MATH_ONE / determinant;

		m._m0 = a * invDet;
		m._m1 = ((_m2 * _m9) - (_m1 * _m10)) * invDet;
		m._m2 = ((_m1 * _m6) - (_m2 * _m5)) * invDet;
		m._m4 = b * invDet;
		m._m5 = ((_m0 * _m10) - (_m2 * _m8)) * invDet;
		m._m6 = ((_m2 * _m4) - (_m0 * _m6)) * invDet;
		m._m8 = c * invDet;
		m._m9 = ((_m1 * _m8) - (_m0 * _m9)) * invDet;
		m._m10 = ((_m0 * _m5) - (_m1 * _m4)) * invDet;

		return m;
	}

	Mat3& Mat3::inv(void)
	{
		const float a = (_m5 * _m10) - (_m6 * _m9);
		const float b = (_m4 * _m10) - (_m6 * _m8);
		const float c = (_m4 * _m9) - (_m5 * _m8);

		const float determinant = (_m0 * a) - (_m1 * b) + (_m2 * c);
		const float invDet = MATH_ONE / determinant;

		const float tmp1 = ((_m2 * _m9) - (_m1 * _m10)) * invDet;
		const float tmp2 = ((_m1 * _m6) - (_m2 * _m5)) * invDet;
		const float tmp5 = ((_m0 * _m10) - (_m2 * _m8)) * invDet;
		const float tmp6 = ((_m2 * _m4) - (_m0 * _m6)) * invDet;
		const float tmp9 = ((_m1 * _m8) - (_m0 * _m9)) * invDet;
		const float tmp10 = ((_m0 * _m5) - (_m1 * _m4)) * invDet;

		_m0 = a * invDet;
		_m1 = tmp1;
		_m2 = tmp2;
		_m4 = b * invDet;
		_m5 = tmp5;
		_m6 = tmp6;
		_m8 = c * invDet;
		_m9 = tmp9;
		_m10 = tmp10;

		return *this;
	}

	bool Mat3::isEqual(const Mat3& A, const float epsilon) const
	{
		return _v0.isEqual(A._v0, epsilon) &&
			_v1.isEqual(A._v1, epsilon) &&
			_v2.isEqual(A._v2, epsilon);
	}

	bool Mat3::isIdentity(const float epsilon) const
	{
		return Util::isOne(_m0, epsilon) && Util::isZero(_m1, epsilon) && Util::isZero(_m2, epsilon) &&
			Util::isZero(_m4, epsilon) && Util::isOne(_m5, epsilon) && Util::isZero(_m6, epsilon) &&
			Util::isZero(_m8, epsilon) && Util::isZero(_m9, epsilon) && Util::isOne(_m10, epsilon);
	}

	Mat3 Mat3::operator+(void) const
	{
		return Mat3(*this);
	}

	Mat3 Mat3::operator+(const Mat3& A) const
	{
		return Mat3(
			Vec3(_m0 + A._m0, _m1 + A._m1, _m2 + A._m2),
			Vec3(_m4 + A._m4, _m5 + A._m5, _m6 + A._m6),
			Vec3(_m8 + A._m8, _m9 + A._m9, _m10 + A._m10)
		);
	}

	Mat3& Mat3::operator+=(const Mat3& A)
	{
		_v0.set(_m0 + A._m0, _m1 + A._m1, _m2 + A._m2, MATH_ZERO);
		_v1.set(_m4 + A._m4, _m5 + A._m5, _m6 + A._m6, MATH_ZERO);
		_v2.set(_m8 + A._m8, _m9 + A._m9, _m10 + A._m10, MATH_ZERO);
		return *this;
	}

	Mat3 Mat3::operator-(void) const
	{
		return Mat3(Vec3(-_v0), Vec3(-_v1), Vec3(-_v2));
	}

	Mat3 Mat3::operator-(const Mat3& A) const
	{
		return Mat3(Vec3(_v0 - A._v0), Vec3(_v1 - A._v1), Vec3(_v2 - A._v2));
	}

	Mat3& Mat3::operator-=(const Mat3& A)
	{
		_v0 -= A._v0;
		_v1 -= A._v1;
		_v2 -= A._v2;
		return *this;
	}

	Mat3 Mat3::operator*(const float s) const
	{
		return Mat3(
			Vec3(_v0 * s),
			Vec3(_v1 * s),
			Vec3(_v2 * s)
		);
	}

	Mat3& Mat3::operator*=(const float scale)
	{
		_v0 *= scale;
		_v1 *= scale;
		_v2 *= scale;
		return *this;
	}

	Mat3 operator*(const float scale, const Mat3& A)
	{
		return A * scale;
	}

	Mat3 Mat3::operator*(const Mat3& A) const
	{
		return Mat3(
			Vec3(
				(_m0 * A._m0) + (_m1 * A._m4) + (_m2 * A._m8),
				(_m0 * A._m1) + (_m1 * A._m5) + (_m2 * A._m9),
				(_m0 * A._m2) + (_m1 * A._m6) + (_m2 * A._m10)
			),
			Vec3(
				(_m4 * A._m0) + (_m5 * A._m4) + (_m6 * A._m8),
				(_m4 * A._m1) + (_m5 * A._m5) + (_m6 * A._m9),
				(_m4 * A._m2) + (_m5 * A._m6) + (_m6 * A._m10)
			),
			Vec3(
				(_m8 * A._m0) + (_m9 * A._m4) + (_m10 * A._m8),
				(_m8 * A._m1) + (_m9 * A._m5) + (_m10 * A._m9),
				(_m8 * A._m2) + (_m9 * A._m6) + (_m10 * A._m10)
			)
		);
	}

	Mat3& Mat3::operator*=(const Mat3& A)
	{
		_v0 = Vec4(
			(_m0 * A._m0) + (_m1 * A._m4) + (_m2 * A._m8),
			(_m0 * A._m1) + (_m1 * A._m5) + (_m2 * A._m9),
			(_m0 * A._m2) + (_m1 * A._m6) + (_m2 * A._m10),
			MATH_ZERO
		);
		_v1 = Vec4(
			(_m4 * A._m0) + (_m5 * A._m4) + (_m6 * A._m8),
			(_m4 * A._m1) + (_m5 * A._m5) + (_m6 * A._m9),
			(_m4 * A._m2) + (_m5 * A._m6) + (_m6 * A._m10),
			MATH_ZERO
		);
		_v2 = Vec4(
			(_m8 * A._m0) + (_m9 * A._m4) + (_m10 * A._m8),
			(_m8 * A._m1) + (_m9 * A._m5) + (_m10 * A._m9),
			(_m8 * A._m2) + (_m9 * A._m6) + (_m10 * A._m10),
			MATH_ZERO
		);
		return *this;
	}

	void Mat3::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}
}

// ---  End of File ---