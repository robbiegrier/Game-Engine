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

TEST(Vec3_Set_Brackets_get, TestConfig::ALL)
{
#if Vec3_Set_Brackets_get

	Vec3 A(2.0f, 3.0f, 4.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	float tx, ty, tz;

	tx = A[x];
	ty = A[y];
	tz = A[z];

	CHECK(tx == 2.0f);
	CHECK(ty == 3.0f);
	CHECK(tz == 4.0f);

#endif
}  TEST_END

// ---  End of File ---------------
