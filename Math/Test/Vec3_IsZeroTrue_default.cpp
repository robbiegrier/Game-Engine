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

TEST(Vec3_IsZeroTrue_default, TestConfig::ALL)
{
#if Vec3_IsZeroTrue_default

	bool bvalue1;
	Vec3 v1(0.0f, 0.0f, 0.0f);

	bvalue1 = v1.isZero();

	CHECK(bvalue1 == true);

#endif
} TEST_END


// ---  End of File ---------------
