#include "FoliageType.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ListNode.h"

namespace Azul
{
	FoliageType::FoliageType(Mesh::Name meshName, TextureObject::Name textureName, float inBaseScale)
		: baseScale(inBaseScale)
	{
		AddModel(meshName, textureName);
	}

	void FoliageType::AddModel(Mesh::Name meshName, TextureObject::Name textureName)
	{
		Mesh* pMesh = MeshManager::Find(meshName);
		TextureObject* pTexture = TextureObjectManager::Find(textureName);
		meshList.Add(new ListNode(pMesh));
		textureList.Add(new ListNode(pTexture));
	}

	void FoliageType::Wash()
	{
	}

	bool FoliageType::Compare(DLink* pTargetNode)
	{
		FoliageType* pOther = (FoliageType*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void FoliageType::Dump()
	{
		Trace::out("FoliageType : %s\n", NameToString());
	}

	void FoliageType::SetName(FoliageType::Name inName)
	{
		name = inName;
	}

	FoliageType::Name FoliageType::GetName() const
	{
		return name;
	}

	const char* FoliageType::NameToString()
	{
		return NameToString(name);
	}

	const char* FoliageType::NameToString(Name inName)
	{
		switch (inName)
		{
		case Name::DryGrassA:
			return "DryGrassA";
		case Name::DryGrassB:
			return "DryGrassB";
		case Name::DryGrassC:
			return "DryGrassC";
		case Name::DryGrassD:
			return "DryGrassD";
		case Name::SmallRockA:
			return "SmallRockA";
		case Name::SmallRockB:
			return "SmallRockB";
		case Name::TreeA:
			return "TreeA";
		case Name::TreeB:
			return "TreeB";
		case Name::TreeBirchA:
			return "TreeBirchA";
		case Name::None:
			return "None";
		default:
			return "NO STRING FOR NAME";
		}
	}
}