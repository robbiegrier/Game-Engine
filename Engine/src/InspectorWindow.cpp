#include "InspectorWindow.h"
#include "imgui.h"
#include "EditorGui.h"
#include "EditorObjectReference.h"
#include "GameObject.h"
#include "EditActionTranslate.h"
#include "EditActionRotate.h"
#include "EditActionScale.h"
#include "EditActionRename.h"
#include "TextureObject.h"
#include "EditorMath.h"

namespace Azul
{
	void InspectorWindow::Update()
	{
		ImGui::Begin("Inspector");

		if (!EditorGui::GetSelection().IsEmpty())
		{
			UpdateName();

			ImGuiTreeNodeFlags flags;
			flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader("Transform", flags))
			{
				UpdateTranslation();
				ImGui::NewLine();

				UpdateRotation();
				ImGui::NewLine();

				UpdateScale();
				ImGui::NewLine();
			}
			if (ImGui::CollapsingHeader("Graphics", flags))
			{
				GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();
				GraphicsObject* pGO = pGameObject->GetGraphicsObject();

				ImGui::BulletText("Model: %s", pGO->GetModel() ? pGO->GetModel()->NameToString() : "nullptr");
				ImGui::BulletText("Shader: %s", pGO->GetModel() ? pGO->GetShader()->NameToString() : "nullptr");
				ImGui::BulletText("Texture: %s", pGO->GetTexture() ? pGO->GetTexture()->NameToString() : "nullptr");

				if (pGO->GetModel())
				{
					Vec3 aabbMin = pGO->GetModel()->GetAABBMin();
					Vec3 aabbMax = pGO->GetModel()->GetAABBMax();
					ImGui::DragFloat3("AABB Min", &aabbMin[x]);
					ImGui::DragFloat3("AABB Max", &aabbMax[x]);

					Vec3 obbMin = pGameObject->GetOBBMin();
					Vec3 obbMax = pGameObject->GetOBBMax();
					ImGui::DragFloat3("OBB Min", &obbMin[x]);
					ImGui::DragFloat3("OBB Max", &obbMax[x]);
				}
			}

			if (ImGui::CollapsingHeader("Tree", flags))
			{
				GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();
				ImGui::BulletText("Parent: %s", pGameObject->GetParent()->GetNamePtr());

				if (pGameObject->GetChild()) ImGui::BulletText("Child: %s", pGameObject->GetChild()->GetNamePtr());
				if (pGameObject->GetNextSibling()) ImGui::BulletText("Next Sibling: %s", pGameObject->GetNextSibling()->GetNamePtr());
				if (pGameObject->GetPrevSibling()) ImGui::BulletText("Prev Sibling: %s", pGameObject->GetPrevSibling()->GetNamePtr());
				ImGui::BulletText("Depth: %d", pGameObject->GetDepth());
			}
		}

		ImGui::End();
	}

	void InspectorWindow::UpdateName()
	{
		GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();

		ImGui::InputText("Name", cacheName, GameObject::NAME_SIZE);

		const bool notRenaming = !ImGui::IsItemActive() && !wasNameActive;
		const bool startRename = ImGui::IsItemActive() && !wasNameActive;
		const bool endRename = !ImGui::IsItemActive() && wasNameActive;

		if (notRenaming)
		{
			pGameObject->GetName(cacheName, GameObject::NAME_SIZE);
		}
		else if (startRename)
		{
			wasNameActive = true;
		}
		else if (endRename)
		{
			wasNameActive = false;

			if (strcmp(cacheName, "") != 0)
			{
				char pPrevName[GameObject::NAME_SIZE];
				pGameObject->GetName(pPrevName, GameObject::NAME_SIZE);

				EditActionRename* pAction = new EditActionRename();
				pAction->SetSingleObject(pGameObject);
				pAction->SetPrevName(pPrevName);
				pAction->SetNewName(cacheName);

				pAction->Execute();
				pAction->Commit();

				Log("renamed %s to %s", pPrevName, cacheName);
			}
		}
	}

	void InspectorWindow::UpdateTranslation()
	{
		GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();

		ImGui::SeparatorText("Translation");
		ImGui::DragFloat3("Translate", (float*)&cacheTranslation, 0.5f);

		const bool notDragging = !ImGui::IsItemActive() && !wasTranslateActive;
		const bool startDrag = ImGui::IsItemActive() && !wasTranslateActive;
		const bool endDrag = !ImGui::IsItemActive() && wasTranslateActive;
		const bool updateDrag = ImGui::IsItemActive() && wasTranslateActive;

		if (notDragging)
		{
			translationStart = pGameObject->GetRelativeLocation();
			prevTranslation = translationStart;
			cacheTranslation = translationStart;
		}
		else if (startDrag)
		{
			wasTranslateActive = true;
		}
		else if (updateDrag)
		{
			for (Iterator& it = *EditorGui::GetSelection().GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
			{
				EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
				pCurr->GetGameObject()->AddRelativeLocationOffset(cacheTranslation - prevTranslation);
			}

			prevTranslation = cacheTranslation;
		}
		else if (endDrag)
		{
			wasTranslateActive = false;
			EditActionTranslate* pAction = new EditActionTranslate();
			pAction->SetObjects(EditorGui::GetSelection().GetSelectedObjectReferences());
			pAction->SetTranslation(pGameObject->GetRelativeLocation() - translationStart);
			pAction->Commit();
		}
	}

	void InspectorWindow::UpdateRotation()
	{
		GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();

		ImGui::SeparatorText("Rotation");

		if (ImGui::Button("Reset"))
		{
			pGameObject->SetRelativeRotation(Quat(Special::Identity));
		}
		const Quat& q = pGameObject->GetRelativeRotation();
		ImGui::Text("Quaternion [%f, %f, %f, %f]", q[x], q[y], q[z], q[w]);
		ImGui::DragFloat3("Rotate", (float*)&cacheRot, 0.5f);

		const bool notDragging = !ImGui::IsItemActive() && !wasRotateActive;
		const bool startDrag = ImGui::IsItemActive() && !wasRotateActive;
		const bool endDrag = !ImGui::IsItemActive() && wasRotateActive;
		const bool updateDrag = ImGui::IsItemActive() && wasRotateActive;

		if (notDragging)
		{
			rotationStart = pGameObject->GetRelativeRotation();
			cacheRot = EditorMath::QuatToEulerAngles(rotationStart);
			cacheRot[x] = EditorMath::RadiansToDegrees(cacheRot[x]);
			cacheRot[y] = EditorMath::RadiansToDegrees(cacheRot[y]);
			cacheRot[z] = EditorMath::RadiansToDegrees(cacheRot[z]);
		}
		else if (startDrag)
		{
			wasRotateActive = true;
			prevRot = rotationStart;
			prevRotE = cacheRot;
			totalRot = Quat();
		}
		else if (updateDrag)
		{
			Vec3 cacheRotRads = cacheRot - prevRotE;
			cacheRotRads[x] = EditorMath::DegreesToRadians(cacheRotRads[x]);
			cacheRotRads[y] = EditorMath::DegreesToRadians(cacheRotRads[y]);
			cacheRotRads[z] = EditorMath::DegreesToRadians(cacheRotRads[z]);
			Quat rot = EditorMath::EulerAnglesToQuat(cacheRotRads);

			for (Iterator& it = *EditorGui::GetSelection().GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
			{
				EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
				pCurr->GetGameObject()->AddWorldRotationOffset(rot);
			}

			prevRot = rot;
			totalRot *= rot;
			prevRotE = cacheRot;
		}
		else if (endDrag)
		{
			wasRotateActive = false;
			EditActionRotate* pAction = new EditActionRotate();
			pAction->SetObjects(EditorGui::GetSelection().GetSelectedObjectReferences());
			pAction->SetRotation(totalRot);
			pAction->Commit();
		}
	}

	void InspectorWindow::UpdateScale()
	{
		GameObject* pGameObject = (GameObject*)EditorGui::GetSelection().GetFocusObject()->GetGameObject();

		ImGui::SeparatorText("Scale");
		ImGui::DragFloat3("Scale", (float*)&cacheScale, 0.5f, 0.01f);

		const bool notDragging = !ImGui::IsItemActive() && !wasScaleActive;
		const bool startDrag = ImGui::IsItemActive() && !wasScaleActive;
		const bool endDrag = !ImGui::IsItemActive() && wasScaleActive;
		const bool updateDrag = ImGui::IsItemActive() && wasScaleActive;

		if (notDragging)
		{
			scaleStart = pGameObject->GetRelativeScale();
			prevScale = scaleStart;
			cacheScale = scaleStart;
		}
		else if (startDrag)
		{
			wasScaleActive = true;
		}
		else if (updateDrag)
		{
			for (Iterator& it = *EditorGui::GetSelection().GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
			{
				EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
				pCurr->GetGameObject()->AddRelativeScaleOffset(cacheScale - prevScale);
			}

			prevScale = cacheScale;
		}
		else if (endDrag)
		{
			wasScaleActive = false;
			EditActionScale* pAction = new EditActionScale();
			pAction->SetObjects(EditorGui::GetSelection().GetSelectedObjectReferences());
			pAction->SetScale(pGameObject->GetRelativeScale() - scaleStart);
			pAction->Commit();
		}
	}
}