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

TEST(Vec4_Proxy, TestConfig::ALL)
{
#if Vec4_Proxy

	Vec4 A(1, 2, 3, 4);
	Vec4 B(3, 4, 5, 2);
	Vec4 C(3, 4, 2, 5);

	float val;

	val = A.len();
	CHECK(UnitUtility::AreEqual(val, 5.4772f));

	val = B.len();
	CHECK(UnitUtility::AreEqual(val, 7.3484f));

	val = C.len();
	CHECK(UnitUtility::AreEqual(val, 7.3484f));

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
	CHECK(UnitUtility::AreEqual(val, 40.2486f))


#endif
} TEST_END



// ---  End of File ---------------
