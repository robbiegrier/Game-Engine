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
static File::Handle fh_file16;
static File::Handle fh_file17;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadSEEK, TestConfig::ALL)
{
#if SimpleWriteReadSEEK

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file13.bin", File::Mode::WRITE);
	fh_file16 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- Open to Read  --------------------------------

	ferror = File::Open(fh, "file13.bin", File::Mode::READ);
	fh_file17 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	//----------- Seek ------------------------------------------

	ferror = File::Seek(fh, File::Position::BEGIN, 0);
	CHECK(ferror == File::Error::SUCCESS);

	unsigned char returnData;
	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0xaa);

	ferror = File::Seek(fh, File::Position::END, -1);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0x22);

	ferror = File::Seek(fh, File::Position::CURRENT, -4);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0xee);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadSEEK)
{
#if SimpleWriteReadSEEK

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file16, &flags);
	if (status)
	{
		File::Close(fh_file16);
	}

	status = GetHandleInformation(fh_file17, &flags);
	if (status)
	{
		File::Close(fh_file17);
	}

#endif
}

// --- End of File ---
