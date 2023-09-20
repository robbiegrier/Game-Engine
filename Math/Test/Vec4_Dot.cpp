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

TEST(Vec4_Dot, TestConfig::ALL)
{
#if Vec4_Dot

	Vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 v2(4.0f, 5.0f, 6.0f, 7.0f);

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

	CHECK(v2[x] == 4.0f);
	CHECK(v2[y] == 5.0f);
	CHECK(v2[z] == 6.0f);
	CHECK(v2[w] == 7.0f);

	float value1;

	value1 = v1.dot(v2);
	CHECK(value1 == 60.0f);

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

	CHECK(v2[x] == 4.0f);
	CHECK(v2[y] == 5.0f);
	CHECK(v2[z] == 6.0f);
	CHECK(v2[w] == 7.0f);

#endif
} TEST_END

// ---  End of File ---------------
