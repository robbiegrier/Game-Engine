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

TEST(Vec4_Constructor_Vec3, TestConfig::ALL)
{
#if Vec4_Constructor_Vec3

	Vec3 A(2.0f, 3.0f, 4.0f);
	Vec4 B(A,1);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(B[x] == A[x]);
	CHECK(B[y] == A[y]);
	CHECK(B[z] == A[z]);
	CHECK(B[w] == 1.0f);

	Vec4 C(A,5);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(C[x] == A[x]);
	CHECK(C[y] == A[y]);
	CHECK(C[z] == A[z]);
	CHECK(C[w] == 5.0f);

	Vec4 D(A, 0);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);

	CHECK(D[x] == A[x]);
	CHECK(D[y] == A[y]);
	CHECK(D[z] == A[z]);
	CHECK(D[w] == 0.0f);


#endif
} TEST_END

// ---  End of File ---------------
