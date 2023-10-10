#include "MathEngine.h"

namespace Azul
{
	Vec4::Vec4()
		: _vx(MATH_ZERO), _vy(MATH_ZERO), _vz(MATH_ZERO), _vw(MATH_ZERO)
	{
	}

	Vec4::Vec4(const Vec4& inV)
		: _mv(inV._mv)
	{
	}

	Vec4& Vec4::operator=(const Vec4& v)
	{
		_mv = v._mv;
		return *this;
	}

	Vec4::~Vec4()
	{
		// Do Nothing
	}

	Vec4::Vec4(const float in_x, const float in_y, const float in_z, const float in_w)
		: _vx(in_x), _vy(in_y), _vz(in_z), _vw(in_w)
	{
	}

	Vec4::Vec4(const Vec3& v, const float w)
		: _vx(v._vx), _vy(v._vy), _vz(v._vz), _vw(w)
	{
	}

	float& Vec4::operator[] (const enum x_enum)
	{
		return _vx;
	}

	float& Vec4::operator[] (const enum y_enum)
	{
		return _vy;
	}

	float& Vec4::operator[] (const enum z_enum)
	{
		return _vz;
	}

	float& Vec4::operator[] (const enum w_enum)
	{
		return _vw;
	}

	float Vec4::operator[](const x_enum) const
	{
		return _vx;
	}

	float Vec4::operator[](const y_enum) const
	{
		return _vy;
	}

	float Vec4::operator[](const z_enum) const
	{
		return _vz;
	}

	float Vec4::operator[](const w_enum) const
	{
		return _vw;
	}

	void Vec4::x(const float v)
	{
		_vx = v;
	}

	void Vec4::y(const float v)
	{
		_vy = v;
	}

	void Vec4::z(const float v)
	{
		_vz = v;
	}

	void Vec4::w(const float v)
	{
		_vw = v;
	}

	float Vec4::x() const
	{
		return _vx;
	}

	float Vec4::y() const
	{
		return _vy;
	}

	float Vec4::z() const
	{
		return _vz;
	}

	float Vec4::w() const
	{
		return _vw;
	}

	Vec4 Vec4::operator+(void) const
	{
		return Vec4(*this);
	}

	Vec4 Vec4::operator+(const Vec4& inV) const
	{
		return Vec4(_vx + inV._vx, _vy + inV._vy, _vz + inV._vz, _vw + inV._vw);
	}

	Vec4& Vec4::operator+=(const Vec4& inV)
	{
		_vx += inV._vx;
		_vy += inV._vy;
		_vz += inV._vz;
		_vw += inV._vw;
		return *this;
	}

	Vec4 Vec4::operator-(void) const
	{
		return Vec4(-_vx, -_vy, -_vz, -_vw);
	}

	Vec4 Vec4::operator-(const Vec4& inV) const
	{
		return Vec4(_vx - inV._vx, _vy - inV._vy, _vz - inV._vz, _vw - inV._vw);
	}

	Vec4& Vec4::operator-=(const Vec4& inV)
	{
		_vx -= inV._vx;
		_vy -= inV._vy;
		_vz -= inV._vz;
		_vw -= inV._vw;
		return *this;
	}

	Vec4 Vec4::operator*(const float scale) const
	{
		return Vec4(_vx * scale, _vy * scale, _vz * scale, _vw * scale);
	}

	Vec4& Vec4::operator*=(const float scale)
	{
		_vx *= scale;
		_vy *= scale;
		_vz *= scale;
		_vw *= scale;
		return *this;
	}

	Vec4 operator*(const float scale, const Vec4& inV)
	{
		return Vec4(inV._vx * scale, inV._vy * scale, inV._vz * scale, inV._vw * scale);
	}

	Vec4 Vec4::operator*(const Mat4& m) const
	{
		return Vec4(
			(_vx * m._m0) + (_vy * m._m4) + (_vz * m._m8) + (_vw * m._m12),
			(_vx * m._m1) + (_vy * m._m5) + (_vz * m._m9) + (_vw * m._m13),
			(_vx * m._m2) + (_vy * m._m6) + (_vz * m._m10) + (_vw * m._m14),
			(_vx * m._m3) + (_vy * m._m7) + (_vz * m._m11) + (_vw * m._m15)
		);
	}

	Vec4& Vec4::operator*=(const Mat4& m)
	{
		const float xDot = (_vx * m._m0) + (_vy * m._m4) + (_vz * m._m8) + (_vw * m._m12);
		const float yDot = (_vx * m._m1) + (_vy * m._m5) + (_vz * m._m9) + (_vw * m._m13);
		const float zDot = (_vx * m._m2) + (_vy * m._m6) + (_vz * m._m10) + (_vw * m._m14);
		const float wDot = (_vx * m._m3) + (_vy * m._m7) + (_vz * m._m11) + (_vw * m._m15);

		_vx = xDot;
		_vy = yDot;
		_vz = zDot;
		_vw = wDot;

		return *this;
	}

	Vec4& Vec4::norm(void)
	{
		float length = len();

		if (Util::isNonZero(length, MATH_TOLERANCE))
		{
			_vx /= length;
			_vy /= length;
			_vz /= length;
			_vw /= length;
		}

		return *this;
	}

	Vec4 Vec4::getNorm(void) const
	{
		auto output = Vec4();
		float length = len();

		if (Util::isNonZero(length, MATH_TOLERANCE))
		{
			output._vx = _vx / length;
			output._vy = _vy / length;
			output._vz = _vz / length;
			output._vw = _vw / length;
		}

		return output;
	}

	float Vec4::dot(const Vec4& vIn) const
	{
		return (_vx * vIn._vx) + (_vy * vIn._vy) + (_vz * vIn._vz) + (_vw * vIn._vw);
	}

	Vec4Proxy Vec4::len() const
	{
		return Vec4Proxy(_vx, _vy, _vz, _vw);
	}

	void Vec4::set(const float inX, const float inY, const float inZ, const float inW)
	{
		_vx = inX;
		_vy = inY;
		_vz = inZ;
		_vw = inW;
	}

	void Vec4::set(const Vec4& A)
	{
		*this = A;
	}

	void Vec4::set(const Vec3& v, const float w)
	{
		_vx = v._vx;
		_vy = v._vy;
		_vz = v._vz;
		_vw = w;
	}

	bool Vec4::isEqual(const Vec4& v, const float epsilon) const
	{
		return Util::isEqual(_vx, v._vx, epsilon) &&
			Util::isEqual(_vy, v._vy, epsilon) &&
			Util::isEqual(_vz, v._vz, epsilon) &&
			Util::isEqual(_vw, v._vw, epsilon);
	}

	bool Vec4::isZero(const float epsilon) const
	{
		return Util::isZero(_vx, epsilon) &&
			Util::isZero(_vy, epsilon) &&
			Util::isZero(_vz, epsilon) &&
			Util::isZero(_vw, epsilon);
	}

	void Vec4::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}
}