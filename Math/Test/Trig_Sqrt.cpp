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

static float Input_Sqrt[]
{
	0.0f,
	0.5f,
	0.8f,
	2.0f,
	4.0f,
	1000.0f,
	5000.0f,
	10000.0f
};

TEST(Trig_Sqrt, TestConfig::ALL)
{
#if Trig_Sqrt

	float Out[sizeof(Input_Sqrt) / sizeof(Input_Sqrt[0])]{0};

	float *pOut = &Out[0];
	for(float x : Input_Sqrt)
	{
		*pOut++ = Trig::sqrt(x);
	}

	CHECK(eq(Out[0], 0.0000f, MATH_TOLERANCE));
	CHECK(eq(Out[1], 0.7071f, MATH_TOLERANCE));
	CHECK(eq(Out[2], 0.8944f, MATH_TOLERANCE));
	CHECK(eq(Out[3], 1.4142f, MATH_TOLERANCE));
	CHECK(eq(Out[4], 2.0000f, MATH_TOLERANCE));
	CHECK(eq(Out[5], 31.6228f, MATH_TOLERANCE));
	CHECK(eq(Out[6], 70.7107f, MATH_TOLERANCE));
	CHECK(eq(Out[7], 100.0000f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
