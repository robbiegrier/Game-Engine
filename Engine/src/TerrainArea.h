#ifndef _TerrainArea
#define _TerrainArea

#include "TerrainUtils.h"

namespace Azul
{
	// Iterator for terrain area
	struct TerrainAreaIterator
	{
		TerrainAreaIterator() = delete;
		TerrainAreaIterator(const TerrainCoordinate& inStart, const TerrainCoordinate& inEnd);
		TerrainAreaIterator(const TerrainAreaIterator&) = default;
		TerrainAreaIterator& operator=(const TerrainAreaIterator&) = default;
		~TerrainAreaIterator() = default;
		bool operator!=(TerrainAreaIterator rhs);
		TerrainCoordinate& operator*();
		void operator++();

	private:
		TerrainCoordinate curr;
		TerrainCoordinate start;
		TerrainCoordinate end;
	};

	// Defines a rectangular area in the terrain
	class TerrainArea
	{
	public:
		TerrainArea() = default;
		TerrainArea(const Vec3& origin, float radius);
		TerrainArea(const TerrainCoordinate& inMin, const TerrainCoordinate& inMax);
		TerrainArea(const TerrainArea&) = default;
		TerrainArea& operator=(const TerrainArea&) = default;
		~TerrainArea() = default;

		TerrainArea Feather(uint feather) const;
		TerrainArea Shrink(uint shrink) const;

		const TerrainCoordinate& GetMin() const { return min; }
		const TerrainCoordinate& GetMax() const { return max; }

		const uint GetWidth() const { return max.x - min.x; }
		const uint GetHeight() const { return max.y - min.y; }

		TerrainAreaIterator begin() const;
		TerrainAreaIterator end() const;

		TerrainArea GetNorthWestQuadrant() const;
		TerrainArea GetNorthEastQuadrant() const;
		TerrainArea GetSouthWestQuadrant() const;
		TerrainArea GetSouthEastQuadrant() const;

		TerrainArea GetUnion(const TerrainArea& other) const;
		void UnionWith(const TerrainArea& other);

		TerrainArea GetBisectionHorizontal() const;
		TerrainArea GetBisectionVertical() const;

		TerrainArea GetDifferenceSouthWise(TerrainArea& other) const;

		bool IsEmpty() const;

	private:
		TerrainCoordinate min;
		TerrainCoordinate max;
	};
}

#endif