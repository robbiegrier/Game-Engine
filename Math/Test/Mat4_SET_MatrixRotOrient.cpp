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

TEST(Mat4_SET_MatrixRotOrient, TestConfig::ALL)
{
#if Mat4_SET_MatrixRotOrient

	// Orientation Type Constructor:
	Vec3  v15( 2.0f, 53.0f, 24.0f);
	Vec3  v16( 0.0f, -24.0f, 53.0f);
	Mat4 m56;
	m56.set(Mat4::Orient::LocalToWorld, v15, v16 );

	CHECK( eq(m56[m0],-0.9994f, MATH_TOLERANCE) );
	CHECK( eq(m56[m1], 0.0313f, MATH_TOLERANCE) );
	CHECK( eq(m56[m2], 0.0142f, MATH_TOLERANCE) );
	CHECK( (m56[m3] == 0.0f) );
	CHECK( eq(m56[m4], 0.0000f, MATH_TOLERANCE) );
	CHECK( eq(m56[m5],-0.4125f, MATH_TOLERANCE) );
	CHECK( eq(m56[m6], 0.9110f, MATH_TOLERANCE) );
	CHECK( (m56[m7] == 0.0f) );
	CHECK( eq(m56[m8], 0.0344f, MATH_TOLERANCE) );
	CHECK( eq(m56[m9], 0.9104f, MATH_TOLERANCE) );
	CHECK( eq(m56[m10], 0.4123f, MATH_TOLERANCE) );
	CHECK( (m56[m11] == 0.0f) );
	CHECK( (m56[m12] == 0.0f) );
	CHECK( (m56[m13] == 0.0f) );
	CHECK( (m56[m14] == 0.0f) );
	//CHECK( (m56[m15] == 1.0f) );
	CHECK(m56.privHasHint() == true);

#endif
} TEST_END

// ---  End of File ---------------
