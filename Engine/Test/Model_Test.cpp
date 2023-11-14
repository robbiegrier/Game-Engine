//---------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

//---------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#include "md5.h"
#include "Verify.h"

using namespace Azul;

//---------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------

TEST(MODEL_STATS, TestConfig::ALL)
{
#if MODEL_STATS

	Verify::Model_stats stats = Verify::GetModelStats();

	CHECK(stats.numVerts == 498);
	CHECK(stats.numTris == 298);
	CHECK(stats.TextureWidth == 512);
	CHECK(stats.TextureHeight == 512);

#endif
} TEST_END

// ---  End of File ---
