#ifndef _MeshUtils
#define _MeshUtils

#include "MathEngine.h"

namespace Azul
{
	// Quick Hack
	struct Vec2f
	{
		Vec2f()
			: u(0.0f), v(0.0f) {}
		Vec2f(const Vec2f&) = default;
		Vec2f& operator = (const Vec2f&) = default;
		~Vec2f() = default;

		Vec2f(const float _u, const float _v)
			: u(_u), v(_v) {}

		float u;
		float v;
	};

	template<typename T>
	struct Point
	{
		Point() : x(T()), y(T()) {}
		Point(const Point<T>&) = default;
		Point& operator = (const Point<T>&) = default;
		~Point() = default;

		bool operator == (const Point<T>& rhs)
		{
			return x == rhs.x && y == rhs.y;
		}

		bool operator != (const Point<T>& rhs)
		{
			return !(*this == rhs);
		}

		Point(T _x, T _y)
			: x(_x), y(_y) {}

		T x;
		T y;
	};

	template<typename T>
	struct TVec3
	{
		TVec3()
			: x(T()), y(T()), z(T()) {}
		TVec3(const TVec3&) = default;
		TVec3& operator = (const TVec3&) = default;
		~TVec3() = default;

		TVec3(T a, T b, T c)
			: x(a), y(b), z(c) {}

		TVec3 operator*(T s) { return TVec3(x * s, y * s, z * s); }

		T x;
		T y;
		T z;
	};

	struct Vec3f
	{
		Vec3f()
			: x(0.0f), y(0.0f), z(0.0f) {}
		Vec3f(const Vec3f&) = default;
		Vec3f& operator = (const Vec3f&) = default;
		~Vec3f() = default;

		Vec3f(const float a, const float b, const float c)
			: x(a), y(b), z(c) {}

		Vec3f operator*(float s) { return Vec3f(x * s, y * s, z * s); }
		Vec3f operator+(const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }

		float x;
		float y;
		float z;
	};

	struct Vec4f
	{
		Vec4f()
			: x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		Vec4f(const Vec4f&) = default;
		Vec4f& operator = (const Vec4f&) = default;
		~Vec4f() = default;

		Vec4f(const float a, const float b, const float c, const float d)
			: x(a), y(b), z(c), w(d) {}

		float x;
		float y;
		float z;
		float w;
	};

	struct Vec4ui
	{
		Vec4ui() = default;
		Vec4ui(const Vec4ui&) = default;
		Vec4ui& operator = (const Vec4ui&) = default;
		~Vec4ui() = default;

		Vec4ui(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d)
			: x(a), y(b), z(c), w(d) {}

		unsigned int x;
		unsigned int y;
		unsigned int z;
		unsigned int w;
	};

	struct Vec4si
	{
		Vec4si() = default;
		Vec4si(const Vec4si&) = default;
		Vec4si& operator = (const Vec4si&) = default;
		~Vec4si() = default;

		Vec4si(const unsigned short a, const unsigned short b, const unsigned short c, const unsigned short d)
			: x(a), y(b), z(c), w(d) {}

		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short w;
	};

	struct Vec3si
	{
		Vec3si() = default;
		Vec3si(const Vec3si&) = default;
		Vec3si& operator = (const Vec3si&) = default;
		~Vec3si() = default;

		Vec3si(const unsigned short a, const unsigned short b, const unsigned short c)
			: x(a), y(b), z(c) {}

		unsigned short x;
		unsigned short y;
		unsigned short z;
	};

	// Vertex data for a colored cube.
	struct VertexColor
	{
		Vec3 Color;
	};

	struct VertexPos
	{
		Vec3f Position;
	};

	struct VertexNorm
	{
		Vec3f Norm;
	};

	struct VertexTexCoord
	{
		Vec2f TexCoord;
	};

	struct VertexWeights
	{
		Vec4f Weight;
	};

	struct VertexJoints
	{
		Vec4si Joint;
	};

	enum class VertexSlot : uint32_t
	{
		Position,  //0
		Color,     //1
		Norm,      //2
		TexCoord,  //3
		Weights,   //4
		Joints     //5
	};

	enum class ConstantBufferSlot : uint32_t
	{
		Projection,
		View,
		World,

		UV_Correction,

		Color,
		LightPos,

		psSpriteColor,	 //6

		vsInvBind = 7,       //7
		vsBoneWorld = 8    //8
	};
}

#endif