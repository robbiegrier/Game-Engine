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

TEST(Mat4_Special_RotY, TestConfig::ALL)
{
#if Mat4_Special_RotY

	Mat4 Ry(Mat4::Rot1::Y, 1.0471975512f);

	CHECK(eq(Ry[m0], 0.5f, MATH_TOLERANCE));
	CHECK(Ry[m1] == 0.0f);
	CHECK(eq(Ry[m2], -0.866f, MATH_TOLERANCE));
	CHECK(Ry[m3] == 0.0f);
	CHECK(Ry[m4] == 0.0f);
	CHECK(Ry[m5] == 1.0f);
	CHECK(Ry[m6] == 0.0f);
	CHECK(Ry[m7] == 0.0f);
	CHECK(eq(Ry[m8], 0.866f, MATH_TOLERANCE));
	CHECK(Ry[m9] == 0.0f);
	CHECK(eq(Ry[m10], 0.5f, MATH_TOLERANCE));
	CHECK(Ry[m11] == 0.0f);
	CHECK(Ry[m12] == 0.0f);
	CHECK(Ry[m13] == 0.0f);
	CHECK(Ry[m14] == 0.0f);
	//CHECK(Ry[m15] == 1.0f);
	CHECK(Ry.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
