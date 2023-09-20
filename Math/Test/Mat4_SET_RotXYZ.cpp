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

TEST(Mat4_SET_RotXYZ, TestConfig::ALL)
{
#if Mat4_SET_RotXYZ

	Mat4 Rx;
	Mat4 Ry;
	Mat4 Rz;

	// Rot_X Type Constructor:
	Rx.set(	Mat4::Rot1::X, MATH_PI3);

	CHECK( Rx[m0] ==  1.0f );
	CHECK( Rx[m1] ==  0.0f );
	CHECK( Rx[m2] ==  0.0f );
	CHECK( Rx[m3] ==  0.0f );
	CHECK( Rx[m4] ==  0.0f );
	CHECK( eq(Rx[m5],  0.5f,  MATH_TOLERANCE) );
	CHECK( eq(Rx[m6], 0.866f, MATH_TOLERANCE) );
	CHECK( Rx[m7] == 0.0f );
	CHECK( Rx[m8] == 0.0f );
	CHECK( eq(Rx[m9],-0.866f, MATH_TOLERANCE) );
	CHECK( eq(Rx[m10],0.5f  , MATH_TOLERANCE) );
	CHECK( Rx[m11] == 0.0f );
	CHECK( Rx[m12] == 0.0f );
	CHECK( Rx[m13] == 0.0f );
	CHECK( Rx[m14] == 0.0f );
	//CHECK( Rx[m15] == 1.0f );
	CHECK(Rx.privHasHint() == true);

	Ry.set(Mat4::Rot1::Y, MATH_7PI8);

	CHECK( eq(Ry[m0],-0.9238f,  MATH_TOLERANCE) );
	CHECK( Ry[m1] ==  0.0f );
	CHECK( eq(Ry[m2],-0.3826f,MATH_TOLERANCE) );
	CHECK( Ry[m3] == 0.0f );
	CHECK( Ry[m4] == 0.0f );
	CHECK( Ry[m5] == 1.0f );
	CHECK( Ry[m6] == 0.0f );
	CHECK( Ry[m7] == 0.0f );
	CHECK( eq(Ry[m8],0.3826f,MATH_TOLERANCE) );
	CHECK( Ry[m9] ==  0.0f );
	CHECK( eq(Ry[m10],-0.9238f,   MATH_TOLERANCE) );
	CHECK( Ry[m11] == 0.0f );
	CHECK( Ry[m12] == 0.0f );
	CHECK( Ry[m13] == 0.0f );
	CHECK( Ry[m14] == 0.0f );
	//CHECK( Ry[m15] == 1.0f );
	CHECK(Ry.privHasHint() == true);

	Rz.set(Mat4::Rot1::Z, MATH_PI2);

	CHECK( eq(Rz[m0],0.0f, MATH_TOLERANCE) );
	CHECK( eq(Rz[m1],1.0f, MATH_TOLERANCE) );
	CHECK( Rz[m2] == 0.0f );
	CHECK( Rz[m3] == 0.0f );
	CHECK( eq(Rz[m4],-1.0f, MATH_TOLERANCE) );
	CHECK( eq(Rz[m5], 0.0f,  MATH_TOLERANCE) );
	CHECK( Rz[m6] == 0.0f );
	CHECK( Rz[m7] == 0.0f );
	CHECK( Rz[m8] == 0.0f );
	CHECK( Rz[m9] == 0.0f );
	CHECK( Rz[m10] == 1.0f );
	CHECK( Rz[m11] == 0.0f );
	CHECK( Rz[m12] == 0.0f );
	CHECK( Rz[m13] == 0.0f );
	CHECK( Rz[m14] == 0.0f );
	//CHECK( Rz[m15] == 1.0f );
	CHECK(Rz.privHasHint() == true);

	Mat4 Rxyz;

	Rxyz = Rx * Ry * Rz;

	Mat4 mTmp;

	mTmp.set(Mat4::Rot3::XYZ, MATH_PI3,MATH_7PI8, MATH_PI2);

	CHECK( eq( Rxyz[m0], mTmp[m0], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m1], mTmp[m1], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m2], mTmp[m2], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m3], mTmp[m3], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m4], mTmp[m4], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m5], mTmp[m5], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m6], mTmp[m6], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m7], mTmp[m7], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m8], mTmp[m8], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m9], mTmp[m9], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m10], mTmp[m10], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m11], mTmp[m11], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m12], mTmp[m12], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m13], mTmp[m13], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m14], mTmp[m14], MATH_TOLERANCE) );
	CHECK( eq( Rxyz[m15], mTmp[m15], MATH_TOLERANCE) );

#endif
} TEST_END

// ---  End of File ---------------
