#include "Selection.h"
#include "GameObject.h"
#include "EditorObjectReference.h"
#include "TranslateHandle.h"
#include "TextureObject.h"
#include "ScaleHandle.h"
#include "ScaleHandleUniform.h"
#include "RotateHandle.h"
#include "EditActionDelete.h"
#include "EditorMath.h"

namespace Azul
{
	Selection::Selection()
	{
		EnterTranslateMode();
	}

	Selection::~Selection()
	{
	}

	void Selection::AddObject(GameObject* pInObject)
	{
		EditorObjectReference* pEditorObjectReference = new EditorObjectReference(pInObject);

		selectedObjects.Push(pEditorObjectReference);
		pEditorObjectReference->OnSelected();

		for (Iterator& it = *editorVisuals.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorVisual* pCurr = (EditorVisual*)it.Curr();
			pCurr->SetLocationFromObject(pEditorObjectReference->GetGameObject());
			pCurr->Update();
		}
	}

	void Selection::RemoveObject(GameObject* pInObject)
	{
		EditorObjectReference* pFoundRef = nullptr;

		for (Iterator& it = *selectedObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();

			if (pCurr->GetGameObject() == pInObject)
			{
				pFoundRef = pCurr;
				break;
			}
		}

		if (pFoundRef)
		{
			RemoveObjectReference(pFoundRef);
		}
	}

	void Selection::DeleteObjects()
	{
		EditActionDelete* pAction = new EditActionDelete();
		pAction->SetObjects(selectedObjects);
		pAction->Execute();
		pAction->Commit();
	}

	bool Selection::IsEmpty() const
	{
		return selectedObjects.IsEmpty();
	}

	void Selection::Clear()
	{
		while (!selectedObjects.IsEmpty())
		{
			RemoveObjectReference((EditorObjectReference*)selectedObjects.Pop());
		}
	}

	void Selection::Update()
	{
		if (!selectedObjects.IsEmpty())
		{
			for (Iterator& it = *editorVisuals.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorVisual* pCurr = (EditorVisual*)it.Curr();
				pCurr->Update();
			}
		}
	}

	void Selection::Draw()
	{
		if (!selectedObjects.IsEmpty())
		{
			for (Iterator& it = *editorVisuals.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorVisual* pCurr = (EditorVisual*)it.Curr();
				pCurr->Draw();
			}
		}

		for (Iterator& it = *selectedObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->Draw();
		}
	}

	const DLinkedList& Selection::GetSelectedObjectReferences() const
	{
		return selectedObjects;
	}

	const DLinkedList& Selection::GetEditorVisuals() const
	{
		return editorVisuals;
	}

	void Selection::EnterTranslateMode()
	{
		ClearVisuals();

		EditorVisual* evX = new TranslateHandle();
		evX->SetTexture(TextureObject::Name::Red);
		evX->SetMutationAxis(EditorVisual::MutationAxis::X);

		EditorVisual* evY = new TranslateHandle();
		evY->SetTexture(TextureObject::Name::Green);
		evY->SetMutationAxis(EditorVisual::MutationAxis::Y);

		EditorVisual* evZ = new TranslateHandle();
		evZ->SetTexture(TextureObject::Name::Blue);
		evZ->SetMutationAxis(EditorVisual::MutationAxis::Z);

		editorVisuals.Add(evX);
		editorVisuals.Add(evY);
		editorVisuals.Add(evZ);

		OnModeChanged();
	}

	void Selection::EnterScaleMode()
	{
		ClearVisuals();

		EditorVisual* evX = new ScaleHandle();
		evX->SetTexture(TextureObject::Name::Red);
		evX->SetMutationAxis(EditorVisual::MutationAxis::X);

		EditorVisual* evY = new ScaleHandle();
		evY->SetTexture(TextureObject::Name::Green);
		evY->SetMutationAxis(EditorVisual::MutationAxis::Y);

		EditorVisual* evZ = new ScaleHandle();
		evZ->SetTexture(TextureObject::Name::Blue);
		evZ->SetMutationAxis(EditorVisual::MutationAxis::Z);

		EditorVisual* evUniform = new ScaleHandleUniform();
		evUniform->SetTexture(TextureObject::Name::Yellow);
		evUniform->SetMutationAxis(EditorVisual::MutationAxis::X);

		editorVisuals.Add(evX);
		editorVisuals.Add(evY);
		editorVisuals.Add(evZ);
		editorVisuals.Add(evUniform);

		OnModeChanged();
	}

	void Selection::EnterRotateMode()
	{
		ClearVisuals();

		EditorVisual* evX = new RotateHandle();
		evX->SetTexture(TextureObject::Name::Red);
		evX->SetMutationAxis(EditorVisual::MutationAxis::X);

		EditorVisual* evY = new RotateHandle();
		evY->SetTexture(TextureObject::Name::Green);
		evY->SetMutationAxis(EditorVisual::MutationAxis::Y);

		EditorVisual* evZ = new RotateHandle();
		evZ->SetTexture(TextureObject::Name::Blue);
		evZ->SetMutationAxis(EditorVisual::MutationAxis::Z);

		editorVisuals.Add(evX);
		editorVisuals.Add(evY);
		editorVisuals.Add(evZ);

		OnModeChanged();
	}

	EditorObjectReference* Selection::GetFocusObject() const
	{
		if (!selectedObjects.IsEmpty())
		{
			return (EditorObjectReference*)selectedObjects.First();
		}

		return nullptr;
	}

	void Selection::RemoveObjectReference(EditorObjectReference* pRef)
	{
		selectedObjects.Remove(pRef);
		pRef->OnDeselected();
		delete pRef;
	}

	void Selection::OnModeChanged()
	{
		for (Iterator& it = *editorVisuals.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorVisual* pCurr = (EditorVisual*)it.Curr();
			pCurr->SetParentSelection(this);
		}

		if (!selectedObjects.IsEmpty())
		{
			EditorObjectReference* pRef = (EditorObjectReference*)selectedObjects.First();

			for (Iterator& it = *editorVisuals.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorVisual* pCurr = (EditorVisual*)it.Curr();
				pCurr->SetLocationFromObject(pRef->GetGameObject());
				pCurr->Update();
			}
		}
	}

	void Selection::ClearVisuals()
	{
		while (!editorVisuals.IsEmpty())
		{
			DLink* tmp = editorVisuals.Pop();
			delete tmp;
		}
	}
}