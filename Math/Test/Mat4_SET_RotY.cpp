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

TEST(Mat4_SET_RotY, TestConfig::ALL)
{
#if Mat4_SET_RotY

	Vec4 V0(17.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vec4 V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vec4 V3(13.0f, 14.0f, 15.0f, 16.0f);

	Mat4 Ry(V0, V1, V2, V3);

	CHECK(Ry[m0] == 17.0f);
	CHECK(Ry[m1] == 2.0f);
	CHECK(Ry[m2] == 3.0f);
	CHECK(Ry[m3] == 4.0f);
	CHECK(Ry[m4] == 5.0f);
	CHECK(Ry[m5] == 6.0f);
	CHECK(Ry[m6] == 7.0f);
	CHECK(Ry[m7] == 8.0f);
	CHECK(Ry[m8] == 9.0f);
	CHECK(Ry[m9] == 10.0f);
	CHECK(Ry[m10] == 11.0f);
	CHECK(Ry[m11] == 12.0f);
	CHECK(Ry[m12] == 13.0f);
	CHECK(Ry[m13] == 14.0f);
	CHECK(Ry[m14] == 15.0f);
	CHECK(Ry[m15] == 16.0f);

	Ry.set(Mat4::Rot1::Y, 1.0471975512f);

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
