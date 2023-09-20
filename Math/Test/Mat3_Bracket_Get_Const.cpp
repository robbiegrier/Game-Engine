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

#if Mat3_Bracket_Get_Const
float constTest(const Mat3 &t)
{
	return (t[m0] + t[m1] + t[m2] 
		+ t[m4] + t[m5] + t[m6]
		+ t[m8] + t[m9] + t[m10]
		);
}
#endif

TEST(Mat3_Bracket_Get_Const, TestConfig::ALL)
{
#if Mat3_Bracket_Get_Const

	Mat3 M;

	M[m0] = 1.0f;
	M[m1] = 2.0f;
	M[m2] = 3.0f;
	
	M[m4] = 7.0f;
	M[m5] = 6.0f;
	M[m6] = 5.0f;

	M[m8] = -4.0f;
	M[m9] = -2.0f;
	M[m10] = -1.0f;

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);

	const float sum = constTest(M);

	CHECK(sum == 17.0f);

#endif
} TEST_END

// --- End of File ---

