//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "File.h"

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

using namespace Azul;

// used for teardown
static  File::Handle fh_file2;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
// To Run all tests: Debug-> Start without debugging
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleOpenClose, TestConfig::ALL)
{
#if SimpleOpenClose

	File::Handle fh(nullptr);
	File::Error  ferror;

	ferror = File::Open(fh, "file2.bin", File::Mode::WRITE);
	fh_file2 = fh;
	// make sure it opened, also check the directory
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleOpenClose)
{
#if SimpleOpenClose

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file2, &flags);
	if (status)
	{
		File::Close(fh_file2);
	}

#endif
}

// --- End of File ---
