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

TEST(Vec4_Normalize, TestConfig::ALL)
{
#if Vec4_Normalize

	Vec4 v1(1.0f, 2.0f, 3.0f, 5.0f);

	v1.norm();

	CHECK(eq(v1[x], 0.1601f, MATH_TOLERANCE));
	CHECK(eq(v1[y], 0.3202f, MATH_TOLERANCE));
	CHECK(eq(v1[z], 0.4803f, MATH_TOLERANCE));
	CHECK(eq(v1[w], 0.8006f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---------------
