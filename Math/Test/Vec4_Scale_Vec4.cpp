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

TEST(Vec4_Scale_Vec4, TestConfig::ALL)
{
#if Vec4_Scale_Vec4

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);
	const float s = 5.0f;
	Vec4 C;

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

	CHECK(C[x] == 0.0f);
	CHECK(C[y] == 0.0f);
	CHECK(C[z] == 0.0f);
	CHECK(C[w] == 0.0f);

	C = s * A;

	CHECK(C[x] == 10.0f);
	CHECK(C[y] == 15.0f);
	CHECK(C[z] == 20.0f);
	CHECK(C[w] == 25.0f);

	CHECK(A[x] == 2.0f);
	CHECK(A[y] == 3.0f);
	CHECK(A[z] == 4.0f);
	CHECK(A[w] == 5.0f);

#endif
} TEST_END

// ---  End of File ---------------
