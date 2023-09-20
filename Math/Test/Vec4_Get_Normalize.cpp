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

TEST(Vec4_Get_Normalize, TestConfig::ALL)
{
#if Vec4_Get_Normalize

	Vec4 v1(1.0f, 2.0f, 3.0f, 5.0f);
	Vec4 v2;

	v2 = v1.getNorm();

	CHECK(eq(v2[x], 0.1601f, MATH_TOLERANCE));
	CHECK(eq(v2[y], 0.3202f, MATH_TOLERANCE));
	CHECK(eq(v2[z], 0.4803f, MATH_TOLERANCE));
	CHECK(eq(v2[w], 0.8006f, MATH_TOLERANCE));

	CHECK((v1[x] == 1.0f));
	CHECK((v1[y] == 2.0f));
	CHECK((v1[z] == 3.0f));
	CHECK((v1[w] == 5.0f));
#endif
} TEST_END

// ---  End of File ---------------
