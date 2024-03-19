#ifndef _EditActionSpawn
#define _EditActionSpawn

#include "EditAction.h"
#include "MathEngine.h"
#include "GameObject.h"

namespace Azul
{
	// Action to spawn an object. Undo deletes the object.
	class EditActionSpawn : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionSpawn() = default;
		EditActionSpawn(const EditActionSpawn&) = delete;
		EditActionSpawn& operator=(const EditActionSpawn&) = delete;
		virtual ~EditActionSpawn();

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		// Set the location to spawn at
		void SetLocation(const Vec3& inLocation);

		// Set the parent object to spawn as child of. Nullptr will spawn under the root.
		void SetTargetParent(GameObject* pParent);

	private:
		Vec3 location;
		GameObject* pTargetParent = nullptr;

		// If the object remains deleted (Undo() called) when the editor is closed, this action owns the object
		bool ownSpawnedObject = false;
	};
}

#endif