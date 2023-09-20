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

TEST(Mat3_Scale_Equals, TestConfig::ALL)
{
#if Mat3_Scale_Equals

	Vec3 V0(1.0f, 2.0f, 3.0f);
	Vec3 V1(5.0f, 6.0f, 7.0f);
	Vec3 V2(9.0f, 10.0f, 11.0f);

	Mat3 A(V0, V1, V2);
	const float scale = 10.0f;

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);

	A *= scale;

	CHECK(A[m0] == 10.0f);
	CHECK(A[m1] == 20.0f);
	CHECK(A[m2] == 30.0f);
	
	CHECK(A[m4] == 50.0f);
	CHECK(A[m5] == 60.0f);
	CHECK(A[m6] == 70.0f);
	
	CHECK(A[m8] == 90.0f);
	CHECK(A[m9] == 100.0f);
	CHECK(A[m10] == 110.0f);

#endif
} TEST_END

// ---  End of File ---
