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

TEST(Vec3_Len, TestConfig::ALL)
{
#if Vec3_Len

	Vec3 v1(1.0f, 2.0f, 3.0f);
	float value;

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);

	value = v1.len();

	CHECK(eq(value, 3.7416f, MATH_TOLERANCE));

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);

#endif
} TEST_END

// ---  End of File ---------------
