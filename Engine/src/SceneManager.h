#ifndef _SceneManager
#define _SceneManager

namespace Azul
{
	// Manages the scene files that can be saved or loaded
	class SceneManager
	{
	public:
		// Load a new scene. Destroys the previous scene.
		static void ChangeScene(const char* pSceneName);

		// Save the scene to a file.
		static void SaveScene(const char* pSceneName);

	private:
		// Big four
		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		~SceneManager() = default;

		// Meyers sigleton
		static SceneManager& GetInstance();

		static std::string GetFilePathName(const char* pSceneName);
	};
}

#endif