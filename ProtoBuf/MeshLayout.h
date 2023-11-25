//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_LAYOUT_H
#define MESH_LAYOUT_H

namespace Azul
{

	struct Mat4f
	{
		Mat4f() = default;
		Mat4f(const Mat4f &) = default;
		Mat4f &operator = (const Mat4f &) = default;
		~Mat4f() = default;

		float m0;
		float m1;
		float m2;
		float m3;

		float m4;
		float m5;
		float m6;
		float m7;

		float m8;
		float m9;
		float m10;
		float m11;

		float m12;
		float m13;
		float m14;
		float m15;


	};

	struct Vec2f
	{
		Vec2f()
			: u(0.0f), v(0.0f){}
		Vec2f(const Vec2f &) = default;
		Vec2f &operator = (const Vec2f &) = default;
		~Vec2f() = default;

		Vec2f(const float _u, const float _v)
			: u(_u), v(_v){}

		float u;
		float v;
	};

	struct Vec3f
	{
		Vec3f()
			: x(0.0f), y(0.0f), z(0.0f){}
		Vec3f(const Vec3f &) = default;
		Vec3f &operator = (const Vec3f &) = default;
		~Vec3f() = default;

		Vec3f(const float a, const float b, const float c)
			: x(a), y(b), z(c){}

		Vec3f operator * (float val)
		{
			x *= val;
			y *= val;
			z *= val;

			return *this;
		}

		float x;
		float y;
		float z;
	};

	struct Vec4f
	{
		Vec4f()
			: x(0.0f), y(0.0f), z(0.0f), w(1.0f){}
		Vec4f(const Vec4f &) = default;
		Vec4f &operator = (const Vec4f &) = default;
		~Vec4f() = default;

		Vec4f(const float a, const float b, const float c, const float d)
			: x(a), y(b), z(c), w(d){}


		float x;
		float y;
		float z;
		float w;
	};

	struct Vec3ui
	{
		Vec3ui() = default;
		Vec3ui(const Vec3ui &) = default;
		Vec3ui &operator = (const Vec3ui &) = default;
		~Vec3ui() = default;

		Vec3ui(const unsigned int a, const unsigned int b, const unsigned int c)
			: x(a), y(b), z(c){}


		unsigned int x;
		unsigned int y;
		unsigned int z;
	};

	struct Vec4ui
	{
		Vec4ui() = default;
		Vec4ui(const Vec4ui &) = default;
		Vec4ui &operator = (const Vec4ui &) = default;
		~Vec4ui() = default;

		Vec4ui(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d)
			: x(a), y(b), z(c), w(d){}

		unsigned int x;
		unsigned int y;
		unsigned int z;
		unsigned int w;
	};

	struct Vec4si
	{
		Vec4si() = default;
		Vec4si(const Vec4si &) = default;
		Vec4si &operator = (const Vec4si &) = default;
	   ~Vec4si() = default;

		Vec4si(const unsigned short a, const unsigned short b, const unsigned short c, const unsigned short d)
			: x(a), y(b), z(c), w(d){}


		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short w;
	};

	struct Vec3si
	{
		Vec3si() = default;
		Vec3si(const Vec3si &) = default;
		Vec3si &operator = (const Vec3si &) = default;
		~Vec3si() = default;

		Vec3si(const unsigned short a, const unsigned short b, const unsigned short c)
			: x(a), y(b), z(c){}

		unsigned short x;
		unsigned short y;
		unsigned short z;
	};

	// Vertex data for a colored cube.
	struct VertexColor
	{
		Vec4f Color;  // r,g,b,a
	};

	struct VertexPos
	{
		VertexPos() = default;
		VertexPos(const VertexPos &) = default;
		VertexPos &operator = (const VertexPos &) = default;
		~VertexPos() = default;

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

	struct TriIndex
	{
		Vec3ui index;
	};
}

#endif

// --- End of File ---
