//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_MATRIX_3x3_H
#define ENGINE_MATH_MATRIX_3x3_H

#include "Vec4.h"
#include "Debug.h"
#include "Mat4.h"

namespace Azul
{
	class Mat3 final : public Align16
	{
	public:
		// Big 4
		Mat3();
		Mat3& operator = (const Mat3& A);
		Mat3(const Mat3& tM);
		~Mat3();

		// Big 6
		Mat3(Mat3&&) = default;
		Mat3& operator=(Mat3&&) = default;

		Mat3(const Vec3& tV0, const Vec3& tV1, const Vec3& tV2);

		explicit Mat3(const Mat4& m);
		explicit Mat3(const Special type);

		// Get
		Vec3 get(const Row3 type) const;

		// Set
		void set(const Mat3& mIn);
		void set(const Special type);
		void set(const Row3 type, const Vec3& V);
		void set(const Vec3& V0, const Vec3& V1, const Vec3& V2);

		// bracket operators
		float& operator[] (const enum m0_enum);
		float& operator[] (const enum m1_enum);
		float& operator[] (const enum m2_enum);

		float& operator[] (const enum m4_enum);
		float& operator[] (const enum m5_enum);
		float& operator[] (const enum m6_enum);

		float& operator[] (const enum m8_enum);
		float& operator[] (const enum m9_enum);
		float& operator[] (const enum m10_enum);

		float operator[] (const enum m0_enum) const;
		float operator[] (const enum m1_enum) const;
		float operator[] (const enum m2_enum) const;

		float operator[] (const enum m4_enum) const;
		float operator[] (const enum m5_enum) const;
		float operator[] (const enum m6_enum) const;

		float operator[] (const enum m8_enum) const;
		float operator[] (const enum m9_enum) const;
		float operator[] (const enum m10_enum) const;

		// Accessor
		void m0(const float v);
		void m1(const float v);
		void m2(const float v);

		void m4(const float v);
		void m5(const float v);
		void m6(const float v);

		void m8(const float v);
		void m9(const float v);
		void m10(const float v);

		float m0() const;
		float m1() const;
		float m2() const;

		float m4() const;
		float m5() const;
		float m6() const;

		float m8() const;
		float m9() const;
		float m10() const;

		// Determinant
		float det() const;

		// Transpose
		Mat3& T(void);
		Mat3 getT(void)const;

		// Inverse
		Mat3 getInv(void) const;
		Mat3& inv(void);

		// Comparison
		bool isEqual(const Mat3& A, const float epsilon = MATH_TOLERANCE) const;
		bool isIdentity(const float epsilon = MATH_TOLERANCE) const;

		// Add operators
		Mat3 operator + (void) const;
		Mat3 operator + (const Mat3& A) const;
		Mat3& operator += (const Mat3& A);

		// Sub operators
		Mat3 operator - (void) const;
		Mat3 operator - (const Mat3& A) const;
		Mat3& operator -= (const Mat3& A);

		// Scale operators
		Mat3 operator * (const float s) const;
		friend Mat3 operator *(const float scale, const Mat3& A);
		Mat3& operator *= (const float scale);

		// Multiply
		Mat3 operator * (const Mat3& A) const;
		Mat3& operator *= (const Mat3& A);

		// For printing
		void Print(const char* pName) const;

	private:

		friend Mat4;
		friend Vec3;
		friend Vec4;

		union
		{
			/*     | m0  m1  m2   0 | */
			/* m = | m4  m5  m6   0 | */
			/*     | m8  m9  m10  0 | */
			/*     | 0   0   0    1 | */

			struct
			{
				Vec4 _rows[4];
			};

			struct
			{
				Vec4 _v0;
				Vec4 _v1;
				Vec4 _v2;
				Vec4 _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};
}

#endif

//--- End of File ---
