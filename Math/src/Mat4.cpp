//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Mat4::Mat4()
		: _m0(MATH_ZERO), _m1(MATH_ZERO), _m2(MATH_ZERO), _m3(MATH_ZERO),
		_m4(MATH_ZERO), _m5(MATH_ZERO), _m6(MATH_ZERO), _m7(MATH_ZERO),
		_m8(MATH_ZERO), _m9(MATH_ZERO), _m10(MATH_ZERO), _m11(MATH_ZERO),
		_m12(MATH_ZERO), _m13(MATH_ZERO), _m14(MATH_ZERO), _m15(MATH_ZERO)
	{
	}

	Mat4& Mat4::operator=(const Mat4& A)
	{
		_v0 = A._v0;
		_v1 = A._v1;
		_v2 = A._v2;
		_v3 = A._v3;
		return *this;
	}

	Mat4::Mat4(const Mat4& tM)
		: _m0(tM._m0), _m1(tM._m1), _m2(tM._m2), _m3(tM._m3),
		_m4(tM._m4), _m5(tM._m5), _m6(tM._m6), _m7(tM._m7),
		_m8(tM._m8), _m9(tM._m9), _m10(tM._m10), _m11(tM._m11),
		_m12(tM._m12), _m13(tM._m13), _m14(tM._m14), _m15(tM._m15)
	{
	}

	Mat4::~Mat4()
	{
		// Do nothing
	}

	Mat4::Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3)
		: _v0(tV0), _v1(tV1), _v2(tV2), _v3(tV3)
	{
	}

	Mat4::Mat4(const Rot1 type, const float angle)
	{
		set(type, angle);
	}

	Mat4::Mat4(const Trans, const float tx, const float ty, const float tz)
		: _v0(MATH_ONE, MATH_ZERO, MATH_ZERO, MATH_ZERO),
		_v1(MATH_ZERO, MATH_ONE, MATH_ZERO, MATH_ZERO),
		_v2(MATH_ZERO, MATH_ZERO, MATH_ONE, MATH_ZERO),
		_v3(tx, ty, tz, MATH_ONE)
	{
	}

	Mat4::Mat4(const Trans, const Vec3& vTrans)
		: _v0(MATH_ONE, MATH_ZERO, MATH_ZERO, MATH_ZERO),
		_v1(MATH_ZERO, MATH_ONE, MATH_ZERO, MATH_ZERO),
		_v2(MATH_ZERO, MATH_ZERO, MATH_ONE, MATH_ZERO),
		_v3(vTrans, MATH_ONE)
	{
	}

	Mat4::Mat4(const Scale, const float sx, const float sy, const float sz)
		: _v0(sx, MATH_ZERO, MATH_ZERO, MATH_ZERO),
		_v1(MATH_ZERO, sy, MATH_ZERO, MATH_ZERO),
		_v2(MATH_ZERO, MATH_ZERO, sz, MATH_ZERO),
		_v3(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE)
	{
	}

	Mat4::Mat4(const Scale, const Vec3& vScale)
		: _v0(vScale[x], MATH_ZERO, MATH_ZERO, MATH_ZERO),
		_v1(MATH_ZERO, vScale[y], MATH_ZERO, MATH_ZERO),
		_v2(MATH_ZERO, MATH_ZERO, vScale[z], MATH_ZERO),
		_v3(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE)
	{
	}

	Mat4::Mat4(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		set(mode, angle_0, angle_1, angle_2);
	}

	Mat4::Mat4(const Special type)
	{
		set(type);
	}

	void Mat4::set(const Mat4& mIn)
	{
		*this = mIn;
	}

	void Mat4::set(const Vec4& V0, const Vec4& V1, const Vec4& V2, const Vec4& V3)
	{
		_v0 = V0;
		_v1 = V1;
		_v2 = V2;
		_v3 = V3;
	}

	void Mat4::set(const Rot1 type, const float angle)
	{
		switch (type)
		{
		case Rot1::X:
			_v0.set(MATH_ONE, MATH_ZERO, MATH_ZERO, MATH_ZERO);
			_v1.set(MATH_ZERO, Trig::cos(angle), Trig::sin(angle), MATH_ZERO);
			_v2.set(MATH_ZERO, -Trig::sin(angle), Trig::cos(angle), MATH_ZERO);
			_v3.set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
			break;
		case Rot1::Y:
			_v0.set(Trig::cos(angle), MATH_ZERO, -Trig::sin(angle), MATH_ZERO);
			_v1.set(MATH_ZERO, MATH_ONE, MATH_ZERO, MATH_ZERO);
			_v2.set(Trig::sin(angle), MATH_ZERO, Trig::cos(angle), MATH_ZERO);
			_v3.set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
			break;
		case Rot1::Z:
			_v0.set(Trig::cos(angle), Trig::sin(angle), MATH_ZERO, MATH_ZERO);
			_v1.set(-Trig::sin(angle), Trig::cos(angle), MATH_ZERO, MATH_ZERO);
			_v2.set(MATH_ZERO, MATH_ZERO, MATH_ONE, MATH_ZERO);
			_v3.set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
			break;
		default:
			assert(false);
		}
	}

	void Mat4::set(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		switch (mode)
		{
		case Rot3::XYZ:
			*this = Mat4(Rot1::X, angle_0) * Mat4(Rot1::Y, angle_1) * Mat4(Rot1::Z, angle_2);
			break;
		default:
			assert(false);
		}
	}

	void Mat4::set(const Trans type, const float tx, const float ty, const float tz)
	{
		switch (type)
		{
		case Trans::XYZ:
			_v0.set(MATH_ONE, MATH_ZERO, MATH_ZERO, MATH_ZERO);
			_v1.set(MATH_ZERO, MATH_ONE, MATH_ZERO, MATH_ZERO);
			_v2.set(MATH_ZERO, MATH_ZERO, MATH_ONE, MATH_ZERO);
			_v3.set(tx, ty, tz, MATH_ONE);
			break;
		default:
			assert(false);
		}
	}

	void Mat4::set(const Trans type, const Vec3& vTrans)
	{
		set(type, vTrans[x], vTrans[y], vTrans[z]);
	}

	void Mat4::set(const Scale type, const float sx, const float sy, const float sz)
	{
		switch (type)
		{
		case Scale::XYZ:
			_v0.set(sx, MATH_ZERO, MATH_ZERO, MATH_ZERO);
			_v1.set(MATH_ZERO, sy, MATH_ZERO, MATH_ZERO);
			_v2.set(MATH_ZERO, MATH_ZERO, sz, MATH_ZERO);
			_v3.set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
			break;
		default:
			assert(false);
		}
	}

	void Mat4::set(const Scale type, const Vec3& vScale)
	{
		set(type, vScale[x], vScale[y], vScale[z]);
	}

	void Mat4::set(const Special type)
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

	void Mat4::set(const Row type, const Vec4& V)
	{
		_rows[static_cast<int>(type)].set(V);
	}

	Vec4 Mat4::get(const Row type) const
	{
		return _rows[static_cast<int>(type)];
	}

	Mat4 Mat4::operator+(void) const
	{
		return Mat4(*this);
	}

	Mat4 Mat4::operator+(const Mat4& A) const
	{
		return Mat4(
			_v0 + A._v0,
			_v1 + A._v1,
			_v2 + A._v2,
			_v3 + A._v3
		);
	}

	void Mat4::operator+=(const Mat4& A)
	{
		_v0 += A._v0;
		_v1 += A._v1;
		_v2 += A._v2;
		_v3 += A._v3;
	}

	Mat4 Mat4::operator-(void) const
	{
		return Mat4(-_v0, -_v1, -_v2, -_v3);
	}

	Mat4 Mat4::operator-(const Mat4& A) const
	{
		return Mat4(_v0 - A._v0, _v1 - A._v1, _v2 - A._v2, _v3 - A._v3);
	}

	void Mat4::operator-=(const Mat4& A)
	{
		_v0 -= A._v0;
		_v1 -= A._v1;
		_v2 -= A._v2;
		_v3 -= A._v3;
	}

	Mat4 Mat4::operator*(const Mat4& A) const
	{
		return Mat4(
			Vec4(
				(_m0 * A._m0) + (_m1 * A._m4) + (_m2 * A._m8) + (_m3 * A._m12),
				(_m0 * A._m1) + (_m1 * A._m5) + (_m2 * A._m9) + (_m3 * A._m13),
				(_m0 * A._m2) + (_m1 * A._m6) + (_m2 * A._m10) + (_m3 * A._m14),
				(_m0 * A._m3) + (_m1 * A._m7) + (_m2 * A._m11) + (_m3 * A._m15)
			),
			Vec4(
				(_m4 * A._m0) + (_m5 * A._m4) + (_m6 * A._m8) + (_m7 * A._m12),
				(_m4 * A._m1) + (_m5 * A._m5) + (_m6 * A._m9) + (_m7 * A._m13),
				(_m4 * A._m2) + (_m5 * A._m6) + (_m6 * A._m10) + (_m7 * A._m14),
				(_m4 * A._m3) + (_m5 * A._m7) + (_m6 * A._m11) + (_m7 * A._m15)
			),
			Vec4(
				(_m8 * A._m0) + (_m9 * A._m4) + (_m10 * A._m8) + (_m11 * A._m12),
				(_m8 * A._m1) + (_m9 * A._m5) + (_m10 * A._m9) + (_m11 * A._m13),
				(_m8 * A._m2) + (_m9 * A._m6) + (_m10 * A._m10) + (_m11 * A._m14),
				(_m8 * A._m3) + (_m9 * A._m7) + (_m10 * A._m11) + (_m11 * A._m15)
			),
			Vec4(
				(_m12 * A._m0) + (_m13 * A._m4) + (_m14 * A._m8) + (_m15 * A._m12),
				(_m12 * A._m1) + (_m13 * A._m5) + (_m14 * A._m9) + (_m15 * A._m13),
				(_m12 * A._m2) + (_m13 * A._m6) + (_m14 * A._m10) + (_m15 * A._m14),
				(_m12 * A._m3) + (_m13 * A._m7) + (_m14 * A._m11) + (_m15 * A._m15)
			)
		);
	}

	void Mat4::operator*=(const Mat4& A)
	{
		float tmp0 = (_m0 * A._m0) + (_m1 * A._m4) + (_m2 * A._m8) + (_m3 * A._m12);
		float tmp1 = (_m0 * A._m1) + (_m1 * A._m5) + (_m2 * A._m9) + (_m3 * A._m13);
		float tmp2 = (_m0 * A._m2) + (_m1 * A._m6) + (_m2 * A._m10) + (_m3 * A._m14);
		float tmp3 = (_m0 * A._m3) + (_m1 * A._m7) + (_m2 * A._m11) + (_m3 * A._m15);
		float tmp4 = (_m4 * A._m0) + (_m5 * A._m4) + (_m6 * A._m8) + (_m7 * A._m12);
		float tmp5 = (_m4 * A._m1) + (_m5 * A._m5) + (_m6 * A._m9) + (_m7 * A._m13);
		float tmp6 = (_m4 * A._m2) + (_m5 * A._m6) + (_m6 * A._m10) + (_m7 * A._m14);
		float tmp7 = (_m4 * A._m3) + (_m5 * A._m7) + (_m6 * A._m11) + (_m7 * A._m15);
		float tmp8 = (_m8 * A._m0) + (_m9 * A._m4) + (_m10 * A._m8) + (_m11 * A._m12);
		float tmp9 = (_m8 * A._m1) + (_m9 * A._m5) + (_m10 * A._m9) + (_m11 * A._m13);
		float tmp10 = (_m8 * A._m2) + (_m9 * A._m6) + (_m10 * A._m10) + (_m11 * A._m14);
		float tmp11 = (_m8 * A._m3) + (_m9 * A._m7) + (_m10 * A._m11) + (_m11 * A._m15);
		float tmp12 = (_m12 * A._m0) + (_m13 * A._m4) + (_m14 * A._m8) + (_m15 * A._m12);
		float tmp13 = (_m12 * A._m1) + (_m13 * A._m5) + (_m14 * A._m9) + (_m15 * A._m13);
		float tmp14 = (_m12 * A._m2) + (_m13 * A._m6) + (_m14 * A._m10) + (_m15 * A._m14);
		float tmp15 = (_m12 * A._m3) + (_m13 * A._m7) + (_m14 * A._m11) + (_m15 * A._m15);

		_v0.set(tmp0, tmp1, tmp2, tmp3);
		_v1.set(tmp4, tmp5, tmp6, tmp7);
		_v2.set(tmp8, tmp9, tmp10, tmp11);
		_v3.set(tmp12, tmp13, tmp14, tmp15);
	}

	Mat4::Mat4(const Rot, const Vec3& vAxis, const float angle)
	{
		set(Rot::AxisAngle, vAxis, angle);
	}

	Mat4::Mat4(const Orient type, const Vec3& dof, const Vec3& up)
	{
		set(type, dof, up);
	}

	void Mat4::set(const Rot, const Vec3& vAxis, const float angle)
	{
		//Quat qtmp(Quat::Rot::AxisAngle, vAxis, angle);
		//this->set(qtmp);

		const float angle_a = 0.5f * angle;
		float cos_a;
		float sin_a;

		Trig::cossin(cos_a, sin_a, angle_a);

		Vec3 qV = vAxis.getNorm();

		qV *= sin_a;

		Vec4 Q;
		Q[x] = qV[x];
		Q[y] = qV[y];
		Q[z] = qV[z];
		Q[w] = cos_a;

		float x2, y2, z2;
		float xx, xy, xz;
		float yy, yz, zz;
		float wx, wy, wz;

		x2 = Q[x] + Q[x];
		y2 = Q[y] + Q[y];
		z2 = Q[z] + Q[z];

		xx = Q[x] * x2;
		xy = Q[x] * y2;
		xz = Q[x] * z2;

		yy = Q[y] * y2;
		yz = Q[y] * z2;
		zz = Q[z] * z2;

		wx = Q[w] * x2;
		wy = Q[w] * y2;
		wz = Q[w] * z2;

		_m0 = 1.0f - (yy + zz);
		_m1 = xy + wz;
		_m2 = xz - wy;
		_m3 = 0.0f;

		_m4 = xy - wz;
		_m5 = 1.0f - (xx + zz);
		_m6 = yz + wx;
		_m7 = 0.0f;

		_m8 = xz + wy;
		_m9 = yz - wx;
		_m10 = 1.0f - (xx + yy);
		_m11 = 0.0f;
		_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Mat4::set(const Orient, const Vec3& dof, const Vec3& up)
	{
		assert(dof.dot(up) != 1.0f);

		const Vec3 rz = dof.getNorm();

		Vec3 rx = up.cross(rz);
		rx.norm();

		Vec3 ry = rz.cross(rx);
		ry.norm();

		set(Vec4(rx, 0), Vec4(ry, 0), Vec4(rz, 0), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		_m3 = 0.0f;
		_m7 = 0.0f;
		_m11 = 0.0f;
		_m15 = 1.0f;
	}

	void Mat4::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}

	bool Mat4::privHasHint() const
	{
		return ((_m15_bits & 0xFFFFFFF0) == 0x3f800000);
	}

	float& Mat4::operator[] (const enum m0_enum)
	{
		return _m0;
	}

	float& Mat4::operator[] (const enum m1_enum)
	{
		return _m1;
	}

	float& Mat4::operator[] (const enum m2_enum)
	{
		return _m2;
	}

	float& Mat4::operator[] (const enum m3_enum)
	{
		return _m3;
	}

	float& Mat4::operator[] (const enum m4_enum)
	{
		return _m4;
	}

	float& Mat4::operator[] (const enum m5_enum)
	{
		return _m5;
	}

	float& Mat4::operator[] (const enum m6_enum)
	{
		return _m6;
	}

	float& Mat4::operator[] (const enum m7_enum)
	{
		return _m7;
	}

	float& Mat4::operator[] (const enum m8_enum)
	{
		return _m8;
	}

	float& Mat4::operator[] (const enum m9_enum)
	{
		return _m9;
	}

	float& Mat4::operator[] (const enum m10_enum)
	{
		return _m10;
	}

	float& Mat4::operator[] (const enum m11_enum)
	{
		return _m11;
	}

	float& Mat4::operator[] (const enum m12_enum)
	{
		return _m12;
	}

	float& Mat4::operator[] (const enum m13_enum)
	{
		return _m13;
	}

	float& Mat4::operator[] (const enum m14_enum)
	{
		return _m14;
	}

	float& Mat4::operator[] (const enum m15_enum)
	{
		return _m15;
	}

	float Mat4::operator[](const m0_enum) const
	{
		return _m0;
	}

	float Mat4::operator[](const m1_enum) const
	{
		return _m1;
	}

	float Mat4::operator[](const m2_enum) const
	{
		return _m2;
	}

	float Mat4::operator[](const m3_enum) const
	{
		return _m3;
	}

	float Mat4::operator[](const m4_enum) const
	{
		return _m4;
	}

	float Mat4::operator[](const m5_enum) const
	{
		return _m5;
	}

	float Mat4::operator[](const m6_enum) const
	{
		return _m6;
	}

	float Mat4::operator[](const m7_enum) const
	{
		return _m7;
	}

	float Mat4::operator[](const m8_enum) const
	{
		return _m8;
	}

	float Mat4::operator[](const m9_enum) const
	{
		return _m9;
	}

	float Mat4::operator[](const m10_enum) const
	{
		return _m10;
	}

	float Mat4::operator[](const m11_enum) const
	{
		return _m11;
	}

	float Mat4::operator[](const m12_enum) const
	{
		return _m12;
	}

	float Mat4::operator[](const m13_enum) const
	{
		return _m13;
	}

	float Mat4::operator[](const m14_enum) const
	{
		return _m14;
	}

	float Mat4::operator[](const m15_enum) const
	{
		return _m15;
	}

	void Mat4::m0(const float v)
	{
		_m0 = v;
	}

	void Mat4::m1(const float v)
	{
		_m1 = v;
	}

	void Mat4::m2(const float v)
	{
		_m2 = v;
	}

	void Mat4::m3(const float v)
	{
		_m3 = v;
	}

	void Mat4::m4(const float v)
	{
		_m4 = v;
	}

	void Mat4::m5(const float v)
	{
		_m5 = v;
	}

	void Mat4::m6(const float v)
	{
		_m6 = v;
	}

	void Mat4::m7(const float v)
	{
		_m7 = v;
	}

	void Mat4::m8(const float v)
	{
		_m8 = v;
	}

	void Mat4::m9(const float v)
	{
		_m9 = v;
	}

	void Mat4::m10(const float v)
	{
		_m10 = v;
	}

	void Mat4::m11(const float v)
	{
		_m11 = v;
	}

	void Mat4::m12(const float v)
	{
		_m12 = v;
	}

	void Mat4::m13(const float v)
	{
		_m13 = v;
	}

	void Mat4::m14(const float v)
	{
		_m14 = v;
	}

	void Mat4::m15(const float v)
	{
		_m15 = v;
	}

	float Mat4::m0() const
	{
		return _m0;
	}

	float Mat4::m1() const
	{
		return _m1;
	}

	float Mat4::m2() const
	{
		return _m2;
	}

	float Mat4::m3() const
	{
		return _m3;
	}

	float Mat4::m4() const
	{
		return _m4;
	}

	float Mat4::m5() const
	{
		return _m5;
	}

	float Mat4::m6() const
	{
		return _m6;
	}

	float Mat4::m7() const
	{
		return _m7;
	}

	float Mat4::m8() const
	{
		return _m8;
	}

	float Mat4::m9() const
	{
		return _m9;
	}

	float Mat4::m10() const
	{
		return _m10;
	}

	float Mat4::m11() const
	{
		return _m11;
	}

	float Mat4::m12() const
	{
		return _m12;
	}

	float Mat4::m13() const
	{
		return _m13;
	}

	float Mat4::m14() const
	{
		return _m14;
	}

	float Mat4::m15() const
	{
		return _m15;
	}

	float Mat4::det() const
	{
		float a = _m5 * ((_m10 * _m15) - (_m11 * _m14));
		float b = _m6 * ((_m9 * _m15) - (_m11 * _m13));
		float c = _m7 * ((_m9 * _m14) - (_m10 * _m13));

		float aa = _m0 * (a - b + c);

		float d = _m4 * ((_m10 * _m15) - (_m11 * _m14));
		float e = _m6 * ((_m8 * _m15) - (_m11 * _m12));
		float f = _m7 * ((_m8 * _m14) - (_m10 * _m12));

		float bb = _m1 * (d - e + f);

		float g = _m4 * ((_m9 * _m15) - (_m11 * _m13));
		float h = _m5 * ((_m8 * _m15) - (_m11 * _m12));
		float i = _m7 * ((_m8 * _m13) - (_m9 * _m12));

		float cc = _m2 * (g - h + i);

		float j = _m4 * ((_m9 * _m14) - (_m10 * _m13));
		float k = _m5 * ((_m8 * _m14) - (_m10 * _m12));
		float l = _m6 * ((_m8 * _m13) - (_m9 * _m12));

		float dd = _m3 * (j - k + l);

		return aa - bb + cc - dd;
	}

	Mat4& Mat4::T(void)
	{
		float tmp = _m1; _m1 = _m4; _m4 = tmp;
		tmp = _m2; _m2 = _m8; _m8 = tmp;
		tmp = _m3; _m3 = _m12; _m12 = tmp;

		tmp = _m6; _m6 = _m9; _m9 = tmp;
		tmp = _m7; _m7 = _m13; _m13 = tmp;

		tmp = _m11; _m11 = _m14; _m14 = tmp;

		return *this;
	}

	Mat4 Mat4::getT(void) const
	{
		return Mat4(
			Vec4(_m0, _m4, _m8, _m12),
			Vec4(_m1, _m5, _m9, _m13),
			Vec4(_m2, _m6, _m10, _m14),
			Vec4(_m3, _m7, _m11, _m15)
		);
	}

	Mat4 Mat4::getInv(void) const
	{
		auto m = Mat4();

		m._m0 = (_m6 * _m11 * _m13) - (_m7 * _m10 * _m13) + (_m7 * _m9 * _m14) - (_m5 * _m11 * _m14) - (_m6 * _m9 * _m15) + (_m5 * _m10 * _m15);
		m._m1 = (_m3 * _m10 * _m13) - (_m2 * _m11 * _m13) - (_m3 * _m9 * _m14) + (_m1 * _m11 * _m14) + (_m2 * _m9 * _m15) - (_m1 * _m10 * _m15);
		m._m2 = (_m2 * _m7 * _m13) - (_m3 * _m6 * _m13) + (_m3 * _m5 * _m14) - (_m1 * _m7 * _m14) - (_m2 * _m5 * _m15) + (_m1 * _m6 * _m15);
		m._m3 = (_m3 * _m6 * _m9) - (_m2 * _m7 * _m9) - (_m3 * _m5 * _m10) + (_m1 * _m7 * _m10) + (_m2 * _m5 * _m11) - (_m1 * _m6 * _m11);

		m._m4 = (_m7 * _m10 * _m12) - (_m6 * _m11 * _m12) - (_m7 * _m8 * _m14) + (_m4 * _m11 * _m14) + (_m6 * _m8 * _m15) - (_m4 * _m10 * _m15);
		m._m5 = (_m2 * _m11 * _m12) - (_m3 * _m10 * _m12) + (_m3 * _m8 * _m14) - (_m0 * _m11 * _m14) - (_m2 * _m8 * _m15) + (_m0 * _m10 * _m15);
		m._m6 = (_m3 * _m6 * _m12) - (_m2 * _m7 * _m12) - (_m3 * _m4 * _m14) + (_m0 * _m7 * _m14) + (_m2 * _m4 * _m15) - (_m0 * _m6 * _m15);
		m._m7 = (_m2 * _m7 * _m8) - (_m3 * _m6 * _m8) + (_m3 * _m4 * _m10) - (_m0 * _m7 * _m10) - (_m2 * _m4 * _m11) + (_m0 * _m6 * _m11);

		m._m8 = (_m5 * _m11 * _m12) - (_m7 * _m9 * _m12) + (_m7 * _m8 * _m13) - (_m4 * _m11 * _m13) - (_m5 * _m8 * _m15) + (_m4 * _m9 * _m15);
		m._m9 = (_m3 * _m9 * _m12) - (_m1 * _m11 * _m12) - (_m3 * _m8 * _m13) + (_m0 * _m11 * _m13) + (_m1 * _m8 * _m15) - (_m0 * _m9 * _m15);
		m._m10 = (_m1 * _m7 * _m12) - (_m3 * _m5 * _m12) + (_m3 * _m4 * _m13) - (_m0 * _m7 * _m13) - (_m1 * _m4 * _m15) + (_m0 * _m5 * _m15);
		m._m11 = (_m3 * _m5 * _m8) - (_m1 * _m7 * _m8) - (_m3 * _m4 * _m9) + (_m0 * _m7 * _m9) + (_m1 * _m4 * _m11) - (_m0 * _m5 * _m11);

		m._m12 = (_m6 * _m9 * _m12) - (_m5 * _m10 * _m12) - (_m6 * _m8 * _m13) + (_m4 * _m10 * _m13) + (_m5 * _m8 * _m14) - (_m4 * _m9 * _m14);
		m._m13 = (_m1 * _m10 * _m12) - (_m2 * _m9 * _m12) + (_m2 * _m8 * _m13) - (_m0 * _m10 * _m13) - (_m1 * _m8 * _m14) + (_m0 * _m9 * _m14);
		m._m14 = (_m2 * _m5 * _m12) - (_m1 * _m6 * _m12) - (_m2 * _m4 * _m13) + (_m0 * _m6 * _m13) + (_m1 * _m4 * _m14) - (_m0 * _m5 * _m14);
		m._m15 = (_m1 * _m6 * _m8) - (_m2 * _m5 * _m8) + (_m2 * _m4 * _m9) - (_m0 * _m6 * _m9) - (_m1 * _m4 * _m10) + (_m0 * _m5 * _m10);

		float invDet = MATH_ONE / det();

		return invDet * m;
	}

	Mat4& Mat4::inv(void)
	{
		*this = getInv();
		return *this;
	}

	bool Mat4::isEqual(const Mat4& A, const float epsilon) const
	{
		return _v0.isEqual(A._v0, epsilon) &&
			_v1.isEqual(A._v1, epsilon) &&
			_v2.isEqual(A._v2, epsilon) &&
			_v3.isEqual(A._v3, epsilon);
	}

	bool Mat4::isIdentity(const float epsilon) const
	{
		return Util::isOne(_m0, epsilon) && Util::isZero(_m1, epsilon) && Util::isZero(_m2, epsilon) && Util::isZero(_m3, epsilon) &&
			Util::isZero(_m4, epsilon) && Util::isOne(_m5, epsilon) && Util::isZero(_m6, epsilon) && Util::isZero(_m7, epsilon) &&
			Util::isZero(_m8, epsilon) && Util::isZero(_m9, epsilon) && Util::isOne(_m10, epsilon) && Util::isZero(_m11, epsilon) &&
			Util::isZero(_m12, epsilon) && Util::isZero(_m13, epsilon) && Util::isZero(_m14, epsilon) && Util::isOne(_m15, epsilon);
	}

	Mat4 Mat4::operator*(const float s) const
	{
		return Mat4(
			_v0 * s,
			_v1 * s,
			_v2 * s,
			_v3 * s
		);
	}

	void Mat4::operator*=(const float scale)
	{
		_v0 *= scale;
		_v1 *= scale;
		_v2 *= scale;
		_v3 *= scale;
	}

	Mat4 operator*(const float scale, const Mat4& A)
	{
		return A * scale;
	}
}

// ---  End of File ---