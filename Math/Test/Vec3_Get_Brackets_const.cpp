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

#if Vec3_Get_Brackets_const
float constTest(const Vec3 &t)
{
	return (t[x] + t[y] + t[z] );
}
#endif

TEST(Vec3_Get_Brackets_const, TestConfig::ALL)
{
#if Vec3_Get_Brackets_const
	Vec3 A(2.0f, 3.0f, 4.0f);

	const float sum = constTest(A);

	CHECK(sum == 9.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

#endif
}  TEST_END

// ---  End of File ---------------
