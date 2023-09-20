//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
using namespace Azul;

//---------------------------------------------------------------------------
// Sample TESTS:
//---------------------------------------------------------------------------

TEST(Vec4_ConstantCheck, TestConfig::ALL)
{
#if Vec4_ConstantCheck

	CHECK(Util::isEqual(MATH_PI2, 1.570796f, MATH_TOLERANCE) == true);

#endif
} TEST_END

// ---  End of File ---------------
