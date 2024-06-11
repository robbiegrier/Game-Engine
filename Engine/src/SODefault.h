#ifndef _SODefault
#define _SODefault

#include "ShaderObject.h"
#include "MathEngine.h"
#include "LightSystem.h"

namespace Azul
{
	class GraphicsObject;

	class SODefault : public ShaderObject, public Align16
	{
	public:
		// Big four
		SODefault();
		SODefault(const SODefault&) = delete;
		SODefault& operator = (const SODefault&) = delete;
		~SODefault();

		virtual void SetFogParameters(float start, float range, const Vec4& col);

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		void SetCurrentObject(GraphicsObject* pObject);

	private:
		struct CBObject
		{
			Mat4 inverse;
			Material material;
		};
		ID3D11Buffer* pConstBuffObject;

		struct CBLightscape
		{
			int numPointLights;
			PointLight pointLights[LightSystem::MAX_LIGHTS];
			DirectionalLight directionalLight;
			Vec4 eyePositionWorld;
		};
		ID3D11Buffer* pConstBuffLightscape;

		struct CBFog
		{
			float FogStart = INFINITY;
			float FogRange = INFINITY;
			Vec4 FogColor = Vec4(0, 0, 0, 0);
		};
		CBFog fog;
		ID3D11Buffer* pConstBuffFog;

		enum class ConstBuffSlot : UINT
		{
			Lightscape = 5u,
			Object = 6u,
			Fog = 7u
		};
	};
}

#endif