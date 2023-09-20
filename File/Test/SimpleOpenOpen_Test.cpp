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
static File::Handle fh_file3;
static File::Handle fh_file3b;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleOpenOpen, TestConfig::ALL)
{
#if SimpleOpenOpen

	File::Handle fh(nullptr);
	File::Error  ferror;

	ferror = File::Open(fh, "file3.bin", File::Mode::WRITE);
	fh_file3 = fh;  // used for teardown

	// make sure it opened, also check the directory
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	// should fail
	ferror = File::Open(fh, "file3.bin", File::Mode::WRITE);
	fh_file3b = fh;  // used for teardown
	CHECK(ferror == File::Error::OPEN_FAIL);

#endif
} TEST_END

TEST_TEARDOWN(SimpleOpenOpen)
{
#if SimpleOpenOpen

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file3, &flags);
	if (status)
	{
		File::Close(fh_file3);
	}
	status = GetHandleInformation(fh_file3b, &flags);
	if (status)
	{
		File::Close(fh_file3b);
	}

#endif
}

// --- End of File ---
