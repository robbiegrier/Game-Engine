#ifndef _LightSystem
#define _LightSystem

#include "LightCommon.h"

namespace Azul
{
	class LightSystem
	{
	public:
		static void Create();
		static void Destroy();
		static void Update();

		static void SetPointLight(const PointLight& inLight);
		static void SetDirectionalLight(const DirectionalLight& inLight);

		static PointLight* GetPointLights();
		static const DirectionalLight& GetDirectionalLight();

		static constexpr int MAX_LIGHTS = 15;

	private:
		LightSystem();
		LightSystem(const LightSystem&) = delete;
		LightSystem& operator=(const LightSystem&) = delete;
		~LightSystem();

		static unsigned int PopPointLightIndex();

		static LightSystem& GetInstance();

		DirectionalLight directionalLight;
		PointLight pointLights[MAX_LIGHTS];

		unsigned int pointLightIndex = 0u;
	};
}

#endif