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

TEST(Vec4_Set_Vec4, TestConfig::ALL)
{
#if Vec4_Set_Vec4

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	Vec4 B(10.0f, 20.0f, 30.0f, 50.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);
	CHECK(B[w] == 50.0f);

	A.set(B);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);
	CHECK(B[w] == 50.0f);

	CHECK(A[x] == 10.0f);
	CHECK(A[y] == 20.0f);
	CHECK(A[z] == 30.0f);
	CHECK(A[W] == 50.0f);

#endif
} TEST_END

// ---  End of File ---------------
