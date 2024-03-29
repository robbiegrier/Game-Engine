#ifndef _ProtoAzul
#define _ProtoAzul

#include "File.h"

namespace Azul
{
	class ProtoAzul
	{
	public:
		template<class T>
		static void WriteProtoFile(const T& source, const char* const filename)
		{
			File::Handle fh;
			std::string strOut;
			source.SerializeToString(&strOut);

			File::Error err = File::Open(fh, filename, File::Mode::WRITE);	assert(err == File::Error::SUCCESS);
			File::Write(fh, strOut.data(), strOut.length());				assert(err == File::Error::SUCCESS);
			err = File::Close(fh);											assert(err == File::Error::SUCCESS);
		}

		template<class T>
		static void ReadProtoFile(T& output, const char* const filename)
		{
			File::Handle fh;
			DWORD FileLength;

			File::Error err = File::Open(fh, filename, File::Mode::READ);	assert(err == File::Error::SUCCESS);
			err = File::Seek(fh, File::Position::END, 0);					assert(err == File::Error::SUCCESS);
			err = File::Tell(fh, FileLength);								assert(err == File::Error::SUCCESS);

			char* poNewBuff = new char[FileLength]();

			err = File::Seek(fh, File::Position::BEGIN, 0);					assert(err == File::Error::SUCCESS);
			err = File::Read(fh, poNewBuff, FileLength);					assert(err == File::Error::SUCCESS);
			err = File::Close(fh);											assert(err == File::Error::SUCCESS);

			std::string strIn(poNewBuff, FileLength);
			delete[] poNewBuff;

			output.ParseFromString(strIn);
		}
	};
}

#endif