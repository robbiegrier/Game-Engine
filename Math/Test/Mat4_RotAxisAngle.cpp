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

TEST(Mat4_RotAxisAngle, TestConfig::ALL)
{
#if Mat4_RotAxisAngle	

	// Axis and Angle Type Constructor:
	Vec3  v11(2.0f, 53.0f, 24.0f);
	Mat4 m54(Mat4::Rot::AxisAngle, v11, MATH_PI3);

	// => Vec4  v11( 2.0f, 53.0f, 24.0f); \n"););
	// => Mat4 m54(ROT_AXIS_ANGLE, v11, MATH_PI3 );\n"););
	CHECK(eq(m54[m0], 0.5005f, MATH_TOLERANCE));
	CHECK(eq(m54[m1], 0.3726f, MATH_TOLERANCE));
	CHECK(eq(m54[m2], -0.7813f, MATH_TOLERANCE));
	CHECK(m54[m3] == 0.0f);
	CHECK(eq(m54[m4], -0.3413f, MATH_TOLERANCE));
	CHECK(eq(m54[m5], 0.9144f, MATH_TOLERANCE));
	CHECK(eq(m54[m6], 0.2174f, MATH_TOLERANCE));
	CHECK((m54[m7] == 0.0f));
	CHECK(eq(m54[m8], 0.7955f, MATH_TOLERANCE));
	CHECK(eq(m54[m9], 0.1579f, MATH_TOLERANCE));
	CHECK(eq(m54[m10], 0.5849f, MATH_TOLERANCE));
	CHECK((m54[m11] == 0.0f));
	CHECK((m54[m12] == 0.0f));
	CHECK((m54[m13] == 0.0f));
	CHECK((m54[m14] == 0.0f));
	//CHECK((m54[m15] == 1.0f));
	CHECK(m54.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
