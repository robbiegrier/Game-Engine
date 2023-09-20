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
static File::Handle fh_file5;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWrite, TestConfig::ALL)
{
#if SimpleWrite

	File::Handle fh(nullptr);
	File::Error  ferror;

	ferror = File::Open(fh, "file5.bin", File::Mode::WRITE);
	fh_file5 = fh;

	// make sure it opened, also check the directory
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWrite)
{
#if SimpleWrite

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file5, &flags);
	if (status)
	{
		File::Close(fh_file5);
	}

#endif
}

// --- End of File ---
