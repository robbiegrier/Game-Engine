//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Quat::Quat(void)
		: _qx(MATH_ZERO), _qy(MATH_ZERO), _qz(MATH_ZERO), _qw(MATH_ONE)
	{
	}

	Quat::Quat(const Quat& qIn)
		: _qx(qIn._qx), _qy(qIn._qy), _qz(qIn._qz), _qw(qIn._qw)
	{
	}

	Quat::Quat(const Vec3& vect, const float real)
	{
		set(vect, real);
	}

	Quat::Quat(const float vx, const float vy, const float vz, const float real)
	{
		set(vx, vy, vz, real);
	}

	Quat::Quat(const Rot& mIn)
	{
		set(mIn);
	}

	Quat::Quat(const Special value)
		: Quat(Rot(value))
	{
	}

	Quat::Quat(const Rot1 type, const float angle)
		: Quat(Rot(type, angle))
	{
	}

	Quat::Quat(const Rot3 type, const float angle_x, const float angle_y, const float angle_z)
		: Quat(Rot(type, angle_x, angle_y, angle_z))
	{
	}

	Quat::Quat(const Axis type, const Vec3& vAxis, const float angle_radians)
		: Quat(Rot(type, vAxis, angle_radians))
	{
	}

	Quat::Quat(const Orient orient, const Vec3& dof, const Vec3& up)
		: Quat(Rot(orient, dof, up))
	{
	}

	Quat::~Quat(void)
	{
	}

	void Quat::set(const Special type)
	{
		switch (type)
		{
		case Special::Zero:
			set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ZERO);
			break;
		case Special::Identity:
			set(MATH_ZERO, MATH_ZERO, MATH_ZERO, MATH_ONE);
			break;
		default:
			break;
		}
	}

	void Quat::set(const Rot1 type, const float angle)
	{
		set(Rot(type, angle));
	}

	void Quat::set(const Rot3 type, const float angle_x, const float angle_y, const float angle_z)
	{
		set(Rot(type, angle_x, angle_y, angle_z));
	}

	void Quat::set(const Axis type, const Vec3& vAxis, const float angle_radians)
	{
		set(Rot(type, vAxis, angle_radians));
	}

	void Quat::set(const Orient orient, const Vec3& dof, const Vec3& up)
	{
		set(Rot(orient, dof, up));
	}

	void Quat::set(const float vx, const float vy, const float vz, const float real)
	{
		_qx = vx;
		_qy = vy;
		_qz = vz;
		_qw = real;
	}

	float& Quat::operator[](const x_enum)
	{
		return _qx;
	}

	float& Quat::operator[](const y_enum)
	{
		return _qy;
	}

	float& Quat::operator[](const z_enum)
	{
		return _qz;
	}

	float& Quat::operator[](const w_enum)
	{
		return _qw;
	}

	void Quat::qx(float val)
	{
		_qx = val;
	}

	void Quat::qy(float val)
	{
		_qy = val;
	}

	void Quat::qz(float val)
	{
		_qz = val;
	}

	void Quat::real(float val)
	{
		_qw = val;
	}

	void Quat::setVec3(const Vec3& vect)
	{
		_qV3 = vect;
	}

	void Quat::set(const Vec3& vect, const float real)
	{
		_qV3 = vect;
		_qw = real;
	}

	void Quat::set(const Mat4& mIn)
	{
		float tr = mIn._m0 + mIn._m5 + mIn._m10;
		float S;

		if (tr > 0.0f)
		{
			S = 2 * sqrtf(tr + 1.0f);
			_qw = 0.25f * S;
			_qx = -(mIn._m9 - mIn._m6) / S;
			_qy = -(mIn._m2 - mIn._m8) / S;
			_qz = -(mIn._m4 - mIn._m1) / S;
		}
		else if ((mIn._m0 > mIn._m5) && (mIn._m0 > mIn._m10))
		{
			S = 2 * sqrtf(1.0f + mIn._m0 - mIn._m5 - mIn._m10);
			_qw = -(mIn._m9 - mIn._m6) / S;
			_qx = 0.25f * S;
			_qy = (mIn._m1 + mIn._m4) / S;
			_qz = (mIn._m2 + mIn._m8) / S;
		}
		else if (mIn._m5 > mIn._m10)
		{
			S = 2 * sqrtf(1.0f + mIn._m5 - mIn._m0 - mIn._m10);
			_qw = -(mIn._m2 - mIn._m8) / S;
			_qx = (mIn._m1 + mIn._m4) / S;
			_qy = 0.25f * S;
			_qz = (mIn._m6 + mIn._m9) / S;
		}
		else
		{
			S = 2 * sqrtf(1.0f + mIn._m10 - mIn._m0 - mIn._m5);
			_qw = -(mIn._m4 - mIn._m1) / S;
			_qx = (mIn._m2 + mIn._m8) / S;
			_qy = (mIn._m6 + mIn._m9) / S;
			_qz = 0.25f * S;
		}
	}

	float Quat::operator[](const x_enum) const
	{
		return _qx;
	}

	float Quat::operator[](const y_enum) const
	{
		return _qy;
	}

	float Quat::operator[](const z_enum) const
	{
		return _qz;
	}

	float Quat::operator[](const w_enum) const
	{
		return _qw;
	}

	float Quat::qx() const
	{
		return _qx;
	}

	float Quat::qy() const
	{
		return _qy;
	}

	float Quat::qz() const
	{
		return _qz;
	}

	float Quat::real() const
	{
		return _qw;
	}

	float Quat::getAngle(void) const
	{
		return 2.f * Trig::acos(_qw);
	}

	void Quat::getVec3(Vec3& vOut) const
	{
		vOut.set(_qV3);
	}

	void Quat::getAxis(Vec3& vOut) const
	{
		vOut = _qV3.getNorm();
	}

	float Quat::dot(const Quat& qin) const
	{
		return _qV4.dot(qin._qV4);
	}

	float Quat::mag(void) const
	{
		return Trig::sqrt(magSquared());
	}

	float Quat::magSquared(void) const
	{
		return (_qx * _qx) + (_qy * _qy) + (_qz * _qz) + (_qw * _qw);
	}

	float Quat::invMag(void) const
	{
		return MATH_ONE / mag();
	}

	Quat& Quat::conj(void)
	{
		_qV3.set(-_qx, -_qy, -_qz);
		return *this;
	}

	Quat Quat::getConj(void) const
	{
		return Quat(-_qV3, _qw);
	}

	Quat& Quat::inv(void)
	{
		*this = getConj() / magSquared();
		return *this;
	}

	Quat Quat::getInv(void) const
	{
		return getConj() / magSquared();
	}

	Quat& Quat::norm(void)
	{
		const float len = mag();
		_qx /= len;
		_qy /= len;
		_qz /= len;
		_qw /= len;
		return *this;
	}

	Quat Quat::getNorm(void) const
	{
		Quat output = *this;
		output.norm();
		return output;
	}

	void Quat::Lqvqc(const Vec3& vIn, Vec3& vOut) const
	{
		Vec3 a = (2.f * _qw) * (vIn.cross(_qV3));
		Vec3 b = vIn * ((_qw * _qw) - _qV3.dot(_qV3));
		Vec3 c = 2.f * vIn.dot(_qV3) * _qV3;
		vOut = a + b + c;
	}

	void Quat::Lqcvq(const Vec3& vIn, Vec3& vOut) const
	{
		Vec3 a = (2.f * _qw) * (_qV3.cross(vIn));
		Vec3 b = vIn * ((_qw * _qw) - _qV3.dot(_qV3));
		Vec3 c = 2.f * vIn.dot(_qV3) * _qV3;
		vOut = a + b + c;
	}

	bool Quat::isEqual(const Quat& qin, const float epsilon) const
	{
		return Util::isEqual(_qx, qin._qx, epsilon) &&
			Util::isEqual(_qy, qin._qy, epsilon) &&
			Util::isEqual(_qz, qin._qz, epsilon) &&
			Util::isEqual(_qw, qin._qw, epsilon);
	}

	bool Quat::isEquivalent(const Quat& qin, const float epsilon) const
	{
		return isEqual(qin, epsilon) || isEqual(-qin, epsilon);
	}

	bool Quat::isNegEqual(const Quat& qin, const float epsilon) const
	{
		return isEqual(-qin, epsilon);
	}

	bool Quat::isConjugateEqual(const Quat& qin, const float epsilon) const
	{
		return qin.isEqual(getConj(), epsilon);
	}

	bool Quat::isIdentity(const float epsilon) const
	{
		return Util::isEqual(_qx, MATH_ZERO, epsilon) &&
			Util::isEqual(_qy, MATH_ZERO, epsilon) &&
			Util::isEqual(_qz, MATH_ZERO, epsilon) &&
			Util::isEqual(_qw, MATH_ONE, epsilon);
	}

	bool Quat::isNormalized(const float epsilon) const
	{
		return Util::isEqual(magSquared(), MATH_ONE, epsilon);
	}

	bool Quat::isZero(const float epsilon) const
	{
		return Util::isEqual(_qx, MATH_ZERO, epsilon) &&
			Util::isEqual(_qy, MATH_ZERO, epsilon) &&
			Util::isEqual(_qz, MATH_ZERO, epsilon) &&
			Util::isEqual(_qw, MATH_ZERO, epsilon);
	}

	Quat& Quat::operator=(const Quat& q)
	{
		_qx = q._qx;
		_qy = q._qy;
		_qz = q._qz;
		_qw = q._qw;
		return *this;
	}

	Quat& Quat::operator=(const Rot& m)
	{
		set(m);
		return *this;
	}

	Quat Quat::operator+(void) const
	{
		return *this;
	}

	Quat Quat::operator+(const Quat& q) const
	{
		return Quat(_qx + q._qx, _qy + q._qy, _qz + q._qz, _qw + q._qw);
	}

	Quat& Quat::operator+=(const Quat& q)
	{
		_qx += q._qx;
		_qy += q._qy;
		_qz += q._qz;
		_qw += q._qw;
		return *this;
	}

	Quat Quat::operator+(const float a) const
	{
		return Quat(_qx + a, _qy + a, _qz + a, _qw + a);
	}

	Quat& Quat::operator+=(const float a)
	{
		_qx += a;
		_qy += a;
		_qz += a;
		_qw += a;
		return *this;
	}

	Quat Quat::operator-(void) const
	{
		return Quat(-_qx, -_qy, -_qz, -_qw);
	}

	Quat Quat::operator-(const Quat& q) const
	{
		return Quat(_qx - q._qx, _qy - q._qy, _qz - q._qz, _qw - q._qw);
	}

	Quat& Quat::operator-=(const Quat& q)
	{
		_qx -= q._qx;
		_qy -= q._qy;
		_qz -= q._qz;
		_qw -= q._qw;
		return *this;
	}

	Quat Quat::operator-(const float a) const
	{
		return Quat(_qx - a, _qy - a, _qz - a, _qw - a);
	}

	Quat& Quat::operator-=(const float a)
	{
		_qx -= a;
		_qy -= a;
		_qz -= a;
		_qw -= a;
		return *this;
	}

	Quat Quat::operator*(const float a) const
	{
		return Quat(_qx * a, _qy * a, _qz * a, _qw * a);
	}

	Quat& Quat::operator*=(const float a)
	{
		_qx *= a;
		_qy *= a;
		_qz *= a;
		_qw *= a;
		return *this;
	}

	Mat4 Quat::operator*(const Mat4& m) const
	{
		return Mat4(*this) * m;
	}

	Quat Quat::operator*(const Quat& q) const
	{
		const Vec3 a = q._qV3.cross(_qV3);
		const Vec3 b = q._qw * _qV3;
		const Vec3 c = _qw * q._qV3;
		const float d = _qw * q._qw;
		const float e = _qV3.dot(q._qV3);
		return Quat(a + b + c, d - e);
	}

	Quat& Quat::operator*=(const Quat& q)
	{
		Quat tmp = *this * q;
		*this = tmp;
		return *this;
	}

	Mat4 Quat::operator*(const Scale& m) const
	{
		return Mat4(*this) * m;
	}

	Quat& Quat::operator*=(const Rot& m)
	{
		*this *= Quat(m);
		return *this;
	}

	Rot Quat::operator*(const Rot& m) const
	{
		return Rot(*this) * m;
	}

	Mat4 Quat::operator*(const Trans& m) const
	{
		return Mat4(*this) * m;
	}

	Quat Quat::operator/(const Quat& q) const
	{
		return Quat(_qx / q._qx, _qy / q._qy, _qz / q._qz, _qw / q._qw);
	}

	Quat& Quat::operator/=(const Quat& q)
	{
		_qx /= q._qx;
		_qy /= q._qy;
		_qz /= q._qz;
		_qw /= q._qw;
		return *this;
	}

	Quat Quat::operator/(const float a) const
	{
		return Quat(_qx / a, _qy / a, _qz / a, _qw / a);
	}

	Quat& Quat::operator/=(const float a)
	{
		_qx /= a;
		_qy /= a;
		_qz /= a;
		_qw /= a;
		return *this;
	}

	void Quat::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}

	Quat operator+(const float a, const Quat& q)
	{
		return Quat(q._qx + a, q._qy + a, q._qz + a, q._qw + a);
	}

	Quat operator-(const float a, const Quat& q)
	{
		return Quat(a - q._qx, a - q._qy, a - q._qz, a - q._qw);
	}

	Quat operator*(const float a, const Quat& q)
	{
		return Quat(q._qx * a, q._qy * a, q._qz * a, q._qw * a);
	}

	Quat operator/(const float a, const Quat& q)
	{
		return Quat(a / q._qx, a / q._qy, a / q._qz, a / q._qw);
	}
}

//--- End of File ---