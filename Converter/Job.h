#ifndef _Converter_Job
#define _Converter_job

#include <string>

namespace Azul
{
	namespace Converter
	{
		class Job
		{
		public:
			Job() = default;
			Job(const Job&) = delete;
			Job& operator=(const Job&) = delete;
			virtual ~Job() = default;

			void SetJobTypeName(const std::string& inJobTypeName);
			void SetInputFile(const std::string& inFile);
			void SetOutputFile(const std::string& inFile);

			virtual std::string GetJobDisplayName();

			virtual void Run() = 0;

		protected:
			std::string jobTypeName;
			std::string inputFileName;
			std::string outputFileName;
		};
	}
}

#endif