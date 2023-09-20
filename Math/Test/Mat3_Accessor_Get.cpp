//---------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

//---------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq	Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------

TEST(Mat3_Accessor_Get, TestConfig::ALL)
{
#if Mat3_Accessor_Get

	Vec3 V0(1.0f, 2.0f, 3.0f);
	Vec3 V1(7.0f, 6.0f, 5.0f);
	Vec3 V2(-4.0f, -2.0f, -1.0f);

	Mat3 M(V0, V1, V2);

	CHECK(M.m0() == 1.0f);
	CHECK(M.m1() == 2.0f);
	CHECK(M.m2() == 3.0f);
	
	CHECK(M.m4() == 7.0f);
	CHECK(M.m5() == 6.0f);
	CHECK(M.m6() == 5.0f);
	
	CHECK(M.m8() == -4.0f);
	CHECK(M.m9() == -2.0f);
	CHECK(M.m10() == -1.0f);
	
	float t0, t1, t2, t4, t5, t6,  t8, t9, t10;

	t0 = M.m0();
	t1 = M.m1();
	t2 = M.m2();

	t4 = M.m4();
	t5 = M.m5();
	t6 = M.m6();

	t8 = M.m8();
	t9 = M.m9();
	t10 = M.m10();

	CHECK(t0 == 1.0f);
	CHECK(t1 == 2.0f);
	CHECK(t2 == 3.0f);

	CHECK(t4 == 7.0f);
	CHECK(t5 == 6.0f);
	CHECK(t6 == 5.0f);

	CHECK(t8 == -4.0f);
	CHECK(t9 == -2.0f);
	CHECK(t10 == -1.0f);

#endif
} TEST_END

// ---  End of File ---
