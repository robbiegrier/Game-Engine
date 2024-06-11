#ifndef _FoliageType
#define _FoliageType

#include "Mesh.h"
#include "TextureObject.h"

namespace Azul
{
	// Defines a type of foliage type that can be spawned on the terrain.
	class FoliageType : public DLink
	{
	public:
		enum class Name
		{
			DryGrassA,
			DryGrassB,
			DryGrassC,
			DryGrassD,
			SmallRockA,
			SmallRockB,
			TreeA,
			TreeB,
			TreeBirchA,
			None,
			Null
		};

		// Big four
		FoliageType() = delete;
		FoliageType(Mesh::Name meshName, TextureObject::Name textureName, float inBaseScale = 1.f);
		FoliageType(const FoliageType&) = delete;
		FoliageType operator=(const FoliageType&) = delete;
		~FoliageType() = default;

		// As a DLink
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(FoliageType::Name inName);
		FoliageType::Name GetName() const;
		const char* NameToString();
		static const char* NameToString(Name inName);

		// Add another model as part of this foliage type
		void AddModel(Mesh::Name meshName, TextureObject::Name textureName);

		// Set whether to align the up axis with the terrain normal
		void SetFollowNormal(bool inFollowNormal) { followNormal = inFollowNormal; }

		DLinkedList meshList;
		DLinkedList textureList;
		float baseScale = 1.f;
		bool followNormal = true;
		Name name;
	};
}

#endif