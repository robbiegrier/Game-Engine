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

TEST(Mat4_GetInverse, TestConfig::ALL)
{
#if Mat4_GetInverse

	Mat4 Rx(Mat4::Rot1::X, 0.23f);
	Mat4 Ry(Mat4::Rot1::Y, -1.23f);
	Mat4 S(Mat4::Scale::XYZ, 4.0f, 5.0f, 2.0f);
	Mat4 T(Mat4::Trans::XYZ, -2.3f, 7.9f, 9.03f);

	Mat4 M = S * T * Rx * Ry * S;
	Mat4 N;

	N = M.getInv();

	Mat4 C;
	C = N * M;

	// verify C is identity
	const bool flag = C.isIdentity(MATH_TOLERANCE);

	CHECK(flag == true);

#endif
} TEST_END

// ---  End of File ---
