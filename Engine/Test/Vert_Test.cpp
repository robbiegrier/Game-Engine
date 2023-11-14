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

unsigned char *pBuffVBO = nullptr;

TEST_WITH_TEARDOWN(VBO_VERT, TestConfig::ALL)
{
#if VBO_VERT

	unsigned int numBytes;

	Verify::GetVertsCopy(pBuffVBO, numBytes);

	unsigned int md5 =0;
	MD5BufferReduced(pBuffVBO, numBytes, md5);

	//Trace::out("pBuffVBO: %p md5: %x  bytes: %x \n", pBuffVBO, md5, numBytes);

	CHECK(md5 == 0x56852faf);
	CHECK(numBytes == 0x1758);

	delete[] pBuffVBO;
	pBuffVBO = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(VBO_VERT)
{
	delete[] pBuffVBO;
}

// ---  End of File ---
