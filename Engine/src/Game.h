#ifndef _Game
#define _Game

#include "Engine.h"
#include "MathEngine.h"

namespace Azul
{
	// The Game implements the core setup, runtime, and teardown methods
	// which are invoked in the correct order by the Engine base class.
	class Game : public Engine
	{
	public:
		// Singleton start and stop
		static int WINAPI Launch(HINSTANCE pInstance, int cmdShow);
		static void Teardown();

	protected:
		// Big four
		Game();
		Game(const Game&) = delete;
		Game& operator = (const Game&) = delete;
		virtual ~Game();

		// Game/Engine contract
		virtual bool LoadContent() override;
		virtual void UnloadContent() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual	void ClearDepthStencilBuffer(const Vec4& color) override;

	private:
		void EndFrame();

		void UpdateDemo(float deltaTime);

		void LoadShaders();
		void LoadCameras();
		void LoadAssets();

		// Singleton
		static Game& GetInstance();
		static Game* pInstance;

		AnimTimer intervalTimer;
		AnimTimer globalTimer;
	};
}

#endif
