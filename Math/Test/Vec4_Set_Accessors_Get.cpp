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

TEST(Vec4_Set_Accessors_Get, TestConfig::ALL)
{
#if Vec4_Set_Accessors_Get

	Vec4 A(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(A.x() == 2.0f);
	CHECK(A.y() == 3.0f);
	CHECK(A.z() == 4.0f);
	CHECK(A.w() == 5.0f);

	float tx, ty, tz, tw;

	A.x(13.0f);
	A.y(14.0f);
	A.z(15.0f);
	A.w(16.0f);

	CHECK(A.x() == 13.0f);
	CHECK(A.y() == 14.0f);
	CHECK(A.z() == 15.0f);
	CHECK(A.w() == 16.0f);

	tx = A.x();
	ty = A.y();
	tz = A.z();
	tw = A.w();

	CHECK(tx == 13.0f);
	CHECK(ty == 14.0f);
	CHECK(tz == 15.0f);
	CHECK(tw == 16.0f);

#endif
} TEST_END

// ---  End of File ---------------
