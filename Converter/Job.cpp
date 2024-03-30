#include "Job.h"

namespace Azul
{
	namespace Converter
	{
		void Job::SetJobTypeName(const std::string& inJobTypeName)
		{
			jobTypeName = inJobTypeName;
		}

		void Job::SetInputFile(const std::string& inFile)
		{
			inputFileName = inFile;
		}

		void Job::SetOutputFile(const std::string& inFile)
		{
			outputFileName = inFile;
		}

		std::string Job::GetJobDisplayName()
		{
			return inputFileName + " (" + jobTypeName + ")" + " => " + outputFileName;
		}
	}
}