#ifndef _Component
#define _Component

#include "DLink.h"
#include "GameObject.h"
#include <json.hpp>

using json = nlohmann::json;

namespace Azul
{
	class GameObject;

	class Component : public DLink
	{
	public:
		Component() = default;
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		virtual ~Component() = default;

		virtual void Update(float deltaTime) { static_cast<void>(deltaTime); }

		virtual void UpdateInspectorGui() {}

		void Wash() override;
		bool Compare(DLink* pTargetNode) override;

		void SetGameObject(GameObject* pInObject) { pObject = pInObject; }
		GameObject* GetGameObject() const { return pObject; }

		virtual void Serialize(json& out) = 0;
		virtual void Deserialize(const json& in) = 0;

	protected:
		GameObject* pObject;
	};
}

#endif