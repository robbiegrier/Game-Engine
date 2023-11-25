#ifndef _ClipManager
#define _ClipManager

#include "ManagerBase.h"
#include "Clip.h"

namespace Azul
{
	class ClipManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static Clip* Add(Clip::Name name, Clip* pShader);
		static Clip* Find(Clip::Name name);
		static void Dump();

	protected:
		// Big four
		ClipManager(int initialReserved, int reserveGrow);
		ClipManager(const ClipManager&) = delete;
		ClipManager& operator =(const ClipManager&) = delete;
		~ClipManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static ClipManager& GetInstance();
		static ClipManager* pInstance;
	};
}

#endif