//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq	Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Combo_VectLine, TestConfig::ALL)
{
#if Combo_VectLine
	Vec3 A(0.0f, 50.0f, 300.0f);
	Vec3 B(10.0f, 100.0f, 1000.0f);
	Vec3 C;

	CHECK(A[x] == 0.0f);
	CHECK(A[y] == 50.0f);
	CHECK(A[z] == 300.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 100.0f);
	CHECK(B[z] == 1000.0f);

	Vec3 Line;
	float t;

	t = 0.0f;
	Line = A + t * (B - A);

	CHECK(Line[x] == 0.0f);
	CHECK(Line[y] == 50.0f);
	CHECK(Line[z] == 300.0f);

	CHECK(A[x] == 0.0f);
	CHECK(A[y] == 50.0f);
	CHECK(A[z] == 300.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 100.0f);
	CHECK(B[z] == 1000.0f);

	t = 1.0f;
	Line = A + t * (B - A);

	CHECK(Line[x] == 10.0f);
	CHECK(Line[y] == 100.0f);
	CHECK(Line[z] == 1000.0f);

	CHECK(A[x] == 0.0f);
	CHECK(A[y] == 50.0f);
	CHECK(A[z] == 300.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 100.0f);
	CHECK(B[z] == 1000.0f);

	t = 0.5f;
	Line = A + t * (B - A);

	CHECK(Line[x] == 5.0f);
	CHECK(Line[y] == 75.0f);
	CHECK(Line[z] == 650.0f);

	CHECK(A[x] == 0.0f);
	CHECK(A[y] == 50.0f);
	CHECK(A[z] == 300.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 100.0f);
	CHECK(B[z] == 1000.0f);

#endif
} TEST_END


// ---  End of File ---
