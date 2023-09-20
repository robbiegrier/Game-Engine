#ifndef _ShaderObjectManager
#define _ShaderObjectManager

#include "ManagerBase.h"
#include "ShaderObject.h"

namespace Azul
{
	class ShaderObjectManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static ShaderObject* Add(ShaderObject::Name name, ShaderObject* pShader);
		static ShaderObject* Find(ShaderObject::Name name);
		static void Dump();

	protected:
		// Big four
		ShaderObjectManager(int initialReserved, int reserveGrow);
		ShaderObjectManager(const ShaderObjectManager&) = delete;
		ShaderObjectManager& operator =(const ShaderObjectManager&) = delete;
		~ShaderObjectManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static ShaderObjectManager& GetInstance();
		static ShaderObjectManager* pInstance;
	};
}

#endif
