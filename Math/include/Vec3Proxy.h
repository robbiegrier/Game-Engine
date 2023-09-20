//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_VEC3_PROXY_H
#define ENGINE_MATH_VEC3_PROXY_H

namespace Azul
{
	// Vector related proxies here:

	struct Vec3Proxy
	{
	public:

		// Suggested inteface:

		Vec3Proxy(float a, float b, float c);

		Vec3Proxy() = delete;
		Vec3Proxy(const Vec3Proxy&) = default;
		Vec3Proxy& operator = (const Vec3Proxy&) = default;
		~Vec3Proxy() = default;

		operator float() const;

		float operator * (const Vec3Proxy& r) const;

		bool operator > (const Vec3Proxy& r) const;
		bool operator < (const Vec3Proxy& r) const;
		bool operator == (const Vec3Proxy& r) const;
		bool operator >= (const Vec3Proxy& r) const;
		bool operator <= (const Vec3Proxy& r) const;
		bool operator != (const Vec3Proxy& r) const;

	private:
		float x;
		float y;
		float z;
	};
}

#endif

//--- End of File ---
