#ifndef _FontLibrary
#define _FontLibrary

#include "ManagerBase.h"
#include "Font.h"

namespace Azul
{
	class FontLibrary : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static Font* Add(Font::Name name, Font* pShader);
		static Font* Find(Font::Name name);
		static void Dump();

	protected:
		// Big four
		FontLibrary(int initialReserved, int reserveGrow);
		FontLibrary(const FontLibrary&) = delete;
		FontLibrary& operator =(const FontLibrary&) = delete;
		~FontLibrary();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static FontLibrary& GetInstance();
		static FontLibrary* pInstance;
	};
}

#endif