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

static float Input_Minus_11[]
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

TEST(Trig_ASin, TestConfig::ALL)
{
#if Trig_ASin

	float OutAngle[sizeof(Input_Minus_11) / sizeof(Input_Minus_11[0])]{0};

	float *pOut = &OutAngle[0];
	for(float x : Input_Minus_11)
	{
		*pOut++ = Trig::asin(x);
	}

	CHECK(eq(OutAngle[0], -1.5708f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[1], -0.8481f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[2], -0.5236f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[3], -0.2527f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[4], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[5], 0.2527f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[6], 0.5236f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[7], 0.8481f, MATH_TOLERANCE));
	CHECK(eq(OutAngle[8], 1.5708f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
