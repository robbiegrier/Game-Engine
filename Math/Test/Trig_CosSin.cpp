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

static float AngleTest_0_2PI_A[] // 0-2PI
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

TEST(Trig_CosSin, TestConfig::ALL)
{
#if Trig_CosSin

	float CosOut[sizeof(AngleTest_0_2PI_A) / sizeof(AngleTest_0_2PI_A[0])]{0};
	float SinOut[sizeof(AngleTest_0_2PI_A) / sizeof(AngleTest_0_2PI_A[0])]{0};

	float *pCosOut = &CosOut[0];
	float *pSinOut = &SinOut[0];
	for(float x : AngleTest_0_2PI_A)
	{
		Trig::cossin(*pCosOut++, *pSinOut++, x);
	}

	CHECK(eq(CosOut[0], 1.0000f, MATH_TOLERANCE));
	CHECK(eq(SinOut[0], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(CosOut[1], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(SinOut[1], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(CosOut[2], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(SinOut[2], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(CosOut[3], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(SinOut[3], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(CosOut[4], -0.0000f, MATH_TOLERANCE));
	CHECK(eq(SinOut[4], 1.0000f, MATH_TOLERANCE));
	CHECK(eq(CosOut[5], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(SinOut[5], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(CosOut[6], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(SinOut[6], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(CosOut[7], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(SinOut[7], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(CosOut[8], -1.0000f, MATH_TOLERANCE));
	CHECK(eq(SinOut[8], -0.0000f, MATH_TOLERANCE));
	CHECK(eq(CosOut[9], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(SinOut[9], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(CosOut[10], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(SinOut[10], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(CosOut[11], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(SinOut[11], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(CosOut[12], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(SinOut[12], -1.0000f, MATH_TOLERANCE));
	CHECK(eq(CosOut[13], 0.3827f, MATH_TOLERANCE));
	CHECK(eq(SinOut[13], -0.9239f, MATH_TOLERANCE));
	CHECK(eq(CosOut[14], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(SinOut[14], -0.7071f, MATH_TOLERANCE));
	CHECK(eq(CosOut[15], 0.9239f, MATH_TOLERANCE));
	CHECK(eq(SinOut[15], -0.3827f, MATH_TOLERANCE));
	CHECK(eq(CosOut[16], 1.0000f, MATH_TOLERANCE));
	CHECK(eq(SinOut[16], 0.0000f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
