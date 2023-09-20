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

TEST(Vec4_Assignment_Operator, TestConfig::ALL)
{
#if Vec4_Assignment_Operator

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);
	Vec4 B(10.0f, 11.0f, 12.0f, 13.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 11.0f);
	CHECK(B[z] == 12.0f);
	CHECK(B[w] == 13.0f);

	B = A;

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	CHECK(B[x] == 2.0f);
	CHECK(B[y] == 3.0f);
	CHECK(B[z] == 4.0f);
	CHECK(B[w] == 5.0f);

#endif
} TEST_END

// ---  End of File ---------------
