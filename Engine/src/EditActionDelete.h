#ifndef _EditActionDelete
#define _EditActionDelete

#include "EditAction.h"
#include "MathEngine.h"
#include "GameObject.h"

namespace Azul
{
	// Action to delete an object. Undo respawns the object.
	class EditActionDelete : public EditAction
	{
	public:
		// Big four
		EditActionDelete() = default;
		EditActionDelete(const EditActionDelete&) = delete;
		EditActionDelete& operator=(const EditActionDelete&) = delete;
		virtual ~EditActionDelete();

		// Action Contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

	private:
		// If the object remains deleted when the editor is closed, this action owns the object
		bool ownSpawnedObject = false;
	};
}

#endif