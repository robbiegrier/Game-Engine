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

TEST(Vec4_Len, TestConfig::ALL)
{
#if Vec4_Len

	Vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	float value;

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

	value = v1.len();

	CHECK(eq(value, 5.4772f, MATH_TOLERANCE));

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

#endif
} TEST_END

// ---  End of File ---------------
