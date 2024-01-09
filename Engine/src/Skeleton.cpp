#include "Skeleton.h"
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

namespace Azul
{
	//Skeleton::Data skel[]
	//{
	//	{ 4, -1, "Bone" },
	//	{ 8, 4, "Bone_R" },
	//	{ 9, 8, "Bone_R_001" },
	//	{ 10, 9, "Bone_R_002" },
	//	{ 5, 4, "Bone_L" },
	//	{ 6, 5, "Bone_L_001" },
	//	{ 7, 6, "Bone_L_002" },
	//	{ 11, 4, "Bone_001" },
	//};

	Skeleton::Skeleton(Bone* pBone, int _numBones, const char* pSkeletonFilename)
		: pFirstBone(nullptr),
		numBones(_numBones)
	{
		skeletonData_proto proto;
		proto.ParseFromString(EngineUtils::FileToString(pSkeletonFilename));

		skeletonData skeletonData;
		skeletonData.Deserialize(proto);

		skeletonData.Print("loaded skelly");

		for (int i = 0; i < numBones - 4; i++)
		{
			skeletonBoneData[i].index = skeletonData.jointIndicies[i];
			skeletonBoneData[i].parentIndex = skeletonData.jointParentIndicies[i];
			memcpy(skeletonBoneData[i].name, skeletonData.jointNames[i], skeletonData::NAME_SIZE);
		}

		this->privSetAnimationHierarchy(pBone);
		assert(pFirstBone);
	}

	Skeleton::~Skeleton()
	{
	}

	GameObjectAnim* Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	GameObjectBasic* Skeleton::GetPivot()
	{
		return pPivot;
	}

	GameObjectAnim* Skeleton::privFindBoneByIndex(int index)
	{
		GameObjectAnim* pFound = nullptr;

		GameObjectAnim* pObj = this->GetFirstBone();

		if (pObj->index == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode* pNode = pIter.First();
			GameObjectAnim* pGameObj = nullptr;

			while (pNode != nullptr)
			{
				assert(pNode);
				pGameObj = (GameObjectAnim*)pNode;
				if (pGameObj->index == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(Bone* pBoneResult)
	{
#if 1
		assert(pBoneResult);

		Mesh* pPyramidMesh = MeshManager::Find(Mesh::Name::Pyramid);
		assert(pPyramidMesh);

		Mesh* MeshArray[8];

		int mi = 0;
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L_001);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_L_002);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R_001);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_R_002);
		MeshArray[mi++] = MeshManager::Find(Mesh::Name::Bone_001);

		ShaderObject* pShader = ShaderObjectManager::Find(ShaderObject::Name::Default);
		TextureObject* pTex = TextureObjectManager::Find(TextureObject::Name::ChickenBot);

		GraphicsObject* pGraphicsObj;

		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);
		Vec3 spawnPos(0, 0, 0);

		pGraphicsObj = new GONull();
		pPivot = new GameObjectBasic(pGraphicsObj);
		pPivot->SetName("Pivot");
		pPivot->delta_y = 0.003f;
		pPivot->SetScale(100, 100, 100);
		pPivot->SetTrans(30, -1, -50);
		pPivot->SetQuat(Quat(Rot1::Z, MATH_PI2));

		GameObjectManager::SpawnObject("Skeleton Pivot", pPivot, spawnPos);
		GameObjectAnim* pGameObj;

		pGraphicsObj = new GOLightTexture(MeshArray[0], pShader, pTex);
		pGameObj = new GameObjectAnimSkin(pGraphicsObj, pBoneResult);
		pGameObj->SetIndex(skeletonBoneData[0].index);
		pGameObj->SetName(skeletonBoneData[0].name);

		GameObjectManager::SpawnObject(skeletonBoneData[0].name, pGameObj, spawnPos, pPivot);
		this->pFirstBone = pGameObj;

		for (int i = 1; i < this->numBones - 4; i++)
		{
			pGraphicsObj = new GOLightTexture(MeshArray[i], pShader, pTex);
			pGameObj = new GameObjectAnimSkin(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(skeletonBoneData[i].index);
			pGameObj->SetName(skeletonBoneData[i].name);

			GameObjectAnim* pParent = this->privFindBoneByIndex(skeletonBoneData[i].parentIndex);
			GameObjectManager::SpawnObject(skeletonBoneData[i].name, pGameObj, spawnPos, pParent);
		}
#else
		assert(pBoneResult);

		Mesh* pPyramidMesh = MeshManager::Find(Mesh::Name::Cube);
		ShaderObject* pShaderObject_textureLight = ShaderObjectManager::Find(ShaderObject::Name::FlatTexture);
		TextureObject* pTex = TextureObjectManager::Find(TextureObject::Name::Crate);

		GraphicsObject* pGraphicsObj;

		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);
		Vec3 spawnPos(0, 0, 0);

		pGraphicsObj = new GONull();
		pPivot = new GameObjectBasic(pGraphicsObj);
		pPivot->SetName("Pivot");
		pPivot->delta_y = 0.003f;
		pPivot->SetScale(100, 100, 100);
		pPivot->SetTrans(30, -1, -50);
		pPivot->SetQuat(Quat(Rot1::Z, MATH_PI2));

		GameObjectManager::SpawnObject("Skeleton Pivot", pPivot, spawnPos);
		GameObjectAnim* pGameObj;

		pGraphicsObj = new GOLightTexture(pPyramidMesh, pShaderObject_textureLight, pTex);
		pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
		pGameObj->SetIndex(skel[0].index);
		pGameObj->SetName(skel[0].name);

		GameObjectManager::SpawnObject(skel[0].name, pGameObj, spawnPos, pPivot);
		this->pFirstBone = pGameObj;

		for (int i = 1; i < this->numBones - 4; i++)
		{
			pGraphicsObj = new GOLightTexture(pPyramidMesh, pShaderObject_textureLight, pTex);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(skel[i].index);
			pGameObj->SetName(skel[i].name);

			GameObjectAnim* pParent = this->privFindBoneByIndex(skel[i].parentIndex);
			GameObjectManager::SpawnObject(skel[i].name, pGameObj, spawnPos, pParent);
		}

#endif
	}
}

// --- End of File ---