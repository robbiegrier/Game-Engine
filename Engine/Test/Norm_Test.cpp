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

unsigned char *pBuffNORM = nullptr;

TEST_WITH_TEARDOWN(VBO_NORM, TestConfig::ALL)
{
#if VBO_NORM

	unsigned int numBytes;

	Verify::GetNormsCopy(pBuffNORM, numBytes);

	unsigned int md5;
	MD5BufferReduced(pBuffNORM, numBytes, md5);

	//Trace::out("pBuffNORM: %p md5: %x  bytes: %x \n", pBuffNORM, md5, numBytes);

	CHECK(md5 == 0x1f9aac90);
	CHECK(numBytes == 0x1758);

	delete[] pBuffNORM;
	pBuffNORM = nullptr;

#endif
} TEST_END

TEST_TEARDOWN(VBO_NORM)
{
	delete[] pBuffNORM;
}


// ---  End of File ---
