#ifndef _Light
#define _Light

#include "Component.h"
#include "LightCommon.h"

namespace Azul
{
	class Light : public Component, public Align16
	{
	public:
		enum class Type
		{
			Directional,
			Point
		};

		Light() = default;
		Light(const Light&) = delete;
		Light& operator=(const Light&) = delete;
		virtual ~Light() = default;

		virtual void UpdateInspectorGui() override;

		virtual void Update(float deltaTime) override;
		virtual void Serialize(json& out) override;
		virtual void Deserialize(const json& in) override;

		void PushDirectionalLight();
		void PushPointLight();

		Type lightType;
		Material lightMaterial;
		Vec4 attenuation;
		float range;

		float intensity;
	};
}

#endif