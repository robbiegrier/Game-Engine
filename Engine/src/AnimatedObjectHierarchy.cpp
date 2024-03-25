#include "AnimatedObjectHierarchy.h"
#include "GOLightTexture.h"
#include "GOFlatTexture.h"
#include "GameObjectBasic.h"
#include "GameObjectManager.h"
#include "ShaderObjectManager.h"
#include "GameObjectRigid.h"
#include "MeshManager.h"
#include "GameObjectAnimSkeleton.h"
#include "GameObjectAnimSkin.h"
#include "TextureObjectManager.h"
#include "PCSTreeForwardIterator.h"
#include "GONull.h"
#include "EngineUtils.h"
#include "skeletonData.h"
#include "Skeleton.h"
#include "GOSkinLightTexture.h"

namespace Azul
{
	AnimatedObjectHierarchy::AnimatedObjectHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture, AnimController* pInController, AnimMode inMode)
		: pRootAnimObject(nullptr), pController(pInController)
	{
		if (inMode == AnimMode::Skeletal)
		{
			privSetAnimationHierarchy(pBoneResult, pSkeleton, pInMesh, pInTexture);
		}
		else if (inMode == AnimMode::Skinned)
		{
			privSetSkinnedAnimationHierarchy(pBoneResult, pSkeleton, pInMesh, pInTexture);
		}
	}

	AnimatedObjectHierarchy::~AnimatedObjectHierarchy()
	{
	}

	GameObjectAnim* AnimatedObjectHierarchy::GetFirstAnimGameObject()
	{
		return pRootAnimObject;
	}

	GameObjectBasic* AnimatedObjectHierarchy::GetPivotGameObject()
	{
		return pPivot;
	}

	GameObjectAnim* AnimatedObjectHierarchy::FindBoneByIndex(int index)
	{
		GameObjectAnim* pObj = GetFirstAnimGameObject();

		if (pObj->index == index)
		{
			return pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode* pNode = pIter.First();
			GameObjectAnim* pGameObj = nullptr;

			while (pNode != nullptr)
			{
				pGameObj = (GameObjectAnim*)pNode;

				if (pGameObj->index == index)
				{
					return pGameObj;
				}

				pNode = pIter.Next();
			}
		}

		return nullptr;
	}

	void AnimatedObjectHierarchy::privSetAnimationHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture)
	{
		static_cast<void>(pInMesh);

		Mesh* MeshArray[8];

		int mi = 0;
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::BoneTransform);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L_001);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L_002);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R_001);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R_002);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_001);

		ShaderObject* pShader = ShaderObjectManager::Find(ShaderObject::Name::Default);
		TextureObject* pTex = pInTexture;

		GraphicsObject* pGraphicsObj;

		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);
		Vec3 spawnPos(0, 0, 0);

		pGraphicsObj = new GONull();
		pPivot = new GameObjectBasic(pGraphicsObj);
		pPivot->SetName("Pivot");
		pPivot->SetRelativeScale(100.f);
		pPivot->SetRelativeLocation(Vec3(30, -1, -50));
		pPivot->SetRelativeRotation(Rot(Rot1::Z, MATH_PI2));

		GameObjectManager::SpawnObject("Skeleton Pivot", pPivot, spawnPos);
		GameObjectAnimSkeleton* pGameObj;

		pGraphicsObj = new GOLightTexture(MeshArray[0], pShader, pTex);
		pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
		pGameObj->SetIndex(pSkeleton->GetBoneHierarchyNode(0).index);
		pGameObj->SetName(pSkeleton->GetBoneHierarchyNode(0).name);

		GameObjectManager::SpawnObject(pSkeleton->GetBoneHierarchyNode(0).name, pGameObj, spawnPos, pPivot);
		this->pRootAnimObject = pGameObj;

		for (int i = 1; i < pSkeleton->GetNumBones(); i++)
		{
			pGraphicsObj = new GOLightTexture(MeshArray[i], pShader, pTex);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(pSkeleton->GetBoneHierarchyNode(i).index);
			pGameObj->SetName(pSkeleton->GetBoneHierarchyNode(i).name);

			GameObjectAnim* pParent = FindBoneByIndex(pSkeleton->GetBoneHierarchyNode(i).parentIndex);
			GameObjectManager::SpawnObject(pSkeleton->GetBoneHierarchyNode(i).name, pGameObj, spawnPos, pParent);
		}

		pSkeletonSource = pSkeleton;
	}

	void AnimatedObjectHierarchy::privSetSkinnedAnimationHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture)
	{
		ShaderObject* pShader = ShaderObjectManager::Find(ShaderObject::Name::SkinLightTexture);

		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);
		Vec3 spawnPos(0, 0, 0);

		pPivot = new GameObjectBasic(new GONull());
		pPivot->SetName("Pivot");
		//pPivot->delta_y = 0.001f;

		GameObjectManager::SpawnObject("Skinned Skeleton Pivot", pPivot, spawnPos);

		GOSkinLightTexture* pGraphicsSkin = new GOSkinLightTexture(pInMesh, pShader, pInTexture);
		GameObjectAnimSkin* pGameObjectSkin = new GameObjectAnimSkin(pGraphicsSkin, this, pBoneResult);
		pGraphicsSkin->pBoneWorld = pGameObjectSkin->poBoneWorld;
		GameObjectManager::SpawnObject("Skinned Game Object", pGameObjectSkin, spawnPos, pPivot);

		pSkeletonSource = pSkeleton;
	}
}