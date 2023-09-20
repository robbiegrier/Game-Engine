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
static File::Handle fh_file9;
static File::Handle fh_file10;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteRead, TestConfig::ALL)
{
#if SimpleWriteRead

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file9.bin", File::Mode::WRITE);
	fh_file9 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- READ ------------------------------------------

	File::Handle fh2(nullptr);

	ferror = File::Open(fh2, "file9.bin", File::Mode::READ);
	fh_file10 = fh2;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh2 != nullptr);

	unsigned char dataFromFile[8];
	ferror = File::Read(fh2, &dataFromFile[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(memcmp(&data[0], &dataFromFile[0], 8) == 0);

	ferror = File::Close(fh2);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END


TEST_TEARDOWN(SimpleWriteRead)
{
#if SimpleWriteRead

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file9, &flags);
	if (status)
	{
		File::Close(fh_file9);
	}

	status = GetHandleInformation(fh_file10, &flags);
	if (status)
	{
		File::Close(fh_file10);
	}

#endif
}

// --- End of File ---
