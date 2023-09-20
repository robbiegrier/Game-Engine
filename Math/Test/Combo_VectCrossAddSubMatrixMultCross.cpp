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

TEST( Combo_VectCrossAddSubMatrixMultCross, TestConfig::ALL)
{
#if Combo_VectCrossAddSubMatrixMultCross

	Vec3 A(1.0f, 2.0f, 3.0f);
	Vec3 B(10.0f, 11.0f, 12.0f);
	Vec3 C;

	CHECK(A[x] == 1.0f);
	CHECK(A[y] == 2.0f);
	CHECK(A[z] == 3.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 11.0f);
	CHECK(B[z] == 12.0f);

	C = A + (A - B).cross(B);
	Vec3 D = C.dot(A) * B;

	CHECK(D[x] == 140.0f);
	CHECK(D[y] == 154.0f);
	CHECK(D[z] == 168.0f);

	CHECK(C[x] == -8.0f);
	CHECK(C[y] == 20.0f);
	CHECK(C[z] == -6.0f);

	CHECK(A[x] == 1.0f);
	CHECK(A[y] == 2.0f);
	CHECK(A[z] == 3.0f);

	CHECK(B[x] == 10.0f);
	CHECK(B[y] == 11.0f);
	CHECK(B[z] == 12.0f);

	Mat4 M( Vec4(A,0), Vec4(B,0), Vec4(C,0), Vec4(D,1) );

	A = Vec3(1.0f, 0.0f, 4.0f).cross( Vec3(Vec4(B,1) * M)   );

	CHECK(A[x] == -2140.0f);
	CHECK(A[y] == 398.0f);
	CHECK(A[z] == 535.0f);


#endif
} TEST_END

// ---  End of File ---
