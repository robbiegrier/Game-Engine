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

unsigned char *pBuffText = nullptr;

TEST_WITH_TEARDOWN(TEXTURE_BUFFER, TestConfig::ALL)
{
#if TEXTURE_BUFFER

	unsigned int numBytes;

	Verify::GetTextureBufferCopy(pBuffText, numBytes);

	unsigned int md5;
	MD5BufferReduced(pBuffText, numBytes, md5);

	//Trace::out("pBuffText: %p md5: %x  bytes: %x \n", pBuffText, md5, numBytes);

	CHECK(md5 == 0xec9329f9);
	CHECK(numBytes == 0x100000);

	delete[] pBuffText;
	pBuffText = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(TEXTURE_BUFFER)
{
	delete[] pBuffText;
}


// ---  End of File ---
