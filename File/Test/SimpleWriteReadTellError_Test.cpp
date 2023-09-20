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
static File::Handle fh_file22;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadTellError, TestConfig::ALL)
{
#if SimpleWriteReadTellError

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file22.bin", File::Mode::WRITE);
	fh_file22 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- OPEN to READ -----------------------------------

	DWORD offset;

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::TELL_FAIL);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadTellError)
{
#if SimpleWriteReadTellError

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file22, &flags);
	if (status)
	{
		File::Close(fh_file22);
	}

#endif
}

// --- End of File ---
