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

TEST(Vec3_Mult_Mat3,TestConfig::ALL)
{
#if Vec3_Mult_Mat3

	Vec3 vA(2.0f, 3.0f, 4.0f);
	Vec3 vOut;

	CHECK(vA[x] == 2.0f);
	CHECK(vA[y] == 3.0f);
	CHECK(vA[z] == 4.0f);

	Vec3 V0(1.0f, 2.0f, 3.0f);
	Vec3 V1(7.0f, 6.0f, 5.0f);
	Vec3 V2(-4.0f, -2.0f, -1.0f);

	Mat3 M(V0, V1, V2);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);

	vOut = vA * M;

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);

	CHECK(vA[x] == 2.0f);
	CHECK(vA[y] == 3.0f);
	CHECK(vA[z] == 4.0f);

	CHECK(eq(vOut[x], 7.0f, MATH_TOLERANCE));
	CHECK(eq(vOut[y], 14.0f, MATH_TOLERANCE));
	CHECK(eq(vOut[z], 17.0f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---------------
