#include "ConverterUtils.h"
#include "File.h"

namespace Azul
{
	bool ConverterUtils::StringReplace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	bool ConverterUtils::DoesFileExist(const std::string& filename)
	{
		WIN32_FIND_DATAA fd = { 0 };
		HANDLE hFound = FindFirstFileA(filename.c_str(), &fd);
		bool retval = hFound != INVALID_HANDLE_VALUE;
		FindClose(hFound);
		return retval;
	}

	std::string ConverterUtils::FileToString(const char* const pMeshFileName)
	{
		File::Handle fh;
		File::Error ferror;
		DWORD length;

		ferror = File::Open(fh, pMeshFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Tell(fh, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		char* poBuff = new char[length];
		assert(poBuff);

		ferror = File::Read(fh, poBuff, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		std::string output = std::string(poBuff, length);
		delete[] poBuff;

		return output;
	}
}