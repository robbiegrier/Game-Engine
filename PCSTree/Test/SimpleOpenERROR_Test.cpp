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
static File::Handle fh_file4;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(SimpleOpenERROR, TestConfig::ALL)
{
#if SimpleOpenERROR

	File::Handle fh;
	File::Error  ferror;

	ferror = File::Open(fh, nullptr, File::Mode::WRITE);
	fh_file4 = fh;
	// make sure it opened, also check the directory
	CHECK(ferror == File::Error::OPEN_FAIL);

#endif
} TEST_END

TEST_TEARDOWN(SimpleOpenERROR)
{
#if SimpleOpenERROR

	DWORD flags;
	const BOOL status = GetHandleInformation(fh_file4, &flags);
	if (status)
	{
		File::Close(fh_file4);
	}

#endif
}

// --- End of File ---
