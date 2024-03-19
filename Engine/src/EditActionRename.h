#ifndef _EditActionRename
#define _EditActionRename

#include "EditAction.h"
#include "GameObject.h"

namespace Azul
{
	// Action to rotate an object with a quaternion
	class EditActionRename : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionRename() = default;
		EditActionRename(const EditActionRename&) = delete;
		EditActionRename& operator=(const EditActionRename&) = delete;
		virtual ~EditActionRename() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		// Access Names
		void SetPrevName(const char* pIn);
		void SetNewName(const char* pIn);
		const char* GetNewName() const;
		const char* GetPrevName() const;

	private:
		char newName[GameObject::NAME_SIZE];
		char prevName[GameObject::NAME_SIZE];
	};
}

#endif