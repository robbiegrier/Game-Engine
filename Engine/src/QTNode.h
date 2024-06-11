#ifndef _QTNode
#define _QTNode

#include "TerrainUtils.h"
#include "Frustum.h"

namespace Azul
{
	// Point class internal to quad tree methods
	struct QTPoint
	{
		QTPoint() = default;
		QTPoint(float inX, float inZ) : x(inX), z(inZ) {}
		QTPoint(const QTPoint&) = default;
		QTPoint& operator=(const QTPoint&) = default;
		~QTPoint() = default;

		float x = 0.f;
		float z = 0.f;
	};

	// Rectangle class internal to quad tree methods
	struct QTRect
	{
		QTRect() = default;
		QTRect(const QTPoint& inMin, const QTPoint& inMax) : min(inMin), max(inMax) {}
		QTRect(float minX, float minZ, float maxX, float maxZ) : min(minX, minZ), max(maxX, maxZ) {}
		QTRect(const QTRect&) = default;
		QTRect& operator=(const QTRect&) = default;
		~QTRect() = default;

		QTPoint min;
		QTPoint max;
	};

	// Struct for return information about raycast hits with the quad tree
	struct QTHit
	{
		uint nodeIndex;
		float entryDistance = 0.f;
		float exitDistance = 0.f;
		bool hit = false;

		bool operator < (const QTHit& other) const { return entryDistance < other.entryDistance; }
	};

	// The node class for the quad tree.
	class QTNode
	{
	public:

		QTNode() = default;
		QTNode(const QTNode&) = delete;
		QTNode(QTNode&&) = delete;
		QTNode& operator=(const QTNode&) = delete;
		QTNode& operator=(QTNode&&) = delete;
		~QTNode() = default;

		QTRect area;
		float heightMin = 0.f;
		float heightMax = 0.f;
		uint nodeIndex = 0u;
		bool dirty = false;
		uint GetFirstChildIndex() const { return (nodeIndex * 4u) + 1u; }
		uint GetParentIndex() const { return (nodeIndex - 1u) / 4u; }
		static uint GetParentIndex(uint inIndex) { return (inIndex - 1u) / 4u; }

		Vec3 GetAABBMin() const { return Vec3(area.min.x, heightMin, area.min.z); }
		Vec3 GetAABBMax() const { return Vec3(area.max.x, heightMax, area.max.z); }

		Vec3 GetAABBMinOrMax(bool max) const { return max ? GetAABBMax() : GetAABBMin(); }

		bool RayIntersection(QTHit& outHit, const Vec3& rayOrigin, const Vec3& rayDir);
		bool FrustumIntersection(const Frustum& frustum);
	};

	// Return hit struct for raycast with individual terrain triangles.
	struct QTTriangleHit
	{
		float distance = 0.f;
		float u = 0.f;
		float v = 0.f;
		Vec3 verts[3];
		bool hit = false;
	};

	// Perform a ray vs triange intersection on the terrian
	bool RayVsTriangle(QTTriangleHit& outHit, const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& vert0, const Vec3& vert1, const Vec3& vert2);
}

#endif