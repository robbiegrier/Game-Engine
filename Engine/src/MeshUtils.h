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

		float x;
		float y;
		float z;
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

	enum class VertexSlot : uint32_t
	{
		Position,  //0
		Color,     //1
		Norm,      //2
		TexCoord   //3
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