#ifndef _Skeleton
#define _Skeleton

#include "BoneHierarchyNode.h"
#include "DLink.h"
#include "Clip.h"

namespace Azul
{
	class Skeleton : public DLink
	{
	public:
		enum class Name
		{
			ChickenBot,
			MixamoRig,
			MixamoRig1,
			Null,
			None
		};

		explicit Skeleton(const char* skeletonFileName);
		Skeleton() = delete;
		Skeleton(const Skeleton&) = delete;
		Skeleton& operator=(const Skeleton&) = delete;
		~Skeleton();

		const BoneHierarchyNode& GetBoneHierarchyNode(int index) const;
		int GetNumBones() const;

		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Skeleton::Name inName);
		Skeleton::Name GetName() const;
		const char* NameToString();

		static constexpr unsigned int HIERARCHY_TABLE_SIZE = BONE_COUNT_MAX * HIERARCHY_DEPTH_MAX;
		int* GetHierarchyTable() const;

		int GetHierarchyDepth() const;

	private:
		int numBones;
		int hierarchyDepth;
		BoneHierarchyNode hierarchyData[BONE_COUNT_MAX];
		int hierarchyTable[HIERARCHY_TABLE_SIZE];

		Name name;
	};
}

#endif