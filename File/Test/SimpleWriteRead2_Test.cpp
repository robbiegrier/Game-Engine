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
static File::Handle fh_file23;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteRead2, TestConfig::ALL)
{
#if SimpleWriteRead2

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file17.bin", File::Mode::READ_WRITE);
	fh_file23 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- READ ------------------------------------------
	ferror = File::Seek(fh, File::Position::BEGIN, 0);
	CHECK(ferror == File::Error::SUCCESS);

	DWORD offset;

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 0);

	unsigned char dataFromFile[8];

	ferror = File::Read(fh, &dataFromFile[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(memcmp(&data[0], &dataFromFile[0], 8) == 0);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteRead2)
{
#if SimpleWriteRead2

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file23, &flags);
	if (status)
	{
		File::Close(fh_file23);
	}

#endif
}

// --- End of File ---
