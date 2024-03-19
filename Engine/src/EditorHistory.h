#ifndef _EditorHistory
#define _EditorHistory

#include "DLinkedList.h"
#include "EditorInputObserver.h"

namespace Azul
{
	class EditAction;

	// Contains the history of actions which can be undone. Also contains undone actions which can be redone.
	// Redo-able actions are lost once the user makes new, manual changes and the history diverges.
	class EditorHistory : public EditorInputObserver
	{
	public:
		// Commit an action performed by the user to the history
		static void CommitAction(EditAction* pAction);

		// Undo from the history
		static void UndoAction();

		// Redo from the undone stack
		static void RedoAction();

		// As Input Observer
		virtual void OnUndoPressed() override;
		virtual void OnRedoPressed() override;

		// Clearing the stacks
		static void ClearHistoryStack();
		static void ClearUndoneStack();
		static void ClearAllHistory();

		// Debug history stacks
		static void Dump();

	private:
		// Big four
		EditorHistory();
		EditorHistory(const EditorHistory&) = delete;
		EditorHistory operator=(const EditorHistory&) = delete;
		~EditorHistory();

		// Meyers singleton
		static EditorHistory& GetInstance();

		// The stacks where actions are stored
		DLinkedList historyStack;
		DLinkedList undoneStack;
	};
}

#endif