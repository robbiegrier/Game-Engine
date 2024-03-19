#ifndef _EditAction
#define _EditAction

#include "DLink.h"
#include "DLinkedList.h"

namespace Azul
{
	class GameObject;

	// Command in command pattern for any action performed in the editor
	class EditAction : public DLink
	{
	public:
		enum class Name
		{
			Translate,
			Rotate,
			Scale,
			Spawn,
			Delete,
			Null
		};

		// Big four
		EditAction() = default;
		EditAction(const EditAction&) = delete;
		EditAction& operator=(const EditAction&) = delete;
		virtual ~EditAction() = default;

		// Name for debugging command stack
		void SetName(Name inName);
		Name GetName() const;
		const char* NameToString();

		// Manipulate objects that this action operates on
		void SetObjects(const DLinkedList& inObjectRefs);
		void SetSingleObject(GameObject* pSingleObject);
		void ClearObjects();

		// Helper to commit the action to the EditorHistory
		void Commit();

		// Action contract
		virtual void Execute() = 0;
		virtual void Undo() = 0;
		virtual void Dump() = 0;

	protected:
		Name name;
		DLinkedList objects;

	private:
		// Inherited via DLink
		void Wash() override;
		bool Compare(DLink* pTargetNode) override;
	};
}

#endif