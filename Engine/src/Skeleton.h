#ifndef _Skeleton
#define _Skeleton

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class GameObjectBasic;

	class Skeleton
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton(Bone* pBone, int numBones);

		Skeleton() = delete;
		Skeleton(const Skeleton&) = delete;
		Skeleton& operator = (const Skeleton&) = delete;
		~Skeleton();

		GameObjectAnim* GetFirstBone();
		GameObjectBasic* GetPivot();

	private:
		void privSetAnimationHierarchy(Bone* pBoneResult);
		GameObjectAnim* privFindBoneByIndex(int index);

	private:
		GameObjectAnim* pFirstBone;
		GameObjectBasic* pPivot;
		int             numBones;
	};
}

#endif
