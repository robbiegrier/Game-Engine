#include "InputShelf.h"
#include "ConverterUtils.h"

namespace Azul
{
	namespace Converter
	{
		void InputShelf::StoreRaw(const char* pFileName)
		{
			InputShelf& self = GetInstance();

			const auto& find = self.shelf.find(pFileName);

			if (find == self.shelf.end())
			{
				std::string content = ConverterUtils::FileToString(pFileName);
				self.shelf.emplace(pFileName, content);

				Trace::out("Input [%s] added to InputShelf\n", pFileName);
			}
			else
			{
				Trace::out("Input [%s] already stored and will be reused\n", pFileName);
			}
		}

		const std::string& InputShelf::LoadRaw(const char* pFileName)
		{
			InputShelf& self = GetInstance();
			const auto& find = self.shelf.find(pFileName);
			assert(find != self.shelf.end());
			return find->second;
		}

		void InputShelf::StoreGLTFModel(const char* pFileName)
		{
			InputShelf& self = GetInstance();

			const auto& find = self.shelf.find(pFileName);

			if (find == self.shelf.end())
			{
				tinygltf::Model gltfModel;
				bool status = GLTF::LoadBinary(gltfModel, pFileName);
				assert(status);
				self.modelShelf.emplace(pFileName, gltfModel);

				Trace::out("Input [%s] added to InputShelf (glb model)\n", pFileName);
			}
			else
			{
				Trace::out("Input [%s] already stored and will be reused (glb model)\n", pFileName);
			}
		}

		const tinygltf::Model& InputShelf::LoadGLTFModel(const char* pFileName)
		{
			InputShelf& self = GetInstance();
			const auto& find = self.modelShelf.find(pFileName);
			assert(find != self.modelShelf.end());
			return find->second;
		}

		InputShelf& InputShelf::GetInstance()
		{
			static InputShelf instance;
			return instance;
		}
	}
}