#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Vec3::Vec3()
		: _vx(MATH_ZERO), _vy(MATH_ZERO), _vz(MATH_ZERO)
	{
	}

	Vec3::Vec3(const Vec3& inV)
		: _mv(inV._mv)
	{
	}

	Vec3& Vec3::operator=(const Vec3& v)
	{
		_vx = v._vx;
		_vy = v._vy;
		_vz = v._vz;
		return *this;
	}

	Vec3::~Vec3()
	{
		// Do Nothing
	}

	Vec3::Vec3(const float in_x, const float in_y, const float in_z)
		: _vx(in_x), _vy(in_y), _vz(in_z)
	{
	}

	Vec3::Vec3(const Vec4& v)
		: _vx(v._vx), _vy(v._vy), _vz(v._vz)
	{
	}

	Vec3& Vec3::operator=(const Vec4& v)
	{
		_vx = v._vx;
		_vy = v._vy;
		_vz = v._vz;
		return *this;
	}

	float& Vec3::operator[] (const enum x_enum)
	{
		return _vx;
	}

	float& Vec3::operator[] (const enum y_enum)
	{
		return _vy;
	}

	float& Vec3::operator[] (const enum z_enum)
	{
		return _vz;
	}

	float Vec3::operator[](const x_enum) const
	{
		return _vx;
	}

	float Vec3::operator[](const y_enum) const
	{
		return _vy;
	}

	float Vec3::operator[](const z_enum) const
	{
		return _vz;
	}

	void Vec3::x(const float v)
	{
		_vx = v;
	}

	void Vec3::y(const float v)
	{
		_vy = v;
	}

	void Vec3::z(const float v)
	{
		_vz = v;
	}

	float Vec3::x() const
	{
		return _vx;
	}

	float Vec3::y() const
	{
		return _vy;
	}

	float Vec3::z() const
	{
		return _vz;
	}

	Vec3 Vec3::operator+(void) const
	{
		return Vec3(*this);
	}

	Vec3 Vec3::operator+(const Vec3& inV) const
	{
		return Vec3(_vx + inV._vx, _vy + inV._vy, _vz + inV._vz);
	}

	void Vec3::operator+=(const Vec3& inV)
	{
		_vx += inV._vx;
		_vy += inV._vy;
		_vz += inV._vz;
	}

	Vec3 Vec3::operator-(void) const
	{
		return Vec3(-_vx, -_vy, -_vz);
	}

	Vec3 Vec3::operator-(const Vec3& inV) const
	{
		return Vec3(_vx - inV._vx, _vy - inV._vy, _vz - inV._vz);
	}

	void Vec3::operator-=(const Vec3& inV)
	{
		_vx -= inV._vx;
		_vy -= inV._vy;
		_vz -= inV._vz;
	}

	Vec3 Vec3::operator*(const float scale) const
	{
		return Vec3(_vx * scale, _vy * scale, _vz * scale);
	}

	void Vec3::operator*=(const float scale)
	{
		_vx *= scale;
		_vy *= scale;
		_vz *= scale;
	}

	Vec3 operator*(const float scale, const Vec3& inV)
	{
		return Vec3(inV._vx * scale, inV._vy * scale, inV._vz * scale);
	}

	Vec3 Vec3::operator*(const Mat3& m) const
	{
		return Vec3(
			(_vx * m._m0) + (_vy * m._m4) + (_vz * m._m8),
			(_vx * m._m1) + (_vy * m._m5) + (_vz * m._m9),
			(_vx * m._m2) + (_vy * m._m6) + (_vz * m._m10)
		);
	}

	Vec3 Vec3::operator*=(const Mat3& m)
	{
		float xDot = (_vx * m._m0) + (_vy * m._m4) + (_vz * m._m8);
		float yDot = (_vx * m._m1) + (_vy * m._m5) + (_vz * m._m9);
		float zDot = (_vx * m._m2) + (_vy * m._m6) + (_vz * m._m10);

		_vx = xDot;
		_vy = yDot;
		_vz = zDot;
		return *this;
	}

	Vec3& Vec3::norm(void)
	{
		float length = len();

		if (Util::isNonZero(length, MATH_TOLERANCE))
		{
			_vx /= length;
			_vy /= length;
			_vz /= length;
		}

		return *this;
	}

	Vec3 Vec3::getNorm(void) const
	{
		auto output = Vec3();
		float length = len();

		if (Util::isNonZero(length, MATH_TOLERANCE))
		{
			output._vx = _vx / length;
			output._vy = _vy / length;
			output._vz = _vz / length;
		}

		return output;
	}

	float Vec3::dot(const Vec3& vIn) const
	{
		return (_vx * vIn._vx) + (_vy * vIn._vy) + (_vz * vIn._vz);
	}

	Vec3 Vec3::cross(const Vec3& vIn) const
	{
		return Vec3(
			(_vy * vIn._vz) - (_vz * vIn._vy),
			(_vz * vIn._vx) - (_vx * vIn._vz),
			(_vx * vIn._vy) - (_vy * vIn._vx)
		);
	}

	Vec3Proxy Vec3::len(void) const
	{
		return Vec3Proxy(_vx, _vy, _vz);
	}

	float Vec3::getAngle(const Vec3& vIn) const
	{
		float totalLength = len() * vIn.len();

		if (Util::isNonZero(totalLength, MATH_TOLERANCE))
		{
			return Trig::acos(dot(vIn) / totalLength);
		}

		return MATH_ZERO;
	}

	void Vec3::set(const float inX, const float inY, const float inZ)
	{
		_vx = inX;
		_vy = inY;
		_vz = inZ;
	}

	void Vec3::set(const Vec3& A)
	{
		_vx = A._vx;
		_vy = A._vy;
		_vz = A._vz;
	}

	void Vec3::set(const Vec4& A)
	{
		_vx = A._vx;
		_vy = A._vy;
		_vz = A._vz;
	}

	bool Vec3::isEqual(const Vec3& v, const float epsilon) const
	{
		return Util::isEqual(_vx, v._vx, epsilon) &&
			Util::isEqual(_vy, v._vy, epsilon) &&
			Util::isEqual(_vz, v._vz, epsilon);
	}

	bool Vec3::isZero(const float epsilon) const
	{
		return Util::isZero(_vx, epsilon) &&
			Util::isZero(_vy, epsilon) &&
			Util::isZero(_vz, epsilon);
	}

	void Vec3::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}
}