#ifndef _TextureObjectManager
#define _TextureObjectManager

#include "ManagerBase.h"
#include "TextureObject.h"

namespace Azul
{
	class TextureObjectManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static TextureObject* Add(TextureObject::Name name, TextureObject* pItem);
		static TextureObject* Find(TextureObject::Name name);
		static void Dump();

	protected:
		// Big four
		TextureObjectManager(int initialReserved, int reserveGrow);
		TextureObjectManager(const TextureObjectManager&) = delete;
		TextureObjectManager& operator =(const TextureObjectManager&) = delete;
		~TextureObjectManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static TextureObjectManager& GetInstance();
		static TextureObjectManager* pInstance;
	};
}

#endif