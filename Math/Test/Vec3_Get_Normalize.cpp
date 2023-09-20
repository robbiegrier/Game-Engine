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

TEST(Vec3_Get_Normalize, TestConfig::ALL)
{
#if Vec3_Get_Normalize

	Vec3 v1(1.0f, 2.0f, 3.0f);
	Vec3 v2;

	v2 = v1.getNorm();

	CHECK(eq(v2[x], 0.2672f, MATH_TOLERANCE));
	CHECK(eq(v2[y], 0.5345f, MATH_TOLERANCE));
	CHECK(eq(v2[z], .80174f, MATH_TOLERANCE));

	CHECK((v1[x] == 1.0f));
	CHECK((v1[y] == 2.0f));
	CHECK((v1[z] == 3.0f));

#endif
} TEST_END

// ---  End of File ---------------
