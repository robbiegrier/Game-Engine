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

TEST(Vec4_Unary_Plus, TestConfig::ALL)
{
#if Vec4_Unary_Plus

	Vec4 A(-2.0f, -3.0f, -4.0f, -5.0f);
	Vec4 B(0, 0, 0, 0);

	CHECK(A[x] == -2.0f);
	CHECK(A[y] == -3.0f);
	CHECK(A[z] == -4.0f);
	CHECK(A[w] == -5.0f);

	CHECK(B[x] == 0.0f);
	CHECK(B[y] == 0.0f);
	CHECK(B[z] == 0.0f);
	CHECK(B[w] == 0.0f);

	B = +A;

	CHECK(A[x] == -2.0f);
	CHECK(A[y] == -3.0f);
	CHECK(A[z] == -4.0f);
	CHECK(A[w] == -5.0f);

	CHECK(B[x] == -2.0f);
	CHECK(B[y] == -3.0f);
	CHECK(B[z] == -4.0f);
	CHECK(B[w] == -5.0f);

#endif
} TEST_END

// ---  End of File ---------------

