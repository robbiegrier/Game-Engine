#ifndef _CameraManager
#define _CameraManager

#include "ManagerBase.h"
#include "Camera.h"

namespace Azul
{
	class CameraManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static void Dump();
		static Camera* Add(Camera::Name name, Camera* pItem);
		static Camera* Find(Camera::Name name);

		// Set and get the current camera (player's view)
		static void SetCurrentCamera(Camera::Name name);
		static Camera* GetCurrentCamera();

		// Update called by the core loop
		static void Update(float deltaTime);

	protected:
		// Big four
		CameraManager(int initialReserved, int reserveGrow);
		CameraManager(const CameraManager&) = delete;
		CameraManager& operator =(const CameraManager&) = delete;
		~CameraManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static CameraManager& GetInstance();
		static CameraManager* pInstance;

		// Track the current camera
		Camera* pCurrentCamera = nullptr;
	};
}

#endif