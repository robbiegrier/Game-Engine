#include "EditorHistory.h"
#include "EditAction.h"
#include "EditorInput.h"

namespace Azul
{
	void EditorHistory::CommitAction(EditAction* pAction)
	{
		EditorHistory& self = GetInstance();
		self.historyStack.Push(pAction);
		self.ClearUndoneStack();
	}

	void EditorHistory::UndoAction()
	{
		EditorHistory& self = GetInstance();

		if (!self.historyStack.IsEmpty())
		{
			EditAction* pAction = (EditAction*)self.historyStack.Pop();
			pAction->Undo();

			self.undoneStack.Push(pAction);
		}
	}

	void EditorHistory::RedoAction()
	{
		EditorHistory& self = GetInstance();

		if (!self.undoneStack.IsEmpty())
		{
			EditAction* pAction = (EditAction*)self.undoneStack.Pop();
			pAction->Execute();

			self.historyStack.Push(pAction);
		}
	}

	void EditorHistory::OnUndoPressed()
	{
		UndoAction();
	}

	void EditorHistory::OnRedoPressed()
	{
		RedoAction();
	}

	void EditorHistory::ClearHistoryStack()
	{
		EditorHistory& self = GetInstance();

		while (!self.historyStack.IsEmpty())
		{
			DLink* pTmp = self.historyStack.Pop();
			delete pTmp;
		}
	}

	void EditorHistory::ClearUndoneStack()
	{
		EditorHistory& self = GetInstance();

		while (!self.undoneStack.IsEmpty())
		{
			DLink* pTmp = self.undoneStack.Pop();
			delete pTmp;
		}
	}

	void EditorHistory::ClearAllHistory()
	{
		ClearHistoryStack();
		ClearUndoneStack();
	}

	EditorHistory::EditorHistory()
	{
		EditorInput::AddObserver(this);
	}

	EditorHistory::~EditorHistory()
	{
	}

	EditorHistory& EditorHistory::GetInstance()
	{
		static EditorHistory instance;
		return instance;
	}

	void EditorHistory::Dump()
	{
		EditorHistory& self = GetInstance();

		Trace::out("EditorHistory -> History Stack:\n");
		for (Iterator& it = *self.historyStack.GetIterator(); !it.IsDone(); it.Next())
		{
			EditAction* pAction = (EditAction*)it.Curr();
			Trace::out("\t");
			pAction->Dump();
		}
		Trace::out("\n");

		Trace::out("EditorHistory -> Undone Stack:\n");
		for (Iterator& it = *self.undoneStack.GetIterator(); !it.IsDone(); it.Next())
		{
			EditAction* pAction = (EditAction*)it.Curr();
			Trace::out("\t");
			pAction->Dump();
		}
		Trace::out("\n");
	}

	void EditorHistory::EraseAction(EditAction* pAction)
	{
		EditorHistory& self = GetInstance();

		self.historyStack.Remove(pAction);
		self.undoneStack.Remove(pAction);
	}
}