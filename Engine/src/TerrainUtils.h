#ifndef _TerrainUtils
#define _TerrainUtils

#include "MeshUtils.h"
#include "MathEngine.h"

namespace Azul
{
	using uint = unsigned int;
	using TerrainCoordinate = Point<uint>;
	using ColorType = uint16_t;

	struct FoliageInstance : public Align16
	{
		float x;
		float z;
		float scale;
		float angle;
		Vec4 color;
	};
}

#endif