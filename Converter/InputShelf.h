#ifndef _Converter_InputShelf
#define _Converter_InputShelf

#include <string>
#include <unordered_map>
#include "GLTF.h"

namespace Azul
{
	namespace Converter
	{
		// A shelf to place read only data of input files so that they can be
		// read from by many jobs without reloading the file each time. Stores
		// the entire content of the input file.
		class InputShelf
		{
		public:
			static void StoreRaw(const char* pFileName);
			static const std::string& LoadRaw(const char* pFileName);

			static void StoreGLTFModel(const char* pFileName);
			static const tinygltf::Model& LoadGLTFModel(const char* pFileName);

		private:
			InputShelf() = default;
			InputShelf(const InputShelf&) = default;
			InputShelf& operator=(const InputShelf&) = default;
			~InputShelf() = default;

			static InputShelf& GetInstance();

			std::unordered_map<std::string, std::string> shelf;
			std::unordered_map<std::string, tinygltf::Model> modelShelf;
		};
	}
}

#endif