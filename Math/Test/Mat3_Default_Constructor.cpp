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

TEST(Mat3_Default_Constructor, TestConfig::ALL)
{
#if Mat3_Default_Constructor

	Mat3 M;

	CHECK(M[m0] == 0.0f);
	CHECK(M[m1] == 0.0f);
	CHECK(M[m2] == 0.0f);

	CHECK(M[m4] == 0.0f);
	CHECK(M[m5] == 0.0f);
	CHECK(M[m6] == 0.0f);

	CHECK(M[m8] == 0.0f);
	CHECK(M[m9] == 0.0f);
	CHECK(M[m10] == 0.0f);

#endif
} TEST_END

// --- End of File ---
