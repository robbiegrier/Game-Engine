//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#include "MathApp.h"
#define eq  		Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

#if Vec3App_LERP_Array

Vec3 vSource[] =
{
	Vec3( 1.00f, 2.0f,   3.0f),
	Vec3(21.00f, 12.0f, 23.0f),
	Vec3(13.00f, 24.0f, 33.0f),
	Vec3( 0.50f, 22.0f, 43.0f),
	Vec3( 1.30f, 23.0f, 35.0f),
	Vec3(15.00f, 72.0f, 35.0f),
	Vec3(21.00f, 92.0f, 33.0f),
};

Vec3 vTarget[] =
{
	Vec3( -1.00f, 12.0f,   13.0f),
	Vec3(213.00f, -12.0f, 263.0f),
	Vec3(133.00f, -24.0f, 363.0f),
	Vec3  (3.50f, -22.0f, 453.0f),
	Vec3( 13.30f, -23.0f, 365.0f),
	Vec3(153.00f, -72.0f, 735.0f),
	Vec3(231.00f, -92.0f, 733.0f),
};

#define numVects  ((sizeof(vSource))/(sizeof(vSource[0])) )

Vec3 vResult[] =
{
	Vec3(-0.300000f, 8.500000f, 9.500000f),
	Vec3(145.799988f, -3.599999f, 179.000000f),
	Vec3(91.000000f, -7.199999f, 247.499985f),
	Vec3(2.450000f, -6.599998f, 309.500000f),
	Vec3(9.099999f, -6.900000f, 249.499985f),
	Vec3(104.699997f, -21.599998f, 489.999969f),
	Vec3(157.500000f, -27.599998f, 487.999969f),

};
#endif

TEST(Vec3App_LERP_Array, TestConfig::ALL)
{
#if Vec3App_LERP_Array

	Vec3 vOut[numVects];
	int i;

	Vec3App::LerpArray(vOut, vSource, vTarget, 0.65f, numVects);

	i = 0;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 1;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 2;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 3;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 4;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 5;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

	i = 6;
	CHECK(eq(vOut[i][x], vResult[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vResult[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vResult[i][z], MATH_TOLERANCE));

#endif	
} TEST_END


#if Vec3App_LERP_Array0
Vec3 vSourceA[] =
{
	Vec3(1.00f, 2.0f,   3.0f),
	Vec3(21.00f, 12.0f, 23.0f),
	Vec3(13.00f, 24.0f, 33.0f),
	Vec3(0.50f, 22.0f, 43.0f),
	Vec3(1.30f, 23.0f, 35.0f),
	Vec3(15.00f, 72.0f, 35.0f),
	Vec3(21.00f, 92.0f, 33.0f),
};

Vec3 vTargetA[] =
{
	Vec3(-1.00f,  12.0f,   13.0f),
	Vec3(213.00f, -12.0f, 263.0f),
	Vec3(133.00f, -24.0f, 363.0f),
	Vec3(3.50f,   -22.0f, 453.0f),
	Vec3(13.30f,  -23.0f, 365.0f),
	Vec3(153.00f, -72.0f, 735.0f),
	Vec3(231.00f, -92.0f, 733.0f),
};

#define numVectsA  ((sizeof(vSourceA))/(sizeof(vSourceA[0])) )

Vec3 vResultA[] =
{
	Vec3(-0.300000f, 8.500000f, 9.500000f),
	Vec3(145.799988f, -3.599999f, 179.000000f),
	Vec3(91.000000f, -7.199999f, 247.499985f),
	Vec3(2.450000f, -6.599998f, 309.500000f),
	Vec3(9.099999f, -6.900000f, 249.499985f),
	Vec3(104.699997f, -21.599998f, 489.999969f),
	Vec3(157.500000f, -27.599998f, 487.999969f),

};
#endif

TEST(Vec3App_LERP_Array0, TestConfig::ALL)
{
#if Vec3App_LERP_Array0

	Vec3 vOut[numVectsA];

	Vec3App::LerpArray(vOut, vSourceA, vTargetA, 0.0f, numVectsA);

	int i;

	i = 0;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));
	

	i = 1;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));
	

	i = 2;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));


	i = 3;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));


	i = 4;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));
	

	i = 5;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));


	i = 6;
	CHECK(eq(vOut[i][x], vSourceA[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vSourceA[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vSourceA[i][z], MATH_TOLERANCE));


#endif
} TEST_END


#if Vec3App_LERP_Array1

Vec3 vSourceB[] =
{
	Vec3(1.00f, 2.0f,    3.0f),
	Vec3(21.00f, 12.0f, 23.0f),
	Vec3(13.00f, 24.0f, 33.0f),
	Vec3(0.50f, 22.0f, 43.0f),
	Vec3(1.30f, 23.0f, 35.0f),
	Vec3(15.00f, 72.0f, 35.0f),
	Vec3(21.00f, 92.0f, 33.0f),
};

Vec3 vTargetB[] =
{
	Vec3(-1.00f, 12.0f,   13.0f),
	Vec3(213.00f, -12.0f, 263.0f),
	Vec3(133.00f, -24.0f, 363.0f),
	Vec3(3.50f, -22.0f, 453.0f),
	Vec3(13.30f, -23.0f, 365.0f),
	Vec3(153.00f, -72.0f, 735.0f),
	Vec3(231.00f, -92.0f, 733.0f),
};

#define numVectsB  ((sizeof(vSourceB))/(sizeof(vSourceB[0])) )

Vec3 vResultB[] =
{
	Vec3(-0.300000f, 8.500000f, 9.500000f),
	Vec3(145.799988f, -3.599999f, 179.000000f),
	Vec3(91.000000f, -7.199999f, 247.499985f),
	Vec3(2.450000f, -6.599998f, 309.500000f),
	Vec3(9.099999f, -6.900000f, 249.499985f),
	Vec3(104.699997f, -21.599998f, 489.999969f),
	Vec3(157.500000f, -27.599998f, 487.999969f),
};	   
#endif

TEST(Vec3App_LERP_Array1, TestConfig::ALL)
{
#if Vec3App_LERP_Array1

	Vec3 vOut[numVectsB];

	Vec3App::LerpArray(vOut, vSourceB, vTargetB, 1.0f, numVectsB);

	int i;

	i = 0;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));

	i = 1;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));

	i = 2;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));
	

	i = 3;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));
	

	i = 4;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));


	i = 5;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));


	i = 6;
	CHECK(eq(vOut[i][x], vTargetB[i][x], MATH_TOLERANCE));
	CHECK(eq(vOut[i][y], vTargetB[i][y], MATH_TOLERANCE));
	CHECK(eq(vOut[i][z], vTargetB[i][z], MATH_TOLERANCE));


#endif
} TEST_END



TEST(Vec3App_Lerp, TestConfig::ALL)
{
#if Vec3App_Lerp
	Vec3 va(0.0f, 0.0f, 0.0f);
	Vec3 vb(10.0f, 20.0f, 30.0f);
	Vec3 vout;

	Vec3App::Lerp(vout, va, vb, 0.0f);

	CHECK((vout[x] == 0.0f));
	CHECK((vout[y] == 0.0f));
	CHECK((vout[z] == 0.0f));
	

#endif
} TEST_END



TEST(Vec3App_Lerp2, TestConfig::ALL)
{
#if Vec3App_Lerp2

	Vec3 va(0.0f, 0.0f, 0.0f);
	Vec3 vb(10.0f, 20.0f, 30.0f);
	Vec3 vout;

	Vec3App::Lerp(vout, va, vb, 0.5f);

	CHECK((vout[x] == 5.0f));
	CHECK((vout[y] == 10.0f));
	CHECK((vout[z] == 15.0f));


#endif
} TEST_END



TEST(Vec3App_Lerp3, TestConfig::ALL)
{
#if Vec3App_Lerp3

	Vec3 va(0.0f, 0.0f, 0.0f);
	Vec3 vb(10.0f, 20.0f, 30.0f);
	Vec3 vout;

	Vec3App::Lerp(vout, va, vb, 0.25f);

	CHECK((vout[x] == 2.5f));
	CHECK((vout[y] == 5.0f));
	CHECK((vout[z] == 7.50f));

#endif
} TEST_END



TEST(Vec3App_Lerp4, TestConfig::ALL)
{
#if Vec3App_Lerp4

	Vec3 va(0.0f, 0.0f, 0.0f);
	Vec3 vb(10.0f, 20.0f, 30.0f);
	Vec3 vout;

	Vec3App::Lerp(vout, va, vb, 1.0f);

	CHECK((vout[x] == 10.0f));
	CHECK((vout[y] == 20.0f));
	CHECK((vout[z] == 30.0f));


#endif
} TEST_END


// ---  End of File ---------------
