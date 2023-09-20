//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_VEC4_PROXY_H
#define ENGINE_MATH_VEC4_PROXY_H

namespace Azul
{
	// Vector related proxies here:

	struct Vec4Proxy
	{
	public:
		Vec4Proxy(float a, float b, float c, float d);

		Vec4Proxy() = delete;
		Vec4Proxy(const Vec4Proxy&) = default;
		Vec4Proxy& operator = (const Vec4Proxy&) = default;
		~Vec4Proxy() = default;

		operator float() const;

		float operator * (const Vec4Proxy& r) const;
		bool operator > (const Vec4Proxy& r) const;
		bool operator < (const Vec4Proxy& r) const;
		bool operator == (const Vec4Proxy& r) const;
		bool operator >= (const Vec4Proxy& r) const;
		bool operator <= (const Vec4Proxy& r) const;
		bool operator != (const Vec4Proxy& r) const;

	private:
		float x;
		float y;
		float z;
		float w;
	};
}

#endif

//--- End of File ---
