//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq  		Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

static float AngleTest_0_2PI_B[] // 0-2PI
{
	0.0f,
	1.0f * MATH_PI8,
	2.0f * MATH_PI8,
	3.0f * MATH_PI8,
	4.0f * MATH_PI8,
	5.0f * MATH_PI8,
	6.0f * MATH_PI8,
	7.0f * MATH_PI8,
	8.0f * MATH_PI8,
	9.0f * MATH_PI8,
	10.0f * MATH_PI8,
	11.0f * MATH_PI8,
	12.0f * MATH_PI8,
	13.0f * MATH_PI8,
	14.0f * MATH_PI8,
	15.0f * MATH_PI8,
	16.0f * MATH_PI8
};

TEST(Trig_Sin, TestConfig::ALL)
{
#if Trig_Sin

	float OutAngle[sizeof(AngleTest_0_2PI_B) / sizeof(AngleTest_0_2PI_B[0])]{0};

	float *pOut = &OutAngle[0];
	for(float x : AngleTest_0_2PI_B)
	{
		*pOut++ = Trig::sin(x);
	}

	CHECK(eq(OutAngle[0], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[1], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[2], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[3], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[4], 1.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[5], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[6], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[7], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[8], -0.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[9], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[10], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[11], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[12], -1.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[13], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[14], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[15], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[16], 0.0000f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
