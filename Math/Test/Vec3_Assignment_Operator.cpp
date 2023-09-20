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

TEST(Vec3_Assignment_Operator, TestConfig::ALL)
{
#if Vec3_Assignment_Operator

	Vec3 A(2.0f, 3.0f, 4.0f);
	Vec3 B(10.0f, 11.0f, 12.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 11.0f);
	CHECK(B[z] == 12.0f);

	B = A;

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(B[x] == A[x]);
	CHECK(B[y] == A[y]);
	CHECK(B[z] == A[z]);

#endif
} TEST_END

// ---  End of File ---------------
