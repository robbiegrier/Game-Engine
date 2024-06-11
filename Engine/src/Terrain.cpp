#include "Terrain.h"
#include "TerrainMesh.h"
#include "QuadTree.h"
#include "TerrainFoliage.h"

namespace Azul
{
	Terrain::Terrain(GraphicsObject* pInGraphicsObject)
		: GameObject(pInGraphicsObject),
		pTerrainMesh((TerrainMesh*)pInGraphicsObject->GetModel()),
		pQuadTree(new QuadTree())
	{
		pQuadTree->Create(pTerrainMesh);
		pTerrainMesh->GetFoliage()->Bind(this);
		typeName = Name::Terrain;
		SetIsSelectable(false);
	}

	Terrain::~Terrain()
	{
		delete pQuadTree;
	}

	void Terrain::Update(float deltaTime)
	{
		GameObject::Update(deltaTime);

		SetRelativeLocation(Vec3(0.f, 0.f, 0.f));
		SetRelativeRotation(Quat(Special::Identity));
		SetRelativeScale(1.f);
	}

	void Terrain::Draw()
	{
		GameObject::Draw();

		if (renderFoliage)
		{
			pTerrainMesh->GetFoliage()->Draw();
		}
	}

	QTNode* Terrain::GetTreeNodeAt2D(const Vec3& location) const
	{
		TerrainCoordinate coord;
		const bool status = GetCellAt2D(location, coord);
		return status ? pQuadTree->GetLeafNodeForVertex(coord) : nullptr;
	}

	bool Terrain::GetCellAt2D(const Vec3& location, TerrainCoordinate& outCoord) const
	{
		const Vec4 locationTerrainLocalSpace = Vec4(location, 1.f) * GetWorld().getInv();
		const float halfWorldScale = pTerrainMesh->GetWorldScale() / 2.f;
		const Vec4 terrainZeroZeroLocation = Vec4(-halfWorldScale, 0.f, -halfWorldScale, 1.f);

		Vec4 zeroZeroToLocation = locationTerrainLocalSpace - terrainZeroZeroLocation;

		const float scaledX = zeroZeroToLocation[x] / pTerrainMesh->GetSingleGridCellScale();
		const float scaledZ = zeroZeroToLocation[z] / pTerrainMesh->GetSingleGridCellScale();

		outCoord.x = static_cast<unsigned int>(scaledX);
		outCoord.y = static_cast<unsigned int>(scaledZ);

		const unsigned int resolution = pTerrainMesh->GetVertexResolution();

		return !(scaledX < 0.f || scaledZ < 0.f || outCoord.x >= resolution || outCoord.y >= resolution);
	}

	bool Terrain::GetCeilingCellAt2D(const Vec3& location, TerrainCoordinate& outCoord) const
	{
		const Vec4 locationTerrainLocalSpace = Vec4(location, 1.f) * GetWorld().getInv();
		const float halfWorldScale = pTerrainMesh->GetWorldScale() / 2.f;
		const Vec4 terrainZeroZeroLocation = Vec4(-halfWorldScale, 0.f, -halfWorldScale, 1.f);

		Vec4 zeroZeroToLocation = locationTerrainLocalSpace - terrainZeroZeroLocation;

		const float scaledX = zeroZeroToLocation[x] / pTerrainMesh->GetSingleGridCellScale();
		const float scaledZ = zeroZeroToLocation[z] / pTerrainMesh->GetSingleGridCellScale();

		outCoord.x = static_cast<unsigned int>(ceilf(scaledX));
		outCoord.y = static_cast<unsigned int>(ceilf(scaledZ));

		const unsigned int resolution = pTerrainMesh->GetVertexResolution();

		return !(scaledX < 0.f || scaledZ < 0.f || outCoord.x >= resolution || outCoord.y >= resolution);
	}

	Vec3 Terrain::GetTerrainVertexWorldLocation(const TerrainCoordinate& coordinate) const
	{
		return Vec3(Vec4(pTerrainMesh->GetVertex(coordinate.y, coordinate.x), 1.f) * GetWorld());
	}

	bool Terrain::GetBarycentricData(float& beta, float& gamma, Vec3& outA, Vec3& outB, Vec3& outC, const Vec3& P) const
	{
		TerrainCoordinate coord;
		Vec3 B, C;
		bool output;

		if (!GetCellAt2D(P, coord))
		{
			return false;
		}

		Vec3 A = pTerrainMesh->GetVertex(coord.y, coord.x);
		Vec3 v2 = Vec3((Vec4(P, 1.f) * GetWorld().getInv())) - A;

		if (-v2[x] > v2[z])
		{
			B = pTerrainMesh->GetVertex(coord.y, coord.x + 1);
			C = pTerrainMesh->GetVertex(coord.y + 1, coord.x + 1);
			output = true;
		}
		else
		{
			B = pTerrainMesh->GetVertex(coord.y + 1, coord.x + 1);
			C = pTerrainMesh->GetVertex(coord.y + 1, coord.x);
			output = false;
		}

		Vec3 v0 = B - A;
		Vec3 v1 = C - B;

		v0[y] = 0.f;
		v1[y] = 0.f;
		v2[y] = 0.f;

		const float a = v0.dot(v0);
		const float b = v1.dot(v0);
		const float c = v1.dot(v1);
		const float d = v2.dot(v0);
		const float e = v2.dot(v1);

		beta = (d * c - b * e) / (a * c - b * b);
		gamma = (a * e - d * b) / (a * c - b * b);
		outA.set(Vec3(Vec4(A, 1.f) * GetWorld()));
		outB.set(Vec3(Vec4(B, 1.f) * GetWorld()));
		outC.set(Vec3(Vec4(C, 1.f) * GetWorld()));

		return output;
	}

	float Terrain::ComputeBarycentricHeight(const Vec3& P) const
	{
		float BETA, GAMMA, output;
		Vec3 A, B, C;
		TerrainCoordinate coord;

		if (GetCellAt2D(P, coord))
		{
			GetBarycentricData(BETA, GAMMA, A, B, C, P);
			output = A.y() + BETA * (B.y() - A.y()) + GAMMA * (C.y() - B.y());
		}
		else
		{
			output = 0;
		}

		return output;
	}

	void Terrain::DebugDraw()
	{
		//pQuadTree->Draw();
	}
}