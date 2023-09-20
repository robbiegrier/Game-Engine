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
static File::Handle fh_file1b;
static File::Handle fh_file1c;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleHandleValid, TestConfig::ALL)
{
#if SimpleHandleValid

	File::Handle fh(nullptr);
	File::Error  ferror;

	ferror = File::Open(fh, "file1.bin", File::Mode::WRITE);
	// used to clean up at end of test
	fh_file1b = fh;

	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	bool status = File::IsHandleValid(fh);
	CHECK(status == true);

	ferror = File::Close(fh);
	// used to clean up at end of test
	fh_file1c = fh;

	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr );

	status = File::IsHandleValid(fh);
	CHECK(status == false);

#endif
} TEST_END

TEST_TEARDOWN(SimpleHandleValid)
{
#if SimpleHandleValid

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file1b, &flags);
	if (status)
	{
		File::Close(fh_file1b);
	}

	status = GetHandleInformation(fh_file1c, &flags);
	if (status)
	{
		File::Close(fh_file1c);
	}

#endif
}

// --- End of File ---
