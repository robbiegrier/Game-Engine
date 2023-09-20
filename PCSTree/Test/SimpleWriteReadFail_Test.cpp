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
static File::Handle fh_file13;
static File::Handle fh_file14;
static File::Handle fh_file15;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadFail, TestConfig::ALL)
{
#if SimpleWriteReadFail

	File::Handle A(nullptr);
	File::Handle fh2(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(A, "file11.bin", File::Mode::WRITE);
	fh_file13 = A;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(A != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(A, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	// Microsoft sucks, these next two lines need to be there, but it cant due to aliasing
	//ferror = File::Close(A);
	//CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Open(fh2, "file12.bin", File::Mode::WRITE);
	fh_file14 = fh2;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh2 != nullptr);

	ferror = File::Close(A);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Write(fh2, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh2);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- READ ------------------------------------------

	ferror = File::Open(fh2, "file12.bin", File::Mode::READ);
	fh_file15 = fh2;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh2 != nullptr);

	unsigned char dataFromFile[8];
	ferror = File::Read(A, &dataFromFile[0], 8);
	CHECK(ferror == File::Error::READ_FAIL);

	ferror = File::Close(fh2);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadFail)
{
#if SimpleWriteReadFail

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file13, &flags);
	if (status)
	{
		File::Close(fh_file13);
	}

	status = GetHandleInformation(fh_file14, &flags);
	if (status)
	{
		File::Close(fh_file14);
	}

	status = GetHandleInformation(fh_file15, &flags);
	if (status)
	{
		File::Close(fh_file15);
	}

#endif
}

// --- End of File ---
