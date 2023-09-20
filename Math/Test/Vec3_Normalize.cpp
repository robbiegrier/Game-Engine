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

TEST(Vec3_Normalize, TestConfig::ALL)
{
#if Vec3_Normalize

	Vec3 v1(1.0f, 2.0f, 3.0f);

	v1.norm();

	CHECK(eq(v1[x], 0.2672f, MATH_TOLERANCE));
	CHECK(eq(v1[y], 0.5345f, MATH_TOLERANCE));
	CHECK(eq(v1[z], 0.80174f, MATH_TOLERANCE));


#endif
} TEST_END

// ---  End of File ---------------
