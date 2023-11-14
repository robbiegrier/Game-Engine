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
unsigned char *pBuffUV = nullptr;

TEST_WITH_TEARDOWN(VBO_UV, TestConfig::ALL)
{
#if VBO_UV

	unsigned int numBytes;

	Verify::GetUVsCopy(pBuffUV, numBytes);

	unsigned int md5;
	MD5BufferReduced(pBuffUV, numBytes, md5);

	//Trace::out("pBuffUV: %p md5: %x  bytes: %x \n", pBuffUV, md5, numBytes);

	CHECK(md5 == 0x2e538634);
	CHECK(numBytes == 0xf90);

	delete[] pBuffUV;
	pBuffUV = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(VBO_UV)
{
	delete[] pBuffUV;
}

// ---  End of File ---
