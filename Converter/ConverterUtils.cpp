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

	std::string ConverterUtils::FileToString(const char* const pFileName)
	{
		File::Handle fh;
		File::Error err;
		DWORD length;

		err = File::Open(fh, pFileName, File::Mode::READ);		assert(err == File::Error::SUCCESS);
		err = File::Seek(fh, File::Position::END, 0);			assert(err == File::Error::SUCCESS);
		err = File::Tell(fh, length);							assert(err == File::Error::SUCCESS);
		err = File::Seek(fh, File::Position::BEGIN, 0);			assert(err == File::Error::SUCCESS);

		char* poBuff = new char[length];

		err = File::Read(fh, poBuff, length);					assert(err == File::Error::SUCCESS);
		err = File::Close(fh);									assert(err == File::Error::SUCCESS);

		std::string output = std::string(poBuff, length);
		delete[] poBuff;

		return output;
	}

	std::vector<std::string> ConverterUtils::GetAllSourceFileNames(const std::string& pattern)
	{
		std::vector<std::string> names;
		WIN32_FIND_DATA fd;

		HANDLE hFind = FindFirstFile(pattern.c_str(), &fd);

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					names.push_back(fd.cFileName);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}

		return names;
	}
}