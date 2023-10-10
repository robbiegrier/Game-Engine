//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_MATRIX_4x4_H
#define ENGINE_MATH_MATRIX_4x4_H

#include "Vec4.h"
#include "Vec3.h"
#include "Debug.h"

namespace Azul
{
	struct Mat4Proxy;

	class Mat4 : public Align16
	{
	protected:

		enum class Hint : uint32_t
		{
			Generalize = 0x0,        // generalize 4x4 inverse  (no flags)
			Rot = 0x1,               // matrix inverse is M.T()
			Trans = 0x2,             // matrix inverse is M(-trans)
			RotTrans = 0x3,          // rot and trans  A_inv is A.T()
			Scale = 0x4,             // matrix invers is M(1/scale)
			RotScale = 0x5,          // Rot and Scale no translate
			TransScale = 0x6,        // Trans and Scale no Translate
			Affine = 0x7             // generalize Affine
		};

		inline void privSetGeneralHint()
		{
			_u_m15 &= 0xFFFFFFF8u;
		}

		inline void privSetRotHint()
		{
			_u_m15 |= 0b0001u;
		}

		inline void privSetTransHint()
		{
			_u_m15 |= 0b0010u;
		}

		inline void privSetScaleHint()
		{
			_u_m15 |= 0b0100u;
		}

		inline void privSetNewHint(Hint A, Hint B)
		{
			privSetGeneralHint();
			_u_m15 |= static_cast<unsigned int>(A);
			_u_m15 |= static_cast<unsigned int>(B);
		}

		inline void privSetCopyHint() const
		{
		}

		inline Hint privGetHint() const
		{
			return static_cast<Hint>(_u_m15 & 0b0111u);
		}

	public:
		// Big 4
		Mat4();
		Mat4& operator = (const Mat4& A);
		Mat4(const Mat4&);
		~Mat4();

		// Big 6
		Mat4(Mat4&&) = default;
		Mat4& operator = (Mat4&&) = default;

		//Mat4& operator = (const Quat&);

		// Constructors
		Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3);
		explicit Mat4(const Special type);
		explicit Mat4(const Quat& q);

		// Set
		void set(const Mat4& mIn);
		void set(const Vec4& V0, const Vec4& V1, const Vec4& V2, const Vec4& V3);
		void set(const Special type);
		void set(const Row4 type, const Vec4& V);
		void set(const Quat& q);

		// Get
		Vec4 get(const Row4 type) const;

		// bracket operators
		Mat4Proxy operator[] (const enum m0_enum);
		Mat4Proxy operator[] (const enum m1_enum);
		Mat4Proxy operator[] (const enum m2_enum);
		Mat4Proxy operator[] (const enum m3_enum);
		Mat4Proxy operator[] (const enum m4_enum);
		Mat4Proxy operator[] (const enum m5_enum);
		Mat4Proxy operator[] (const enum m6_enum);
		Mat4Proxy operator[] (const enum m7_enum);
		Mat4Proxy operator[] (const enum m8_enum);
		Mat4Proxy operator[] (const enum m9_enum);
		Mat4Proxy operator[] (const enum m10_enum);
		Mat4Proxy operator[] (const enum m11_enum);
		Mat4Proxy operator[] (const enum m12_enum);
		Mat4Proxy operator[] (const enum m13_enum);
		Mat4Proxy operator[] (const enum m14_enum);
		Mat4Proxy operator[] (const enum m15_enum);

		float operator[] (const enum m0_enum) const;
		float operator[] (const enum m1_enum) const;
		float operator[] (const enum m2_enum) const;
		float operator[] (const enum m3_enum) const;
		float operator[] (const enum m4_enum) const;
		float operator[] (const enum m5_enum) const;
		float operator[] (const enum m6_enum) const;
		float operator[] (const enum m7_enum) const;
		float operator[] (const enum m8_enum) const;
		float operator[] (const enum m9_enum) const;
		float operator[] (const enum m10_enum) const;
		float operator[] (const enum m11_enum) const;
		float operator[] (const enum m12_enum) const;
		float operator[] (const enum m13_enum) const;
		float operator[] (const enum m14_enum) const;
		float operator[] (const enum m15_enum) const;

		// Accessor
		void m0(const float v);
		void m1(const float v);
		void m2(const float v);
		void m3(const float v);
		void m4(const float v);
		void m5(const float v);
		void m6(const float v);
		void m7(const float v);
		void m8(const float v);
		void m9(const float v);
		void m10(const float v);
		void m11(const float v);
		void m12(const float v);
		void m13(const float v);
		void m14(const float v);
		void m15(const float v);

		float m0() const;
		float m1() const;
		float m2() const;
		float m3() const;
		float m4() const;
		float m5() const;
		float m6() const;
		float m7() const;
		float m8() const;
		float m9() const;
		float m10() const;
		float m11() const;
		float m12() const;
		float m13() const;
		float m14() const;
		float m15() const;

		// Determinant
		float det() const;

		// Transpose
		Mat4& T(void);
		Mat4 getT(void)const;

		// Inverse
		Mat4 getInv(void) const;
		Mat4& inv(void);

		// Comparison
		bool isEqual(const Mat4& A, const float epsilon = MATH_TOLERANCE) const;
		bool isIdentity(const float epsilon = MATH_TOLERANCE) const;
		//bool isRotation(const float epsilon = MATH_TOLERANCE) const;

		// Add operators
		Mat4 operator + (void) const;
		Mat4 operator + (const Mat4& A) const;
		Mat4& operator += (const Mat4& A);

		// Sub operators
		Mat4 operator - (void) const;
		Mat4 operator - (const Mat4& A) const;
		Mat4& operator -= (const Mat4& A);

		// Scale operators
		Mat4 operator * (const float s) const;
		friend Mat4 operator *(const float scale, const Mat4& A);
		Mat4& operator *= (const float scale);

		// Multiply
		Mat4 operator * (const Mat4& A) const;
		Mat4& operator *= (const Mat4& A);

		// Simple casting to Mat4 operations... just a wrapper
		Mat4 operator * (const Quat& q) const;
		Mat4& operator *= (const Quat& q);

		Mat4 operator * (const Scale& A) const;
		Mat4& operator *= (const Scale& A);

		Mat4 operator * (const Rot& A) const;
		Mat4& operator *= (const Rot& A);

		Mat4 operator * (const Trans& A) const;
		Mat4& operator *= (const Trans& A);

		// For printing
		void Print(const char* pName) const;

	protected:
		Mat4 privGetAdj(void) const;

		friend Mat3;
		friend Vec3;
		friend Vec4;
		friend Mat4Proxy;
		friend Quat;
		friend Scale;
		friend Trans;
		friend Rot;

		union
		{
			/*     | m0  m1  m2   m3  | */
			/* m = | m4  m5  m6   m7  | */
			/*     | m8  m9  m10  m11 | */
			/*     | m12 m13 m14  m15 | */

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
				union
				{
					float _m15;
					unsigned int _u_m15;
				};
			};
		};
	};
}

#endif

//--- End of File ---
