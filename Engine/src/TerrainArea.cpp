#include "TerrainArea.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"

namespace Azul
{
	TerrainArea::TerrainArea(const Vec3& origin, float radius)
	{
		Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();

		if (pTerrain)
		{
			const Vec3 origin2D(origin.x(), 0.f, origin.z());
			const Vec3 offset2D(radius, 0.f, radius);
			const Vec3 areaMin2D = origin2D - offset2D;
			const Vec3 areaMax2D = origin2D + offset2D;

			pTerrain->GetCellAt2D(areaMin2D, min);
			pTerrain->GetCeilingCellAt2D(areaMax2D, max);

			if (areaMin2D.x() < -pTerrain->GetTerrainMesh()->GetHalfWorldScale()) min.x = 0u;
			if (areaMin2D.z() < -pTerrain->GetTerrainMesh()->GetHalfWorldScale()) min.y = 0u;
			if (areaMax2D.x() < -pTerrain->GetTerrainMesh()->GetHalfWorldScale()) max.x = 0u;
			if (areaMax2D.z() < -pTerrain->GetTerrainMesh()->GetHalfWorldScale()) max.y = 0u;

			min.x = std::min(min.x, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
			min.y = std::min(min.y, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
			max.x = std::min(max.x, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
			max.y = std::min(max.y, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
		}
	}

	TerrainArea::TerrainArea(const TerrainCoordinate& inMin, const TerrainCoordinate& inMax)
		: min(inMin), max(inMax)
	{
		Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
		max.x = std::min(max.x, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
		max.y = std::min(max.y, pTerrain->GetTerrainMesh()->GetVertexResolution() - 1u);
	}

	TerrainArea TerrainArea::Feather(uint feather) const
	{
		return TerrainArea(TerrainCoordinate(min.x - feather, min.y - feather), TerrainCoordinate(max.x + feather, max.y + feather));
	}

	TerrainArea TerrainArea::Shrink(uint shrink) const
	{
		return TerrainArea(TerrainCoordinate(min.x + shrink, min.y + shrink), TerrainCoordinate(max.x - shrink, max.y - shrink));
	}

	TerrainAreaIterator TerrainArea::begin() const
	{
		return TerrainAreaIterator(min, max);
	}

	TerrainAreaIterator TerrainArea::end() const
	{
		const TerrainCoordinate onePassedEnd(min.x, max.y + 1u);
		return TerrainAreaIterator(onePassedEnd, onePassedEnd);
	}

	TerrainArea TerrainArea::GetNorthWestQuadrant() const
	{
		TerrainCoordinate newMin;
		newMin.x = (min.x + max.x) / 2u;
		newMin.y = (min.y + max.y) / 2u;
		return TerrainArea(newMin, max);
	}

	TerrainArea TerrainArea::GetNorthEastQuadrant() const
	{
		TerrainCoordinate newMin = min;
		newMin.y = (min.y + max.y) / 2u;
		TerrainCoordinate newMax = max;
		newMax.x = (min.x + max.x) / 2u;
		return TerrainArea(newMin, newMax);
	}

	TerrainArea TerrainArea::GetSouthWestQuadrant() const
	{
		TerrainCoordinate newMin = min;
		newMin.x = (min.x + max.x) / 2u;
		TerrainCoordinate newMax = max;
		newMax.y = (min.y + max.y) / 2u;
		return TerrainArea(newMin, newMax);
	}

	TerrainArea TerrainArea::GetSouthEastQuadrant() const
	{
		TerrainCoordinate newMax = max;
		newMax.x = (min.x + max.x) / 2u;
		newMax.y = (min.y + max.y) / 2u;
		return TerrainArea(min, newMax);
	}

	TerrainArea TerrainArea::GetUnion(const TerrainArea& other) const
	{
		return TerrainArea(
			TerrainCoordinate(std::min(min.x, other.min.x), std::min(min.y, other.min.y)),
			TerrainCoordinate(std::max(max.x, other.max.x), std::max(max.y, other.max.y))
		);
	}

	void TerrainArea::UnionWith(const TerrainArea& other)
	{
		min.x = std::min(min.x, other.min.x);
		min.y = std::min(min.y, other.min.y);
		max.x = std::max(max.x, other.max.x);
		max.y = std::max(max.y, other.max.y);
	}

	TerrainArea TerrainArea::GetBisectionHorizontal() const
	{
		TerrainCoordinate newMax = max;
		newMax.y = (min.y + max.y) / 2u;
		return TerrainArea(min, newMax);
	}

	TerrainArea TerrainArea::GetBisectionVertical() const
	{
		TerrainCoordinate newMax = max;
		newMax.x = (min.x + max.x) / 2u;
		return TerrainArea(min, newMax);
	}

	TerrainArea TerrainArea::GetDifferenceSouthWise(TerrainArea& other) const
	{
		return TerrainArea(
			TerrainCoordinate(min.x, std::max(min.y, other.max.y)),
			TerrainCoordinate(max.x, max.y)
		);
	}

	bool TerrainArea::IsEmpty() const
	{
		return min.x >= max.x || min.y >= max.y;
	}

	TerrainAreaIterator::TerrainAreaIterator(const TerrainCoordinate& inStart, const TerrainCoordinate& inEnd)
		: curr(inStart), start(inStart), end(inEnd)
	{
	}

	bool TerrainAreaIterator::operator!=(TerrainAreaIterator rhs)
	{
		return curr != rhs.curr;
	}

	TerrainCoordinate& TerrainAreaIterator::operator*()
	{
		return curr;
	}

	void TerrainAreaIterator::operator++()
	{
		curr.x++;

		if (curr.x > end.x)
		{
			curr.x = start.x;
			curr.y++;
		}
	}
}