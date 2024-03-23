#ifndef _Clipboard
#define _Clipboard

#include "json.hpp"
#include "Selection.h"
#include "MathEngine.h"

namespace Azul
{
	class GameObject;

	// The clipboard allows data to be copied and pasted
	class Clipboard
	{
	public:
		using json = nlohmann::json;

		// Big four
		Clipboard() = default;
		Clipboard(const Clipboard&) = delete;
		Clipboard& operator=(const Clipboard&) = delete;
		~Clipboard() = default;

		// JSON & GameObject utilities
		static json SceneToJson(GameObject* pRootObject);
		static json ObjectToJson(GameObject* pObject);
		static GameObject* JsonToObject(const json& item);
		static GameObject* GetParentFromJson(const json& item);
		static void SpawnObjectsFromJson(const json& scene, const Vec3& location);

		// Copy editor selection to the clipboard
		void Copy(const Selection& selection);

		// Paste clipboard objects at a location
		void Paste(const Vec3& location);

		void MakeContentUnique();

		std::string GenerateUniqueObjectName(const char* pInName);

	private:
		json content;
	};
}

#endif