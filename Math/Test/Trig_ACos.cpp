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

static float Input_Minus_1[]
{
	-1.0f + 0 * 0.25f,
	-1.0f + 1 * 0.25f,
	-1.0f + 2 * 0.25f,
	-1.0f + 3 * 0.25f,
	-1.0f + 4 * 0.25f,
	-1.0f + 5 * 0.25f,
	-1.0f + 6 * 0.25f,
	-1.0f + 7 * 0.25f,
	-1.0f + 8 * 0.25f
};

TEST(Trig_ACos, TestConfig::ALL)
{
#if Trig_ACos

	float OutAngle[sizeof(Input_Minus_1) / sizeof(Input_Minus_1[0])]{0};

	float *pOut = &OutAngle[0];
	for(float x : Input_Minus_1)
	{
		*pOut++ = Trig::acos(x);
	}

	CHECK(eq(OutAngle[0], 3.1416f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[1], 2.4189f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[2], 2.0944f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[3], 1.8235f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[4], 1.5708f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[5], 1.3181f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[6], 1.0472f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[7], 0.7227f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[8], 0.0000f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
