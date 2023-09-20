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

TEST(Vec3_Proxy, TestConfig::ALL)
{
#if Vec3_Proxy


	Vec3 A(1, 2, 3);
	Vec3 B(3, 4, 5);
	Vec3 C(3, 4, 5);

	float val;

	val = A.len();
	CHECK(UnitUtility::AreEqual(val, 3.7416f));

	val = B.len();
	CHECK(UnitUtility::AreEqual(val, 7.0710f));

	val = C.len();
	CHECK(UnitUtility::AreEqual(val, 7.0710f));

	if (B.len() > A.len())
	{
		CHECK(true);
	}
	else
	{
		CHECK(false);
	}

	if (B.len() < A.len())
	{
		CHECK(false);
	}
	else
	{
		CHECK(true);
	}

	if (B.len() == A.len())
	{
		CHECK(false);
	}
	else
	{
		CHECK(true);
	}

	if (B.len() == C.len())
	{
		CHECK(true);
	}
	else
	{
		CHECK(false);
	}


	if (B.len() >= A.len())
	{
		CHECK(true);
	}
	else
	{
		CHECK(false);
	}

	if (B.len() >= C.len())
	{
		CHECK(true);
	}
	else
	{
		CHECK(false);
	}

	if (B.len() <= A.len())
	{
		CHECK(false);
	}
	else
	{
		CHECK(true);
	}

	if (B.len() <= C.len())
	{
		CHECK(true);
	}
	else
	{
		CHECK(false);
	}

	if (B.len() != C.len())
	{
		CHECK(false);
	}
	else
	{
		CHECK(true);
	}


	// These len need to converted to float before multiplication
	val = A.len() * B.len();
	CHECK(UnitUtility::AreEqual(val, 26.4575f))


#endif
} TEST_END



// ---  End of File ---------------
