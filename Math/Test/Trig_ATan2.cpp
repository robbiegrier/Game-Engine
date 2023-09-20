//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq  		Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Trig_ATan2, TestConfig::ALL)
{
#if Trig_ATan2

	float angle;
	float x;
	float y;

	x = -1.0f;
	y = 0.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, 3.1416f, MATH_TOLERANCE));

	y = 0.01f;
	x = -1.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, 3.1316f, MATH_TOLERANCE));

	y = -0.01f;
	x = -1.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, -3.1316f, MATH_TOLERANCE));

	y = 1.0f;
	x = 0.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, 1.5708f, MATH_TOLERANCE));

	y = -1.0f;
	x = 0.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, -1.5708f, MATH_TOLERANCE));

	y = 0.0f;
	x = 1.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, 0.0000f, MATH_TOLERANCE));

	y = 0.01f;
	x = 1.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, 0.0100f, MATH_TOLERANCE));

	y = -0.01f;
	x = 1.0f;
	angle = Trig::atan2(y, x);
	CHECK(eq(angle, -0.0100f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---
