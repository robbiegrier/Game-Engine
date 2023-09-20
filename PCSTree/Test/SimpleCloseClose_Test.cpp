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
static File::Handle fh_file25;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleCloseClose,TestConfig::ALL)
{
#if SimpleCloseClose

	File::Handle fh(nullptr);
	File::Error  ferror;

	ferror = File::Open(fh, "file4.bin", File::Mode::WRITE);
	fh_file25 = fh;
	// make sure it opened, also check the directory
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::CLOSE_FAIL);

#endif
} TEST_END

TEST_TEARDOWN(SimpleCloseClose)
{
#if SimpleCloseClose

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file25, &flags);
	if (status)
	{
		File::Close(fh_file25);
	}

#endif
}

// --- End of File ---
