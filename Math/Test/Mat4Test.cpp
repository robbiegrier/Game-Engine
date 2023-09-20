#include "Mat4Test.h"
#include "_UnitTestConfiguration.h"

namespace Azul
{
#if SpeedTestA

	Mat4Test::Mat4Test()
		: _v0(0.0f, 0.0f, 0.0f, 0.0f),
		_v1(0.0f, 0.0f, 0.0f, 0.0f),
		_v2(0.0f, 0.0f, 0.0f, 0.0f),
		_v3(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	Mat4Test::~Mat4Test()
	{
	}

	Mat4Test::Mat4Test(const Mat4Test &tM)
		:_v0(tM._v0), _v1(tM._v1), _v2(tM._v2), _v3(tM._v3)
	{
	};

	Mat4Test &Mat4Test::operator = (const Mat4Test &A)
	{
		_v0 = A._v0;
		_v1 = A._v1;
		_v2 = A._v2;
		_v3 = A._v3;

		return *this;
	}

	const Mat4Test Mat4Test::getInv2(void) const
	{
		Mat4Test tmp;

		// Full blown 4x4 matrix inverse
		const float det = this->det();
		const float invDet = 1.0f / det;

		tmp = this->privGetAdj();
		tmp *= invDet;

		return tmp;
	}

	const Mat4Test Mat4Test::privGetAdj(void) const
	{
		float A, B, C, D, E, F, G, H, I;
		Mat4Test tmp;

		A = (_m10 * _m15) - (_m11 * _m14);
		B = (_m7 * _m14) - (_m6 * _m15);
		C = (_m6 * _m11) - (_m7 * _m10);
		D = (_m3 * _m14) - (_m2 * _m15);
		E = (_m2 * _m11) - (_m3 * _m10);
		F = (_m2 * _m7) - (_m3 * _m6);
		G = _m5;
		H = _m13;

		/* 0		a = {5,  9, 13,  6, 10, 14,  7, 11, 15} */
		tmp._m0 = (G * A) + (_m9 * B) + (H * C);

		/* 2		a = {1,  5, 13,  2,  6, 14,  3,  7, 15} */
		tmp._m2 = -(_m1 * B) + (G * D) + (H * F);

		G = _m0;
		H = _m8;
		/* 5		a = {0,  8, 12,  2, 10, 14,  3, 11, 15} */
		tmp._m5 = (G * A) + (H * D) + (_m12 * E);

		/* 7		a = {0,  4,  8,  2,  6, 10,  3,  7, 11} */
		tmp._m7 = (G * C) + (H * F) - (_m4 * E);

		A = (_m9 * _m15) - (_m11 * _m13);
		B = (_m3 * _m13) - (_m1 * _m15);
		C = (_m1 * _m11) - (_m3 * _m9);
		D = (_m7 * _m13) - (_m5 * _m15);
		E = (_m5 * _m11) - (_m7 * _m9);
		F = (_m1 * _m7) - (_m3 * _m5);
		G = _m2;
		H = _m10;
		/* 1		a = {2, 10, 14,  1,  9, 13,  3, 11, 15} */
		tmp._m1 = (G * A) + (H * B) + (_m14 * C);

		/* 3		a = {2,  6, 10,  1,  5,  9,  3,  7, 11} */
		tmp._m3 = (G * E) + (H * F) - (_m6 * C);

		G = _m4;
		H = _m12;
		/* 8		a = {4,  8, 12,  5,  9, 13,  7, 11, 15} */
		tmp._m8 = (G * A) + (_m8 * D) + (H * E);

		/* A		a = {0,  4, 12,  1,  5, 13,  3,  7, 15} */
		tmp._m10 = -(_m0 * D) + (G * B) + (H * F);

		A = (_m8 * _m15) - (_m11 * _m12);
		B = (_m4 * _m11) - (_m7 * _m8);
		C = (_m3 * _m12) - (_m0 * _m15);
		D = (_m0 * _m7) - (_m3 * _m4);
		E = (_m7 * _m12) - (_m4 * _m15);
		F = (_m0 * _m11) - (_m3 * _m8);
		G = _m6;
		H = _m14;
		/* 4		a = {6, 10, 14,  4,  8, 12,  7, 11, 15} */
		tmp._m4 = (G * A) + (_m10 * E) + (H * B);

		/* 6		a = {2,  6, 14,  0,  4, 12,  3,  7, 15} */
		tmp._m6 = -(_m2 * E) + (G * C) + (H * D);

		G = _m1;
		H = _m9;
		/* 9		a = {1,  9, 13,  0,  8, 12,  3, 11, 15} */
		tmp._m9 = (G * A) + (H * C) + (_m13 * F);

		/* B		a = {1,  5,  9,  0,  4,  8,  3,  7, 11} */
		tmp._m11 = (G * B) + (H * D) - (_m5 * F);

		A = _m4;
		B = _m6;
		C = _m12;
		D = _m14;
		E = (B * C) - (A * D);
		F = _m8;
		G = _m10;
		H = _m5;
		I = _m13;
		/* C		a = {5,  9, 13,  4,  8, 12,  6, 10, 14} */
		tmp._m12 = (H * ((F * D) - (G * C))) + (_m9 * E) + (I * ((A * G) - (B * F)));

		F = _m2;
		G = _m0;
		/* E		a = {1,  5, 13,  0,  4, 12,  2,  6, 14} */
		tmp._m14 = -(_m1 * E) + (H * ((F * C) - (G * D))) + (I * ((G * B) - (F * A)));

		A = _m1;
		B = _m2;
		C = _m9;
		D = _m10;
		E = (A * D) - (B * C);
		F = _m13;
		G = _m14;
		H = _m0;
		I = _m8;
		/* D		a = {0,  8, 12,  1,  9, 13,  2, 10, 14} */
		tmp._m13 = (H * ((C * G) - (D * F))) + (I * ((B * F) - (A * G))) + (_m12 * E);

		F = _m5;
		G = _m6;
		/* F		a = {0,  4,  8,  1,  5,  9,  2,  6, 10}} */
		tmp._m15 = -(_m4 * E) + (H * ((F * D) - (G * C))) + (I * ((A * G) - (B * F)));

		return(tmp);
	}

	void Mat4Test::operator *= (const float s)
	{
		_v0.set(s * _m0, s * _m1, s * _m2, s * _m3);
		_v1.set(s * _m4, s * _m5, s * _m6, s * _m7);
		_v2.set(s * _m8, s * _m9, s * _m10, s * _m11);
		_v3.set(s * _m12, s * _m13, s * _m14, s * _m15);
	}

	const float Mat4Test::det() const
	{
		float A, B, C, D, E, F;

		/*     | m0  m1  m2  m3  | */
		/* m = | m4  m5  m6  m7  | */
		/*     | m8  m9  m10 m11 | */
		/*     | m12 m13 m14 m15 | */

		/* det(m) =  m0*[m5(m10*m15-m14*m11) -m6*(m9*m15-m13*m11) +m7*(m9*m14-m13*m10)] */
		/*          -m1*[m4(m10*m15-m14*m11) -m6*(m8*m15-m12*m11) +m7*(m8*m14-m12*m10)] */
		/*           m2*[m4(m9*m15-m13*m11)  -m5*(m8*m15-m12*m11) +m7*(m8*m13-m12*m9) ] */
		/*          -m3*[m4(m9*m14-m13*m10)  -m5*(m8*m14-m12*m10) +m6*(m8*m13-m12*m9) ] */

		/* let: A = m10*m15 - m14*m11 */
		/*      B = m8*m15  - m12*m11 */
		/*      C = m8*m13  - m12*m9  */
		/*      D = m8*m14  - m12*m10 */
		/*      E = m9*m15  - m13*m11 */
		/*      F = m9*m14  - m13*m10 */

		/* det(m) =  m0*[ m5*A - m6*E + m7*F ] */
		/*          -m1*[ m4*A - m6*B + m7*D ] */
		/*           m2*[ m4*E - m5*B + m7*C ] */
		/*          -m3*[ m4*F - m5*D + m6*C ] */

		A = (_m10 * _m15) - (_m14 * _m11);
		B = (_m8 * _m15) - (_m12 * _m11);
		C = (_m8 * _m13) - (_m12 * _m9);
		D = (_m8 * _m14) - (_m12 * _m10);
		E = (_m9 * _m15) - (_m13 * _m11);
		F = (_m9 * _m14) - (_m13 * _m10);

		return (_m0 * ((_m5 * A) - (_m6 * E) + (_m7 * F))
			- _m1 * ((_m4 * A) - (_m6 * B) + (_m7 * D))
			+ _m2 * ((_m4 * E) - (_m5 * B) + (_m7 * C))
			- _m3 * ((_m4 * F) - (_m5 * D) + (_m6 * C)));
	}

#endif
}

// **** End of File ***
