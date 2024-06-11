#ifndef _LightCommon
#define _LightCommon

#include "MathEngine.h"

namespace Azul
{
	struct Material
	{
		Vec4 ambient;
		Vec4 diffuse;
		Vec4 specular;
	};

	struct PointLight
	{
		Material light;
		Vec4 position;
		Vec4 attenuation;
		float range;
	};

	struct DirectionalLight
	{
		Material light;
		Vec4 direction;
	};
}

#endif