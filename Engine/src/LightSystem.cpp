#include "LightSystem.h"

namespace Azul
{
	LightSystem::LightSystem()
	{
		memset(pointLights, 0, sizeof(PointLight) * MAX_LIGHTS);
		memset(&directionalLight, 0, sizeof(DirectionalLight));
	}

	LightSystem::~LightSystem()
	{
	}

	void LightSystem::Create()
	{
	}

	void LightSystem::Destroy()
	{
	}

	void LightSystem::Update()
	{
		LightSystem& self = GetInstance();
		memset(self.pointLights, 0, sizeof(PointLight) * MAX_LIGHTS);
		memset(&self.directionalLight, 0, sizeof(DirectionalLight));
	}

	void LightSystem::SetPointLight(const PointLight& inLight)
	{
		LightSystem& self = GetInstance();
		self.pointLights[PopPointLightIndex()] = inLight;
	}

	void LightSystem::SetDirectionalLight(const DirectionalLight& inLight)
	{
		LightSystem& self = GetInstance();
		self.directionalLight = inLight;
	}

	PointLight* LightSystem::GetPointLights()
	{
		return GetInstance().pointLights;
	}

	const DirectionalLight& LightSystem::GetDirectionalLight()
	{
		return GetInstance().directionalLight;
	}

	unsigned int LightSystem::PopPointLightIndex()
	{
		LightSystem& self = GetInstance();
		unsigned int output = self.pointLightIndex;
		self.pointLightIndex = (self.pointLightIndex + 1u) % MAX_LIGHTS;
		return output;
	}

	LightSystem& LightSystem::GetInstance()
	{
		static LightSystem instance;
		return instance;
	}
}