#ifndef _Component
#define _Component

#include "DLink.h"
#include "GameObject.h"

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

		void Wash() override;
		bool Compare(DLink* pTargetNode) override;

		void SetGameObject(GameObject* pInObject) { pObject = pInObject; }
		GameObject* GetGameObject() const { return pObject; }

	private:
		GameObject* pObject;
	};
}

#endif