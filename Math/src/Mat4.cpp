//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Mat4::Mat4()
		: _v0(0.0f, 0.0f, 0.0f, 0.0f),
		_v1(0.0f, 0.0f, 0.0f, 0.0f),
		_v2(0.0f, 0.0f, 0.0f, 0.0f),
		_v3(0.0f, 0.0f, 0.0f, 0.0f)
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
		: _v0(tM._v0), _v1(tM._v1), _v2(tM._v2), _v3(tM._v3)
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

	Mat4::Mat4(const Special type)
	{
		set(type);
	}

	Mat4::Mat4(const Quat& q)
	{
		set(q);
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

	void Mat4::set(const Row4 type, const Vec4& V)
	{
		_rows[static_cast<int>(type)].set(V);
	}

	void Mat4::set(const Quat& q)
	{
		_m0 = 1.f - (2.f * ((q._qy * q._qy) + (q._qz * q._qz)));
		_m1 = 2.f * ((q._qx * q._qy) + (q._qw * q._qz));
		_m2 = 2.f * ((q._qx * q._qz) - (q._qw * q._qy));
		_m3 = MATH_ZERO;
		_m4 = 2.f * ((q._qx * q._qy) - (q._qw * q._qz));
		_m5 = 1.f - (2.f * ((q._qx * q._qx) + (q._qz * q._qz)));
		_m6 = 2.f * ((q._qy * q._qz) + (q._qw * q._qx));
		_m7 = MATH_ZERO;
		_m8 = 2.f * ((q._qx * q._qz) + (q._qw * q._qy));
		_m9 = 2.f * ((q._qy * q._qz) - (q._qw * q._qx));
		_m10 = 1.f - (2.f * ((q._qx * q._qx) + (q._qy * q._qy)));
		_m11 = MATH_ZERO;
		_m12 = MATH_ZERO;
		_m13 = MATH_ZERO;
		_m14 = MATH_ZERO;
		_m15 = MATH_ONE;
	}

	Vec4 Mat4::get(const Row4 type) const
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

	Mat4& Mat4::operator+=(const Mat4& A)
	{
		_v0 += A._v0;
		_v1 += A._v1;
		_v2 += A._v2;
		_v3 += A._v3;
		return *this;
	}

	Mat4 Mat4::operator-(void) const
	{
		return Mat4(-_v0, -_v1, -_v2, -_v3);
	}

	Mat4 Mat4::operator-(const Mat4& A) const
	{
		return Mat4(_v0 - A._v0, _v1 - A._v1, _v2 - A._v2, _v3 - A._v3);
	}

	Mat4& Mat4::operator-=(const Mat4& A)
	{
		_v0 -= A._v0;
		_v1 -= A._v1;
		_v2 -= A._v2;
		_v3 -= A._v3;
		return *this;
	}

	Mat4 Mat4::operator*(const Mat4& A) const
	{
		Mat4 output = Mat4(
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

		output._u_m15 |= (_u_m15 & 0b0111u);
		output._u_m15 |= (A._u_m15 & 0b0111u);

		return output;
	}

	Mat4& Mat4::operator*=(const Mat4& A)
	{
		const unsigned int existingHint = _u_m15;
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

		_u_m15 |= (existingHint & 0b0111u);
		_u_m15 |= (A._u_m15 & 0b0111u);

		return *this;
	}

	Mat4 Mat4::operator*(const Quat& q) const
	{
		return *this * Mat4(q);
	}

	Mat4& Mat4::operator*=(const Quat& q)
	{
		*this = *this * q;
		return *this;
	}

	Mat4 Mat4::operator*(const Scale& A) const
	{
		Mat4 output = Mat4(
			Vec4(_m0 * A._m0, _m1 * A._m5, _m2 * A._m10, _m3 * A._m15),
			Vec4(_m4 * A._m0, _m5 * A._m5, _m6 * A._m10, _m7 * A._m15),
			Vec4(_m8 * A._m0, _m9 * A._m5, _m10 * A._m10, _m11 * A._m15),
			Vec4(_m12 * A._m0, _m13 * A._m5, _m14 * A._m10, _m15 * A._m15)
		);

		output._u_m15 |= (_u_m15 & 0b0111u);
		output._u_m15 |= (A._u_m15 & 0b0111u);

		return output;
	}

	Mat4& Mat4::operator*=(const Scale& A)
	{
		const unsigned int existingHint = _u_m15;
		const float tmp0 = _m0 * A._m0;
		const float tmp1 = _m1 * A._m5;
		const float tmp2 = _m2 * A._m10;
		const float tmp3 = _m3 * A._m15;
		const float tmp4 = _m4 * A._m0;
		const float tmp5 = _m5 * A._m5;
		const float tmp6 = _m6 * A._m10;
		const float tmp7 = _m7 * A._m15;
		const float tmp8 = _m8 * A._m0;
		const float tmp9 = _m9 * A._m5;
		const float tmp10 = _m10 * A._m10;
		const float tmp11 = _m11 * A._m15;
		const float tmp12 = _m12 * A._m0;
		const float tmp13 = _m13 * A._m5;
		const float tmp14 = _m14 * A._m10;
		const float tmp15 = _m15 * A._m15;

		_v0.set(tmp0, tmp1, tmp2, tmp3);
		_v1.set(tmp4, tmp5, tmp6, tmp7);
		_v2.set(tmp8, tmp9, tmp10, tmp11);
		_v3.set(tmp12, tmp13, tmp14, tmp15);

		_u_m15 |= (existingHint & 0b0111u);
		_u_m15 |= (A._u_m15 & 0b0111u);

		return *this;
	}

	Mat4 Mat4::operator*(const Rot& A) const
	{
		return *this * static_cast<Mat4>(A);
	}

	Mat4& Mat4::operator*=(const Rot& A)
	{
		*this *= static_cast<Mat4>(A);
		return *this;
	}

	Mat4 Mat4::operator*(const Trans& A) const
	{
		return *this * static_cast<Mat4>(A);
	}

	Mat4& Mat4::operator*=(const Trans& A)
	{
		*this *= static_cast<Mat4>(A);
		return *this;
	}

	void Mat4::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}

	Mat4Proxy Mat4::operator[] (const enum m0_enum) { return Mat4Proxy(*this, _m0); }
	Mat4Proxy Mat4::operator[] (const enum m1_enum) { return Mat4Proxy(*this, _m1); }
	Mat4Proxy Mat4::operator[] (const enum m2_enum) { return Mat4Proxy(*this, _m2); }
	Mat4Proxy Mat4::operator[] (const enum m3_enum) { return Mat4Proxy(*this, _m3); }
	Mat4Proxy Mat4::operator[] (const enum m4_enum) { return Mat4Proxy(*this, _m4); }
	Mat4Proxy Mat4::operator[] (const enum m5_enum) { return Mat4Proxy(*this, _m5); }
	Mat4Proxy Mat4::operator[] (const enum m6_enum) { return Mat4Proxy(*this, _m6); }
	Mat4Proxy Mat4::operator[] (const enum m7_enum) { return Mat4Proxy(*this, _m7); }
	Mat4Proxy Mat4::operator[] (const enum m8_enum) { return Mat4Proxy(*this, _m8); }
	Mat4Proxy Mat4::operator[] (const enum m9_enum) { return Mat4Proxy(*this, _m9); }
	Mat4Proxy Mat4::operator[] (const enum m10_enum) { return Mat4Proxy(*this, _m10); }
	Mat4Proxy Mat4::operator[] (const enum m11_enum) { return Mat4Proxy(*this, _m11); }
	Mat4Proxy Mat4::operator[] (const enum m12_enum) { return Mat4Proxy(*this, _m12); }
	Mat4Proxy Mat4::operator[] (const enum m13_enum) { return Mat4Proxy(*this, _m13); }
	Mat4Proxy Mat4::operator[] (const enum m14_enum) { return Mat4Proxy(*this, _m14); }
	Mat4Proxy Mat4::operator[] (const enum m15_enum) { return Mat4Proxy(*this, _m15); }

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
		const float A = (_m10 * _m15) - (_m14 * _m11);
		const float B = (_m8 * _m15) - (_m12 * _m11);
		const float C = (_m8 * _m13) - (_m12 * _m9);
		const float D = (_m8 * _m14) - (_m12 * _m10);
		const float E = (_m9 * _m15) - (_m13 * _m11);
		const float F = (_m9 * _m14) - (_m13 * _m10);

		return (_m0 * ((_m5 * A) - (_m6 * E) + (_m7 * F))
			- _m1 * ((_m4 * A) - (_m6 * B) + (_m7 * D))
			+ _m2 * ((_m4 * E) - (_m5 * B) + (_m7 * C))
			- _m3 * ((_m4 * F) - (_m5 * D) + (_m6 * C)));
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
		Mat4 output = *this;
		return output.T();
	}

	Mat4 Mat4::getInv(void) const
	{
		switch (privGetHint())
		{
		case Hint::Generalize:
		{
			// 4x4 General Matrix
			const float invDet = MATH_ONE / det();
			Mat4 m = privGetAdj();
			m *= invDet;

			m.privSetGeneralHint();
			return m;
		}
		case Hint::Rot:
		{
			// Rot transpose as inverse
			Mat4 m = *this;
			reinterpret_cast<Mat3&>(m).T();

			m.privSetRotHint();
			return m;
		}
		case Hint::Trans:
		{
			// -Trans as inverse
			Mat4 m = *this;
			m._m12 = -_m12;
			m._m13 = -_m13;
			m._m14 = -_m14;

			m.privSetTransHint();
			return m;
		}
		case Hint::RotTrans:
		{
			// Rot transpose as A^-1
			Mat4 m = *this;
			reinterpret_cast<Mat3&>(m).T();

			// Minimal -CA^-1
			m._m12 = (-_m12 * m._m0) + (-_m13 * m._m4) + (-_m14 * m._m8);
			m._m13 = (-_m12 * m._m1) + (-_m13 * m._m5) + (-_m14 * m._m9);
			m._m14 = (-_m12 * m._m2) + (-_m13 * m._m6) + (-_m14 * m._m10);

			m.privSetRotHint();
			m.privSetTransHint();
			return m;
		}
		case Hint::Scale:
		{
			// 1/Scale as A^-1
			Mat4 m = *this;
			m._m0 = MATH_ONE / m._m0;
			m._m5 = MATH_ONE / m._m5;
			m._m10 = MATH_ONE / m._m10;

			m.privSetScaleHint();
			return m;
		}
		case Hint::RotScale:
		{
			// A^-1 in-place
			Mat4 m = *this;
			reinterpret_cast<Mat3&>(m).inv();

			m.privSetRotHint();
			m.privSetScaleHint();
			return m;
		}
		case Hint::TransScale:
		{
			// 1/Scale as A^-1
			Mat4 m = *this;
			m._m0 = MATH_ONE / _m0;
			m._m5 = MATH_ONE / _m5;
			m._m10 = MATH_ONE / _m10;

			// Minimal -CA^-1
			m._m12 = (-_m12 * m._m0);
			m._m13 = (-_m13 * m._m5);
			m._m14 = (-_m14 * m._m10);

			m.privSetTransHint();
			m.privSetScaleHint();
			return m;
		}
		case Hint::Affine:
		{
			// A^-1 in-place
			Mat4 m = *this;
			reinterpret_cast<Mat3&>(m).inv();

			// Minimal -CA^-1
			m._m12 = (-_m12 * m._m0) + (-_m13 * m._m4) + (-_m14 * m._m8);
			m._m13 = (-_m12 * m._m1) + (-_m13 * m._m5) + (-_m14 * m._m9);
			m._m14 = (-_m12 * m._m2) + (-_m13 * m._m6) + (-_m14 * m._m10);

			m.privSetRotHint();
			m.privSetTransHint();
			m.privSetScaleHint();
			return m;
		}
		default:
			assert(false);
			break;
		}

		return *this;
	}

	Mat4 Mat4::privGetAdj(void) const
	{
		float A, B, C, D, E, F, G, H, I;
		Mat4 tmp;

		A = (_m10 * _m15) - (_m11 * _m14);
		B = (_m7 * _m14) - (_m6 * _m15);
		C = (_m6 * _m11) - (_m7 * _m10);
		D = (_m3 * _m14) - (_m2 * _m15);
		E = (_m2 * _m11) - (_m3 * _m10);
		F = (_m2 * _m7) - (_m3 * _m6);
		G = _m5; H = _m13;

		tmp._m0 = (G * A) + (_m9 * B) + (H * C);
		tmp._m2 = -(_m1 * B) + (G * D) + (H * F);
		G = _m0; H = _m8;
		tmp._m5 = (G * A) + (H * D) + (_m12 * E);
		tmp._m7 = (G * C) + (H * F) - (_m4 * E);
		A = (_m9 * _m15) - (_m11 * _m13);
		B = (_m3 * _m13) - (_m1 * _m15);
		C = (_m1 * _m11) - (_m3 * _m9);
		D = (_m7 * _m13) - (_m5 * _m15);
		E = (_m5 * _m11) - (_m7 * _m9);
		F = (_m1 * _m7) - (_m3 * _m5);
		G = _m2; H = _m10;
		tmp._m1 = (G * A) + (H * B) + (_m14 * C);
		tmp._m3 = (G * E) + (H * F) - (_m6 * C);
		G = _m4; H = _m12;
		tmp._m8 = (G * A) + (_m8 * D) + (H * E);
		tmp._m10 = -(_m0 * D) + (G * B) + (H * F);
		A = (_m8 * _m15) - (_m11 * _m12);
		B = (_m4 * _m11) - (_m7 * _m8);
		C = (_m3 * _m12) - (_m0 * _m15);
		D = (_m0 * _m7) - (_m3 * _m4);
		E = (_m7 * _m12) - (_m4 * _m15);
		F = (_m0 * _m11) - (_m3 * _m8);
		G = _m6; H = _m14;
		tmp._m4 = (G * A) + (_m10 * E) + (H * B);
		tmp._m6 = -(_m2 * E) + (G * C) + (H * D);
		G = _m1; H = _m9;
		tmp._m9 = (G * A) + (H * C) + (_m13 * F);
		tmp._m11 = (G * B) + (H * D) - (_m5 * F);
		A = _m4; B = _m6; C = _m12; D = _m14;
		E = (B * C) - (A * D);
		F = _m8; G = _m10; H = _m5; I = _m13;
		tmp._m12 = (H * ((F * D) - (G * C))) + (_m9 * E) + (I * ((A * G) - (B * F)));
		F = _m2; G = _m0;
		tmp._m14 = -(_m1 * E) + (H * ((F * C) - (G * D))) + (I * ((G * B) - (F * A)));
		A = _m1; B = _m2; C = _m9; D = _m10;
		E = (A * D) - (B * C);
		F = _m13; G = _m14; H = _m0; I = _m8;
		tmp._m13 = (H * ((C * G) - (D * F))) + (I * ((B * F) - (A * G))) + (_m12 * E);
		F = _m5; G = _m6;
		tmp._m15 = -(_m4 * E) + (H * ((F * D) - (G * C))) + (I * ((A * G) - (B * F)));

		return(tmp);
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

	Mat4& Mat4::operator*=(const float s)
	{
		_v0.set(s * _m0, s * _m1, s * _m2, s * _m3);
		_v1.set(s * _m4, s * _m5, s * _m6, s * _m7);
		_v2.set(s * _m8, s * _m9, s * _m10, s * _m11);
		_v3.set(s * _m12, s * _m13, s * _m14, s * _m15);
		return *this;
	}

	Mat4 operator*(const float scale, const Mat4& A)
	{
		return A * scale;
	}
}

// ---  End of File ---