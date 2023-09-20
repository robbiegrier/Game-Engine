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

TEST(Vec3_Get_Angle, TestConfig::ALL)
{
#if Vec3_Get_Angle

	float value;
	Vec3 v1(1.0f, 2.0f, 3.0f);
	Vec3 v2(1.80f, -2.0f, 2.0f);

	value = v1.getAngle(v2);

	CHECK(eq(value, 1.263f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---------------
