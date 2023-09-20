//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Vec4_IsEqualTrue_default, TestConfig::ALL)
{
#if Vec4_IsEqualTrue_default

	bool bvalue1;
	Vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 v2(1.0f, 2.0f, 3.05f, 4.0f);

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

	CHECK(v2[x] == 1.0f);
	CHECK(v2[y] == 2.0f);
	CHECK(v2[z] == 3.05f);
	CHECK(v2[w] == 4.0f);

	bvalue1 = v1.isEqual(v2);

	CHECK(bvalue1 == false);

	CHECK(v1[x] == 1.0f);
	CHECK(v1[y] == 2.0f);
	CHECK(v1[z] == 3.0f);
	CHECK(v1[w] == 4.0f);

	CHECK(v2[x] == 1.0f);
	CHECK(v2[y] == 2.0f);
	CHECK(v2[z] == 3.05f);
	CHECK(v2[w] == 4.0f);

#endif
} TEST_END

// ---  End of File ---------------
