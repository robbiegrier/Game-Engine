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

#if Mat4_Bracket_Get_Const
float constTest(const Mat4 &t)
{
	return (t[m0] + t[m1] + t[m2] + t[m3]
		+ t[m4] + t[m5] + t[m6] + t[m7]
		+ t[m8] + t[m9] + t[m10] + t[m11]
		+ t[m12] + t[m13] + t[m14] + t[m15]
		);
}
#endif

TEST(Mat4_Bracket_Get_Const, TestConfig::ALL)
{
#if Mat4_Bracket_Get_Const
	Mat4 M;
	M[m0] = 1.0f;
	M[m1] = 2.0f;
	M[m2] = 3.0f;
	M[m3] = 4.0f;
	M[m4] = 7.0f;
	M[m5] = 6.0f;
	M[m6] = 5.0f;
	M[m7] = 3.0f;
	M[m8] = -4.0f;
	M[m9] = -2.0f;
	M[m10] = -1.0f;
	M[m11] = -4.0f;
	M[m12] = 9.0f;
	M[m13] = -7.0f;
	M[m14] = -2.0f;
	M[m15] = 5.0f;

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

	const float sum = constTest(M);

	CHECK(sum == 25.0f);

#endif
} TEST_END

// --- End of File ---

