#include "Skeleton.h"
#include "EngineUtils.h"
#include "skeletonData.h"

namespace Azul
{
	Skeleton::Skeleton(const char* skeletonFileName)
	{
		skeletonData_proto proto;
		proto.ParseFromString(EngineUtils::FileToString(skeletonFileName));

		skeletonData skeletonData(0);
		skeletonData.Deserialize(proto);
		numBones = skeletonData.numBones;
		hierarchyDepth = 0;

		for (int i = 0; i < numBones; i++)
		{
			hierarchyData[i].index = skeletonData.jointIndicies[i];
			hierarchyData[i].parentIndex = skeletonData.jointParentIndicies[i];
			memcpy(hierarchyData[i].name, skeletonData.jointNames[i], skeletonData::NAME_SIZE);
		}

		memset(hierarchyTable, -1, sizeof(hierarchyTable));

		for (int i = 0; i < numBones; i++)
		{
			int depth = HIERARCHY_DEPTH_MAX - 1;

			BoneHierarchyNode& iNode = hierarchyData[i];
			BoneHierarchyNode* curr = &hierarchyData[i];

			for (int j = i; j >= 0; j--)
			{
				BoneHierarchyNode& jNode = hierarchyData[j];
				if (jNode.index == curr->parentIndex || jNode.index == curr->index)
				{
					int index = (iNode.index * HIERARCHY_DEPTH_MAX) + depth;
					hierarchyTable[index] = jNode.index;

					curr = &jNode;
					depth--;
				}
			}

			int depthReached = (HIERARCHY_DEPTH_MAX - 1) - depth;
			hierarchyDepth = std::max(hierarchyDepth, depthReached);
		}

		for (int i = 0; i < numBones; i++)
		{
			BoneHierarchyNode& iNode = hierarchyData[i];

			int startIndex = 0;
			while (startIndex < HIERARCHY_DEPTH_MAX && hierarchyTable[((iNode.index * HIERARCHY_DEPTH_MAX) + startIndex)] == -1)
			{
				startIndex++;
			}

			if (startIndex < HIERARCHY_DEPTH_MAX)
			{
				int writeIndexDepth = 0;
				while (startIndex < HIERARCHY_DEPTH_MAX)
				{
					int indexWrite = (iNode.index * HIERARCHY_DEPTH_MAX) + writeIndexDepth;
					int indexRead = (iNode.index * HIERARCHY_DEPTH_MAX) + startIndex;
					hierarchyTable[indexWrite] = hierarchyTable[indexRead];
					startIndex++;
					writeIndexDepth++;
				}

				while (writeIndexDepth < HIERARCHY_DEPTH_MAX)
				{
					int indexWrite = (iNode.index * HIERARCHY_DEPTH_MAX) + writeIndexDepth;
					hierarchyTable[indexWrite] = -1;
					writeIndexDepth++;
				}
			}
		}

		Trace::out("Hierarchy data for %s:\n", skeletonFileName);
		for (int i = 0; i < numBones; i++)
		{
			Trace::out("%d: %d (parent %d) (%s)\n", i, hierarchyData[i].index, hierarchyData[i].parentIndex, hierarchyData[i].name);
		}

		Trace::out("Hierarchy Table for %s:\n", skeletonFileName);
		Trace::out("[Bones: %d] [Depth: %d]\n", numBones, hierarchyDepth);
		for (int i = 0; i < numBones; i++)
		{
			BoneHierarchyNode& iNode = hierarchyData[i];
			for (int j = 0; j < hierarchyDepth; j++)
			{
				int index = (iNode.index * HIERARCHY_DEPTH_MAX) + j;
				if (hierarchyTable[index] >= 0 && hierarchyTable[index] < 10) Trace::out(" ");
				Trace::out("%d ", hierarchyTable[index]);
			}
			Trace::out("  (node %d)\n", hierarchyData[i].index);
		}
	}

	Skeleton::~Skeleton()
	{
	}

	const BoneHierarchyNode& Skeleton::GetBoneHierarchyNode(int index) const
	{
		return hierarchyData[index];
	}

	int Skeleton::GetNumBones() const
	{
		return numBones;
	}

	void Skeleton::Wash()
	{
		name = Name::None;
	}

	bool Skeleton::Compare(DLink* pTargetNode)
	{
		return name == ((Skeleton*)pTargetNode)->name;
	}

	void Skeleton::Dump()
	{
		Trace::out("Skeleton: %s\n", NameToString());
	}

	void Skeleton::SetName(Skeleton::Name inName)
	{
		name = inName;
	}

	Skeleton::Name Skeleton::GetName() const
	{
		return name;
	}

	const char* Skeleton::NameToString()
	{
		switch (name)
		{
		case Azul::Skeleton::Name::ChickenBot:
			return "Walk";
		case Azul::Skeleton::Name::Null:
			return "Null";
		case Azul::Skeleton::Name::None:
			return "None";
		default:
			break;
		}

		return "None";
	}

	int* Skeleton::GetHierarchyTable() const
	{
		return (int*)hierarchyTable;
	}

	int Skeleton::GetHierarchyDepth() const
	{
		return hierarchyDepth;
	}
}