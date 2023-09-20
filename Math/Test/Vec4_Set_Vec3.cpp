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

TEST(Vec4_Set_Vec3, TestConfig::ALL)
{
#if Vec4_Set_Vec3

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	Vec3 B(10.0f, 20.0f, 30.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);

	A.set(B, 1);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);

	CHECK(A[x] == 10.0f);
	CHECK(A[y] == 20.0f);
	CHECK(A[z] == 30.0f);
	CHECK(A[w] == 1.0f);

	A.set(B,4);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);

	CHECK(A[x] == 10.0f);
	CHECK(A[y] == 20.0f);
	CHECK(A[z] == 30.0f);
	CHECK(A[w] == 4.0f);

	A.set(B, 0);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 20.0f);
	CHECK(B[z] == 30.0f);

	CHECK(A[x] == 10.0f);
	CHECK(A[y] == 20.0f);
	CHECK(A[z] == 30.0f);
	CHECK(A[w] == 0.0f);

#endif
} TEST_END

// ---  End of File ---------------
