//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "File.h"

namespace Azul
{
	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD flags;
		BOOL status = GetHandleInformation(fh, &flags);
		return status;
	}

	File::Error File::Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept
	{
		if (!fileName)
		{
			return File::Error::OPEN_FAIL;
		}

		if (IsHandleValid(fh))
		{
			return File::Error::OPEN_FAIL;
		}

		DWORD accessCode = 0;

		switch (mode)
		{
		case File::Mode::READ:
			accessCode = GENERIC_READ;
			break;
		case File::Mode::WRITE:
			accessCode = GENERIC_WRITE;
			break;
		case File::Mode::READ_WRITE:
			accessCode = GENERIC_READ | GENERIC_WRITE;
			break;
		}

		fh = CreateFile(fileName, accessCode, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (IsHandleValid(fh))
		{
			return File::Error::SUCCESS;
		}
		else
		{
			return File::Error::OPEN_FAIL;
		}
	}

	File::Error File::Close(File::Handle& fh) noexcept
	{
		if (IsHandleValid(fh))
		{
			BOOL status = CloseHandle(fh);

			if (status)
			{
				return File::Error::SUCCESS;
			}
		}

		return File::Error::CLOSE_FAIL;
	}

	File::Error File::Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept
	{
		BOOL status = WriteFile(fh, buffer, inSize, nullptr, nullptr);

		if (status)
		{
			return File::Error::SUCCESS;
		}
		else
		{
			return File::Error::WRITE_FAIL;
		}
	}

	File::Error File::Read(File::Handle fh, void* const buffer, const DWORD inSize) noexcept
	{
		BOOL status = ReadFile(fh, buffer, inSize, nullptr, nullptr);

		if (status)
		{
			return File::Error::SUCCESS;
		}
		else
		{
			return File::Error::READ_FAIL;
		}
	}

	File::Error File::Seek(File::Handle fh, File::Position location, int offset) noexcept
	{
		DWORD moveMethod = 0;

		switch (location)
		{
		case File::Position::BEGIN:
			moveMethod = FILE_BEGIN;
			break;
		case File::Position::END:
			moveMethod = FILE_END;
			break;
		case File::Position::CURRENT:
			moveMethod = FILE_CURRENT;
			break;
		}

		DWORD setOffset = SetFilePointer(fh, offset, nullptr, moveMethod);

		if (setOffset != INVALID_SET_FILE_POINTER)
		{
			return File::Error::SUCCESS;
		}
		else
		{
			return File::Error::SEEK_FAIL;
		}
	}

	File::Error File::Tell(File::Handle fh, DWORD& offset) noexcept
	{
		if (!IsHandleValid(fh))
		{
			return File::Error::TELL_FAIL;
		}

		offset = SetFilePointer(fh, 0, nullptr, FILE_CURRENT);

		return File::Error::SUCCESS;
	}

	File::Error File::Flush(File::Handle fh) noexcept
	{
		if (!IsHandleValid(fh))
		{
			return File::Error::FLUSH_FAIL;
		}

		FlushFileBuffers(fh);

		return File::Error::SUCCESS;
	}
}

// --- End of File ---