//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"

namespace Azul
{
	// Add Proxy here

	Vec4Proxy::Vec4Proxy(float a, float b, float c, float d)
		: x(a), y(b), z(c), w(d)
	{
	}

	Vec4Proxy::operator float() const
	{
		return Trig::sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}

	float Vec4Proxy::operator*(const Vec4Proxy& r) const
	{
		return static_cast<float>(*this) * static_cast<float>(r);
	}

	bool Vec4Proxy::operator>(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return sqrThis > sqrR;
	}

	bool Vec4Proxy::operator<(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return sqrThis < sqrR;
	}

	bool Vec4Proxy::operator==(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE);
	}

	bool Vec4Proxy::operator>=(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE) || sqrThis > sqrR;
	}

	bool Vec4Proxy::operator<=(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return Util::isEqual(sqrThis, sqrR, MATH_TOLERANCE) || sqrThis < sqrR;
	}

	bool Vec4Proxy::operator!=(const Vec4Proxy& r) const
	{
		auto sqrThis = (x * x) + (y * y) + (z * z) + (w * w);
		auto sqrR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z) + (r.w * r.w);
		return Util::isNotEqual(sqrThis, sqrR, MATH_TOLERANCE);
	}
}

//--- End of File ---