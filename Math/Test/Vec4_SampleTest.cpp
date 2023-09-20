//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
using namespace Azul;

//---------------------------------------------------------------------------
// Sample TESTS:
//---------------------------------------------------------------------------

TEST(Vec4_SampleTest, TestConfig::ALL)
{
#if Vec4_SampleTest

	const float x(5.0f);

	CHECK(x == 5.0f);

#endif
} TEST_END

// ---  End of File ---------------
