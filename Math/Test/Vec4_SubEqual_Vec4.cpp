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

TEST(Vec4_SubEqual_Vec4, TestConfig::ALL)
{
#if Vec4_SubEqual_Vec4

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0F);
	Vec4 B(21.0f, 31.0f, 41.0f, 51.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	CHECK(B[x] == 21.0f);
	CHECK(B[y] == 31.0f);
	CHECK(B[z] == 41.0f);
	CHECK(B[w] == 51.0f);

	B -= A;

	CHECK(B[x] == 19.0f);
	CHECK(B[y] == 28.0f);
	CHECK(B[z] == 37.0f);
	CHECK(B[w] == 46.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

#endif

} TEST_END

// ---  End of File ---------------
