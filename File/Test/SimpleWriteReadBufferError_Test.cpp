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
static File::Handle fh_file11;
static File::Handle fh_file12;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadBufferError, TestConfig::ALL)
{
#if SimpleWriteReadBufferError

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file10.bin", File::Mode::WRITE);
	fh_file11 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- READ ------------------------------------------

	File::Handle fh2(nullptr);

	ferror = File::Open(fh2, "file10.bin", File::Mode::READ);
	fh_file12 = fh2;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh2 != nullptr);

	unsigned char dataFromFile[8] = { 0 };
	ferror = File::Read(fh2, nullptr, 8);
	CHECK(ferror == File::Error::READ_FAIL);

	ferror = File::Read(fh2, &dataFromFile[0], 0xFFFFFFFF);
	CHECK(ferror == File::Error::READ_FAIL);

	ferror = File::Close(fh2);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadBufferError)
{
#if SimpleWriteReadBufferError
	DWORD flags;
	BOOL status = GetHandleInformation(fh_file11, &flags);
	if (status)
	{
		File::Close(fh_file11);
	}

	status = GetHandleInformation(fh_file12, &flags);
	if (status)
	{
		File::Close(fh_file12);
	}

#endif
}

// --- End of File ----
