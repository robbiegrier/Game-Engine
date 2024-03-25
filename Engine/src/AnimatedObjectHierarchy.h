#ifndef _AnimatedObjectHeirarchy
#define _AnimatedObjectHeirarchy

#include "GameObjectAnimSkin.h"
#include "BoneTransform.h"
#include "BoneHierarchyNode.h"

namespace Azul
{
	class GameObjectBasic;
	class Skeleton;
	class Mesh;
	class TextureObject;
	class AnimController;

	enum class AnimMode
	{
		Skinned,
		Skeletal
	};

	class AnimatedObjectHierarchy
	{
	public:
		AnimatedObjectHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture, AnimController* pInController, AnimMode inMode);

		AnimatedObjectHierarchy() = delete;
		AnimatedObjectHierarchy(const AnimatedObjectHierarchy&) = delete;
		AnimatedObjectHierarchy& operator = (const AnimatedObjectHierarchy&) = delete;
		~AnimatedObjectHierarchy();

		GameObjectAnim* GetFirstAnimGameObject();
		GameObjectBasic* GetPivotGameObject();
		GameObjectAnim* FindBoneByIndex(int index);

		Skeleton* pSkeletonSource;
		AnimController* pController;

	private:
		void privSetAnimationHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture);
		void privSetSkinnedAnimationHierarchy(BoneTransform* pBoneResult, Skeleton* pSkeleton, Mesh* pInMesh, TextureObject* pInTexture);

	private:
		GameObjectAnim* pRootAnimObject;
		GameObjectBasic* pPivot;
		//int numBones;
		//BoneHierarchyNode hierarchyData[8];
	};
}

#endif
