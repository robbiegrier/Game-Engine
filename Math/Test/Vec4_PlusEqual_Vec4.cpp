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

TEST(Vec4_PlusEqual_Vec4, TestConfig::ALL)
{
#if Vec4_PlusEqual_Vec4

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);
	Vec4 B(21.0f, 31.0f, 41.0f, 51.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	CHECK(B[x] == 21.0f);
	CHECK(B[y] == 31.0f);
	CHECK(B[z] == 41.0f);
	CHECK(B[w] == 51.0f);

	A += B;

	CHECK(A[x] == 23.0f);
	CHECK(A[y] == 34.0f);
	CHECK(A[z] == 45.0f);
	CHECK(A[w] == 56.0f);

	CHECK(B[x] == 21.0f);
	CHECK(B[y] == 31.0f);
	CHECK(B[z] == 41.0f);
	CHECK(B[w] == 51.0f);


#endif
} TEST_END

// ---  End of File ---------------
