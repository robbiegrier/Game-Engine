#include "EditorObjectReference.h"
#include "GameObject.h"
#include "GOFlatTexture.h"
#include "ShaderObjectManager.h"
#include "TextureObjectManager.h"
#include "MeshManager.h"
#include "CameraManager.h"

namespace Azul
{
	EditorObjectReference::EditorObjectReference(GameObject* pInObject)
		: pObject(pInObject)
	{
		pSelectionIndicator = new GOFlatTexture(
			MeshManager::Find(Mesh::Name::UnitIcoSphere),
			ShaderObjectManager::Find(ShaderObject::Name::EditorVisual),
			TextureObjectManager::Find(TextureObject::Name::Hovered)
		);
	}

	EditorObjectReference::~EditorObjectReference()
	{
		delete pSelectionIndicator;
	}

	void EditorObjectReference::SetGameObject(GameObject* pInObject)
	{
		pObject = pInObject;
	}

	GameObject* EditorObjectReference::GetGameObject() const
	{
		return pObject;
	}

	void EditorObjectReference::OnSelected()
	{
		char nameTmp[GameObject::NAME_SIZE];
		GetGameObject()->GetName(nameTmp, GameObject::NAME_SIZE);
		//Trace::out("%s added to the selection!\n", nameTmp);
	}

	void EditorObjectReference::OnDeselected()
	{
		char nameTmp[GameObject::NAME_SIZE];
		GetGameObject()->GetName(nameTmp, GameObject::NAME_SIZE);
		//Trace::out("%s removed from the selection!\n", nameTmp);
	}

	void EditorObjectReference::Draw()
	{
		//GetGameObject()->RenderShell();

		const Vec3& camPos = CameraManager::GetCurrentCamera()->GetLocation();
		const float dist = (camPos - GetGameObject()->GetWorldLocation()).len();
		const float scaleVal = dist * 0.01f;

		Trans trans(GetGameObject()->GetWorldLocation());
		Rot rot(Special::Identity);
		Scale scale(Vec3(scaleVal, scaleVal, scaleVal));

		Mat4 indWorld = scale * rot * trans;

		pSelectionIndicator->SetWorld(indWorld);
		pSelectionIndicator->Render();
	}

	void EditorObjectReference::StorePCSCache()
	{
		pcsCache.pParent = GetGameObject()->GetParentGameObject();
		pcsCache.pPrev = (GameObject*)GetGameObject()->GetPrevSibling();
	}

	const EditorObjectReference::PCSCache& EditorObjectReference::GetPCSCache() const
	{
		return pcsCache;
	}

	void EditorObjectReference::Wash()
	{
		pObject = nullptr;
	}

	bool EditorObjectReference::Compare(DLink* pTargetNode)
	{
		return GetGameObject() == ((EditorObjectReference*)pTargetNode)->GetGameObject();
	}
}