//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Rot::Rot()
		: Mat4(Special::Identity)
	{
		privSetRotHint();
	}

	Rot::Rot(const Rot1 type, const float angle)
	{
		set(type, angle);
	}

	Rot::Rot(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		set(mode, angle_0, angle_1, angle_2);
	}

	Rot::Rot(const Axis mode, const Vec3& vAxis, const float angle_radians)
	{
		set(mode, vAxis, angle_radians);
	}

	Rot::Rot(const Orient type, const Vec3& dof, const Vec3& up)
	{
		set(type, dof, up);
	}

	void Rot::set(const Rot1 type, const float angle)
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

		privSetRotHint();
	}

	void Rot::set(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		Rot xRot(Rot1::X, angle_0);
		Rot yRot(Rot1::Y, angle_1);
		Rot zRot(Rot1::Z, angle_2);

		switch (mode)
		{
		case Rot3::XYZ:
			*this = Rot(xRot * yRot * zRot);
			break;
		case Rot3::XZY:
			*this = Rot(xRot * zRot * yRot);
			break;
		case Rot3::YXZ:
			*this = Rot(yRot * xRot * zRot);
			break;
		case Rot3::YZX:
			*this = Rot(yRot * zRot * xRot);
			break;
		case Rot3::ZXY:
			*this = Rot(zRot * xRot * yRot);
			break;
		case Rot3::ZYX:
			*this = Rot(zRot * yRot * xRot);
			break;
		default:
			assert(false);
			break;
		}

		privSetRotHint();
	}

	void Rot::set(const Axis mode, const Vec3& vAxis, const float angle_radians)
	{
		static_cast<void>(mode);

		const float angle_a = 0.5f * angle_radians;
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

		privSetRotHint();
	}

	void Rot::set(const Orient mode, const Vec3& dof, const Vec3& up)
	{
		assert(dof.dot(up) != 1.0f);

		const Vec3 rz = dof.getNorm();

		Vec3 rx = up.cross(rz);
		rx.norm();

		Vec3 ry = rz.cross(rx);
		ry.norm();

		Mat4::set(Vec4(rx, 0), Vec4(ry, 0), Vec4(rz, 0), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		_m3 = 0.0f;
		_m7 = 0.0f;
		_m11 = 0.0f;
		_m15 = 1.0f;

		if (mode == Orient::WorldToLocal)
		{
			inv();
		}

		privSetRotHint();
	}

	Rot& Rot::operator=(const Quat& q)
	{
		set(q);
		return *this;
	}

	Rot::Rot(const Mat4& base)
		: Mat4(base)
	{
		privSetRotHint();
	}

	Rot::Rot(const Quat& q)
	{
		set(q);
	}

	Rot::Rot(const Special val)
	{
		set(val);
	}

	void Rot::set(const Quat& q)
	{
		*this = Rot(Mat4(q));
	}

	void Rot::set(const Special val)
	{
		Mat4::set(val);
	}

	Mat4 Rot::operator*(const Mat4& A) const
	{
		return static_cast<Mat4>(*this) * A;
	}

	Rot Rot::operator*(const Quat& A) const
	{
		return Rot(static_cast<Mat4>(*this) * Mat4(A));
	}

	Rot& Rot::operator*=(const Quat& A)
	{
		*this = Rot(static_cast<Mat4>(*this) * static_cast<Mat4>(A));
		return *this;
	}

	Mat4 Rot::operator*(const Scale& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}

	Rot Rot::operator*(const Rot& A) const
	{
		return Rot(static_cast<Mat4>(*this) * static_cast<Mat4>(A));
	}

	Rot& Rot::operator*=(const Rot& A)
	{
		*this = Rot(static_cast<Mat4>(*this) * static_cast<Mat4>(A));
		return *this;
	}

	Mat4 Rot::operator*(const Trans& A) const
	{
		return static_cast<Mat4>(*this) * static_cast<Mat4>(A);
	}
}

// ---  End of File ---