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

static float AngleTest_PI_2[] // -PI/2 to PI/2
{
	-4.0f * MATH_PI8,
	-3.0f * MATH_PI8,
	-2.0f * MATH_PI8,
	-1.0f * MATH_PI8,
	0.0f,
	1.0f * MATH_PI8,
	2.0f * MATH_PI8,
	3.0f * MATH_PI8,
	4.0f * MATH_PI8,
};

TEST(Trig_ATan, TestConfig::ALL)
{
#if Trig_ATan

	float OutAngle[sizeof(AngleTest_PI_2) / sizeof(AngleTest_PI_2[0])]{0};

	float *pOut = &OutAngle[0];
	for(float x : AngleTest_PI_2)
	{
		*pOut++ = Trig::atan(x);
	}

	CHECK(eq(OutAngle[0], -1.0039f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[1], -0.8670f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[2], -0.6658f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[3], -0.3742f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[4], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[5], 0.3742f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[6], 0.6658f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[7], 0.8670f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[8], 1.0039f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
