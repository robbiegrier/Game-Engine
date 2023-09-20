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
static File::Handle fh_file18;
static File::Handle fh_file19;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadSeekError, TestConfig::ALL)
{
#if SimpleWriteReadSeekError

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file14.bin", File::Mode::WRITE);
	fh_file18 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- Open to Read  --------------------------------

	ferror = File::Open(fh, "file14.bin", File::Mode::READ);
	fh_file19 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	//----------- Seek ------------------------------------------

	ferror = File::Seek(fh, File::Position::BEGIN, -1);
	CHECK(ferror == File::Error::SEEK_FAIL);

	ferror = File::Seek(fh, File::Position::CURRENT, -55);
	CHECK(ferror == File::Error::SEEK_FAIL);

	ferror = File::Seek(fh, File::Position::END, -77);
	CHECK(ferror == File::Error::SEEK_FAIL);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadSeekError)
{
#if SimpleWriteReadSeekError

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file18, &flags);
	if (status)
	{
		File::Close(fh_file18);
	}

	status = GetHandleInformation(fh_file19, &flags);
	if (status)
	{
		File::Close(fh_file19);
	}

#endif
}

// --- End of File ---
