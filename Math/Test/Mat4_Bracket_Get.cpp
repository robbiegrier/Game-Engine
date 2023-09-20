//---------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

//---------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq	Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------

TEST(Mat4_Bracket_Get, TestConfig::ALL)
{
#if Mat4_Bracket_Get

	Vec4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vec4 V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vec4 V3(9.0f, -7.0f, -2.0f, 5.0f);

	Mat4 M(V0, V1, V2, V3);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	float t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;

	t0 = M[m0];
	t1 = M[m1];
	t2 = M[m2];
	t3 = M[m3];
	t4 = M[m4];
	t5 = M[m5];
	t6 = M[m6];
	t7 = M[m7];
	t8 = M[m8];
	t9 = M[m9];
	t10 = M[m10];
	t11 = M[m11];
	t12 = M[m12];
	t13 = M[m13];
	t14 = M[m14];
	t15 = M[m15];

	CHECK(t0 == 1.0f);
	CHECK(t1 == 2.0f);
	CHECK(t2 == 3.0f);
	CHECK(t3 == 4.0f);
	CHECK(t4 == 7.0f);
	CHECK(t5 == 6.0f);
	CHECK(t6 == 5.0f);
	CHECK(t7 == 3.0f);
	CHECK(t8 == -4.0f);
	CHECK(t9 == -2.0f);
	CHECK(t10 == -1.0f);
	CHECK(t11 == -4.0f);
	CHECK(t12 == 9.0f);
	CHECK(t13 == -7.0f);
	CHECK(t14 == -2.0f);
	CHECK(t15 == 5.0f);

#endif
} TEST_END

// ---  End of File ---
