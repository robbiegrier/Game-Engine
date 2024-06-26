//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_HELPER_H
#define GLTF_HELPER_H

class Vec3fx
{
public:
	float x, y, z = 0.f;
	Vec3fx() = default;
	Vec3fx(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vec3fx(const Vec3fx&) = default;
	Vec3fx& operator =(const Vec3fx&) = default;
	~Vec3fx() = default;
};

class StringThis
{
public:
	static const unsigned int BUFFER_SIZE = 64;
public:
	StringThis(int x);

	operator char* ();

	// data:
	char buffer[BUFFER_SIZE];
};

//class PTrace
//{
//public:
//	static void out(const char* const fmt, ...)
//	{
//		va_list args;
//		va_start(args, fmt);
//		va_end(args, fmt);
//
//		Trace::out(fmt, args);
//	}
//};

#define PTrace(...) Trace::out(__VA_ARGS__); printf(__VA_ARGS__);

#define StringMe(x)  ((char *)StringThis(x))

#endif

// --- End of File ---
