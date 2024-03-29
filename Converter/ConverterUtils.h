#ifndef _ConverterUtils
#define _ConverterUtils

#include <string>

namespace Azul
{
	class ConverterUtils
	{
	public:
		static bool StringReplace(std::string& str, const std::string& from, const std::string& to);
		static bool DoesFileExist(const std::string& filename);
		static std::string FileToString(const char* const pMeshFileName);
	};
}

#endif