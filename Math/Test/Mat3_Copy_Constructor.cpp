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

TEST(Mat3_Copy_Constructor, TestConfig::ALL)
{
#if Mat3_Copy_Constructor

	Vec3 V0(1.0f, 2.0f, 3.0f);
	Vec3 V1(7.0f, 6.0f, 5.0f);
	Vec3 V2(-4.0f, -2.0f, -1.0f);

	CHECK(V0[x] == 1.0f);
	CHECK(V0[y] == 2.0f);
	CHECK(V0[z] == 3.0f);
	
	CHECK(V1[x] == 7.0f);
	CHECK(V1[y] == 6.0f);
	CHECK(V1[z] == 5.0f);
	
	CHECK(V2[x] == -4.0f);
	CHECK(V2[y] == -2.0f);
	CHECK(V2[z] == -1.0f);

	Mat3 M(V0, V1, V2);

	Mat3 N(M);

	CHECK(N[m0] == 1.0f);
	CHECK(N[m1] == 2.0f);
	CHECK(N[m2] == 3.0f);
	
	CHECK(N[m4] == 7.0f);
	CHECK(N[m5] == 6.0f);
	CHECK(N[m6] == 5.0f);

	CHECK(N[m8] == -4.0f);
	CHECK(N[m9] == -2.0f);
	CHECK(N[m10] == -1.0f);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);

	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);

#endif
} TEST_END

// --- End of File ---
