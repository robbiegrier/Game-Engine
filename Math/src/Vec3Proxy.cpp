//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	Vec3Proxy::Vec3Proxy(float a, float b, float c)
		: x(a), y(b), z(c)
	{
	}

	Vec3Proxy::operator float() const
	{
		return Trig::sqrt((x * x) + (y * y) + (z * z));
	}

	float Vec3Proxy::operator*(const Vec3Proxy& r) const
	{
		return static_cast<float>(*this) * static_cast<float>(r);
	}

	bool Vec3Proxy::operator>(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return sqrThis > sqrR;
	}

	bool Vec3Proxy::operator<(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return sqrThis < sqrR;
	}

	bool Vec3Proxy::operator==(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE);
	}

	bool Vec3Proxy::operator>=(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE) || sqrThis > sqrR;
	}

	bool Vec3Proxy::operator<=(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE) || sqrThis < sqrR;
	}

	bool Vec3Proxy::operator!=(const Vec3Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
		return Util::isNotEqual(sqrThis, sqrR, MATH_TOLERANCE);
	}
}

//--- End of File ---