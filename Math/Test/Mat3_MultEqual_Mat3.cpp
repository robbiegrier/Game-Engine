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

TEST(Mat3_MultEqual_Mat3, TestConfig::ALL)
{
#if Mat3_MultEqual_Mat3

	Vec3 v0(1.0f, 2.0f, 3.0f);
	Vec3 v1(5.0f, 6.0f, 7.0f);
	Vec3 v2(9.0f, 10.0f, 11.0f);
	Vec3 v3(13.0f, 14.0f, 15.0f);
	Vec3 v4(10.0f, 20.0f, 30.0f);
	Vec3 v5(50.0f, 60.0f, 70.0f);
	Vec3 v6(90.0f, 100.0f, 110.0f);
	Vec3 v7(130.0f, 140.0f, 150.0f);

	Mat3 A(v0, v1, v2);
	Mat3 B(v4, v5, v6);
	Mat3 C;

	CHECK((A[m0] == 1.0f));
	CHECK((A[m1] == 2.0f));
	CHECK((A[m2] == 3.0f));

	CHECK((A[m4] == 5.0f));
	CHECK((A[m5] == 6.0f));
	CHECK((A[m6] == 7.0f));
	
	CHECK((A[m8] == 9.0f));
	CHECK((A[m9] == 10.0f));
	CHECK((A[m10] == 11.0f));


	CHECK((B[m0] == 10.0f));
	CHECK((B[m1] == 20.0f));
	CHECK((B[m2] == 30.0f));

	CHECK((B[m4] == 50.0f));
	CHECK((B[m5] == 60.0f));
	CHECK((B[m6] == 70.0f));
	
	CHECK((B[m8] == 90.0f));
	CHECK((B[m9] == 100.0f));
	CHECK((B[m10] == 110.0f));


	A *= B;

	CHECK((B[m0] == 10.0f));
	CHECK((B[m1] == 20.0f));
	CHECK((B[m2] == 30.0f));

	CHECK((B[m4] == 50.0f));
	CHECK((B[m5] == 60.0f));
	CHECK((B[m6] == 70.0f));

	CHECK((B[m8] == 90.0f));
	CHECK((B[m9] == 100.0f));
	CHECK((B[m10] == 110.0f));


	CHECK((A[m0] == 380.0f));
	CHECK((A[m1] == 440.0f));
	CHECK((A[m2] == 500.0f));

	CHECK((A[m4] == 980.0f));
	CHECK((A[m5] == 1160.0f));
	CHECK((A[m6] == 1340.0f));

	CHECK((A[m8] == 1580.0f));
	CHECK((A[m9] == 1880.0f));
	CHECK((A[m10] == 2180.0f));


#endif
} TEST_END

// ---  End of File ---
