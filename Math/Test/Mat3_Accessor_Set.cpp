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

TEST(Mat3_Accessor_Set, TestConfig::ALL)
{
#if Mat3_Accessor_Set

	Mat3 M;

	CHECK(M.m0() == 0.0f);
	CHECK(M.m1() == 0.0f);
	CHECK(M.m2() == 0.0f);

	CHECK(M.m4() == 0.0f);
	CHECK(M.m5() == 0.0f);
	CHECK(M.m6() == 0.0f);

	CHECK(M.m8() == 0.0f);
	CHECK(M.m9() == 0.0f);
	CHECK(M.m10() == 0.0f);

	M.m0(1.0f);
	M.m1(2.0f);
	M.m2(3.0f);

	M.m4(7.0f);
	M.m5(6.0f);
	M.m6(5.0f);

	M.m8(-4.0f);
	M.m9(-2.0f);
	M.m10(-1.0f);

	CHECK(M.m0() == 1.0f);
	CHECK(M.m1() == 2.0f);
	CHECK(M.m2() == 3.0f);

	CHECK(M.m4() == 7.0f);
	CHECK(M.m5() == 6.0f);
	CHECK(M.m6() == 5.0f);

	CHECK(M.m8() == -4.0f);
	CHECK(M.m9() == -2.0f);
	CHECK(M.m10() == -1.0f);

#endif
} TEST_END

// --- End of File ---
