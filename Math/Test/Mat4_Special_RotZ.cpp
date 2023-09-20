//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq	Util::isEqual
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Mat4_Special_RotZ, TestConfig::ALL)
{
#if Mat4_Special_RotZ

	Mat4 Rz(Mat4::Rot1::Z, 1.0471975512f);

	CHECK(eq(Rz[m0], 0.5f, MATH_TOLERANCE));
	CHECK(eq(Rz[m1], 0.866f, MATH_TOLERANCE));
	CHECK(Rz[m2] == 0.0f);
	CHECK(Rz[m3] == 0.0f);
	CHECK(eq(Rz[m4], -0.866f, MATH_TOLERANCE));
	CHECK(eq(Rz[m5], 0.5f, MATH_TOLERANCE));
	CHECK(Rz[m6] == 0.0f);
	CHECK(Rz[m7] == 0.0f);
	CHECK(Rz[m8] == 0.0f);
	CHECK(Rz[m9] == 0.0f);
	CHECK(Rz[m10] == 1.0f);
	CHECK(Rz[m11] == 0.0f);
	CHECK(Rz[m12] == 0.0f);
	CHECK(Rz[m13] == 0.0f);
	CHECK(Rz[m14] == 0.0f);
	//CHECK(Rz[m15] == 1.0f);
	CHECK(Rz.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
