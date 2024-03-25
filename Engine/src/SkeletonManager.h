#ifndef _SkeletonManager
#define _SkeletonManager

#include "ManagerBase.h"
#include "Skeleton.h"

namespace Azul
{
	class SkeletonManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static Skeleton* Add(Skeleton::Name name, Skeleton* pItem);
		static Skeleton* Find(Skeleton::Name name);
		static void Dump();

	protected:
		// Big four
		SkeletonManager(int initialReserved, int reserveGrow);
		SkeletonManager(const SkeletonManager&) = delete;
		SkeletonManager& operator =(const SkeletonManager&) = delete;
		~SkeletonManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static SkeletonManager& GetInstance();
		static SkeletonManager* pInstance;
	};
}

#endif