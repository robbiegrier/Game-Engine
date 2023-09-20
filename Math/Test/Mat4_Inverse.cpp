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

TEST( Mat4_Inverse, TestConfig::ALL)
{
#if Mat4_Inverse

	Mat4 Rx(Mat4::Rot1::X, 0.23f);
	Mat4 Ry(Mat4::Rot1::Y, -1.23f);
	Mat4 T(Mat4::Trans::XYZ, 3.4f, 2.9f, -9.0f);

	Mat4 M = Rx * Ry * T;
	Mat4 N(M);

	M.inv();

	Mat4 C;
	C = N * M;

	// verify C is identity

	const bool flag = C.isIdentity(MATH_TOLERANCE);

	CHECK(flag == true);

#endif
} TEST_END

// ---  End of File ---
