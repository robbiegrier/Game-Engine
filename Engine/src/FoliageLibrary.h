#ifndef _FoliageLibrary
#define _FoliageLibrary

#include "ManagerBase.h"
#include "FoliageType.h"

namespace Azul
{
	// Manages the set of foliage types available. Read only library of assets.
	class FoliageLibrary : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static FoliageType* Add(FoliageType::Name name, FoliageType* pShader);
		static FoliageType* Find(FoliageType::Name name);
		static void Dump();

		static Iterator* GetManagerActiveIterator() { return GetInstance().GetActiveIterator(); }

	protected:
		// Big four
		FoliageLibrary(int initialReserved, int reserveGrow);
		FoliageLibrary(const FoliageLibrary&) = delete;
		FoliageLibrary& operator =(const FoliageLibrary&) = delete;
		~FoliageLibrary();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static FoliageLibrary& GetInstance();
		static FoliageLibrary* pInstance;
	};
}

#endif