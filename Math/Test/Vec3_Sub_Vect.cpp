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

TEST(Vec3_Sub_Vect, TestConfig::ALL)
{
#if Vec3_Sub_Vect

	Vec3 A(2.0f, 3.0f, 4.0f);
	Vec3 B(21.0f, 31.0f, 41.0f);
	Vec3 C;

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(B[x] == 21.0f);
	CHECK(B[y] == 31.0f);
	CHECK(B[z] == 41.0f);

	C = B - A;

	CHECK(C[x] == 19.0f);
	CHECK(C[y] == 28.0f);
	CHECK(C[z] == 37.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(B[x] == 21.0f);
	CHECK(B[y] == 31.0f);
	CHECK(B[z] == 41.0f);

#endif
} TEST_END

// ---  End of File ---------------
