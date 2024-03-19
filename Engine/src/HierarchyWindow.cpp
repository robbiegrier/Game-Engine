#include "HierarchyWindow.h"
#include "imgui.h"
#include "GameObjectManager.h"
#include "PCSNode.h"
#include "EditorGui.h"
#include "CameraManager.h"
#include "GameObject.h"
#include "EditorInput.h"
#include "EditActionReorder.h"

namespace Azul
{
	void HierarchyWindow::Update()
	{
		ImGui::Begin("Hierarchy", &open);
		bool selectionBool = false;
		ReorderSpacer((GameObject*)GameObjectManager::GetAllObjects().GetRoot(), nullptr);
		TreeBuildHelper(GameObjectManager::GetAllObjects().GetRoot()->GetChild(), selectionBool);
		ImGui::End();
	}

	void HierarchyWindow::TreeBuildHelper(PCSNode* pNode, bool& selectionBool)
	{
		if (pNode)
		{
			char name[GameObject::NAME_SIZE];
			pNode->GetName(name, GameObject::NAME_SIZE);
			GameObject* pObject = GameObjectManager::FindObject(name);

			bool isCurrentlySelected = false;

			if (pNode->GetChild())
			{
				if (pObject)
				{
					ImGuiTreeNodeFlags flags{ 0 };
					if (isCurrentlySelected) flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
					bool treeOpen = ImGui::TreeNodeEx(name, flags);

					DragDrop(pObject);
					GoTo(pObject, selectionBool);
					Selection(pObject, selectionBool);

					if (treeOpen)
					{
						ReorderSpacer(pObject, nullptr);
						TreeBuildHelper(pNode->GetChild(), selectionBool);
						ImGui::TreePop();
					}

					ReorderSpacer(pObject->GetParentGameObject(), pObject);
				}
			}
			else
			{
				if (pObject)
				{
					ImGui::Bullet();
					if (ImGui::Selectable(name, isCurrentlySelected)) {}

					DragDrop(pObject);
					GoTo(pObject, selectionBool);
					Selection(pObject, selectionBool);
					ReorderSpacer(pObject->GetParentGameObject(), pObject);
				}
			}

			TreeBuildHelper(pNode->GetNextSibling(), selectionBool);
		}
	}

	void HierarchyWindow::GoTo(GameObject* pObject, bool& selectionBool)
	{
		if (EditorGui::IsGoToRequested() && !selectionBool)
		{
			selectionBool = true;

			if (pObject)
			{
				CameraManager::GetCurrentCamera()->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), pObject->GetWorldLocation(),
					pObject->GetWorldLocation() + Vec3(0, 8, 15));

				EditorGui::UpdateSelectionOnClick(EditorInput::GetModState(), pObject);
			}
		}
	}

	void HierarchyWindow::Selection(GameObject* pObject, bool selectionBool)
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && !selectionBool)
		{
			EditorGui::UpdateSelectionOnClick(EditorInput::GetModState(), pObject);
		}
	}

	void HierarchyWindow::DragDrop(GameObject* pObject)
	{
		char name[GameObject::NAME_SIZE];
		pObject->GetName(name, GameObject::NAME_SIZE);

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("GameObjectPtr", &pObject, sizeof(GameObject*));
			ImGui::Text(name);
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectPtr"))
			{
				GameObject* pPayloadObject = *(GameObject**)payload->Data;
				PerformObjectReorder(pPayloadObject, pObject, nullptr);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void HierarchyWindow::ReorderSpacer(GameObject* pParent, GameObject* pPrev)
	{
		ImGui::Dummy(ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight() / 2.f));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectPtr"))
			{
				GameObject* pPayloadObject = *(GameObject**)payload->Data;
				PerformObjectReorder(pPayloadObject, pParent, pPrev);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void HierarchyWindow::PerformObjectReorder(GameObject* pObject, GameObject* pParent, GameObject* pPrev)
	{
		GameObject* pPrevParent = pObject->GetParentGameObject();
		GameObject* pPrevPrevSibling = (GameObject*)pObject->GetPrevSibling();

		if (!GameObjectManager::ReparentObject(pObject, pParent, pPrev))
		{
			char pDropName[GameObject::NAME_SIZE];
			pObject->GetName(pDropName, GameObject::NAME_SIZE);

			char pParentName[GameObject::NAME_SIZE];
			pParent->GetName(pParentName, GameObject::NAME_SIZE);

			Log("Cannot reparent %s under %s!", pDropName, pParentName);
		}
		else
		{
			EditActionReorder* pAction = new EditActionReorder();
			pAction->SetSingleObject(pObject);

			pAction->SetReorder(pPrevParent, pPrevPrevSibling, pParent, pPrev);
			pAction->Commit();
		}
	}
}