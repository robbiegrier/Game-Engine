//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "File.h"

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

using namespace Azul;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleCloseFail,TestConfig::ALL)
{
#if SimpleCloseFail

	File::Error  ferror(File::Error::UNDEFINED);
	int a = 0x55;
	File::Handle fh = &a;

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::CLOSE_FAIL);

#endif
} TEST_END

TEST_TEARDOWN(SimpleCloseFail)
{
#if SimpleCloseFail


#endif
}

// --- End of File ---
