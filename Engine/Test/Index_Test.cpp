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

unsigned char *pBuffIndex;

TEST_WITH_TEARDOWN(VBO_INDEX, TestConfig::ALL)
{
#if VBO_INDEX

	unsigned int numBytes;

	Verify::GetIndexCopy(pBuffIndex, numBytes);

	unsigned int md5;
	MD5BufferReduced(pBuffIndex, numBytes, md5);

	//Trace::out("pbuff: %p md5: %x  bytes: %x \n", pBuff, md5, numBytes);

	CHECK(md5 == 0x78c7f3d9);
	CHECK(numBytes == 0xdf8);

	delete[] pBuffIndex;
	pBuffIndex = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(VBO_INDEX)
{
	delete[] pBuffIndex;
}

// ---  End of File ---
