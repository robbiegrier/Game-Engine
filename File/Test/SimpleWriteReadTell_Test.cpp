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
static File::Handle fh_file20;
static File::Handle fh_file21;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleWriteReadTell, TestConfig::ALL)
{
#if SimpleWriteReadTell

	File::Handle fh(nullptr);
	File::Error  ferror;

	//----------- WRITE ------------------------------------------

	ferror = File::Open(fh, "file15.bin", File::Mode::WRITE);
	fh_file20 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	const unsigned char data[8] = { 0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22 };

	ferror = File::Write(fh, &data[0], 8);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

	//----------- OPEN to READ -----------------------------------

	DWORD offset(0);

	ferror = File::Open(fh, "file15.bin", File::Mode::READ);
	fh_file21 = fh;
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(fh != nullptr);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 0);

	ferror = File::Seek(fh, File::Position::END, 0);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 8);

	//----------- Seek ------------------------------------------

	ferror = File::Seek(fh, File::Position::BEGIN, 0);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 0);


	unsigned char returnData;
	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0xaa);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 1);

	ferror = File::Seek(fh, File::Position::END, -1);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 7);

	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0x22);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 8);

	ferror = File::Seek(fh, File::Position::CURRENT, -4);
	CHECK(ferror == File::Error::SUCCESS);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 4);

	ferror = File::Read(fh, &returnData, 1);
	CHECK(ferror == File::Error::SUCCESS);

	CHECK(returnData == 0xee);

	ferror = File::Tell(fh, offset);
	CHECK(ferror == File::Error::SUCCESS);
	CHECK(offset == 5);

	ferror = File::Close(fh);
	CHECK(ferror == File::Error::SUCCESS);

#endif
} TEST_END

TEST_TEARDOWN(SimpleWriteReadTell)
{
#if SimpleWriteReadTell

	DWORD flags;
	BOOL status = GetHandleInformation(fh_file20, &flags);
	if (status)
	{
		File::Close(fh_file20);
	}

	status = GetHandleInformation(fh_file21, &flags);
	if (status)
	{
		File::Close(fh_file21);
	}

#endif
}

// --- End of File ---
